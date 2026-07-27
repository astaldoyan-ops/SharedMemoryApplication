#include "CConsumer.h"

#include <conio.h>

namespace Consumers {

    CConsumer::CConsumer( )
    {
    }

    CConsumer& CConsumer::processFrame( ) {

        Framing::Frame frame( m_ipc.storageSize( ) );
        m_ipc.process( frame );

        return *this;
    }

    Applications::CApplication& CConsumer::run( )
    {
        while( !toStop( ) ) {
            processFrame( );
        }
        return *this;
    }

    bool CConsumer::toStop( ) const
    {
        if( _kbhit( ) && ( _getch( ) == Signals::chEscape ) ) return true;
        else return false;
    }

    CConsumer::CReceiver::CReceiver( )
        : m_payloadSize(0)
        , m_sequenceNumber(0)
    {
    }

    Ipcs::CIpcUnit& CConsumer::CReceiver::process( Framing::Frame& _frame )
    {
        bool frameUpdated{ false };

        while( !frameUpdated ) {

            Ipcs::CGuard g;

            struct stat stats;
            fstat( m_ShmFileDescriptor, &stats );
            auto objSize = stats.st_size;
            m_payloadSize = objSize - sizeof( Framing::Header );
            auto *storage = mmap( nullptr, objSize, PROT_READ, MAP_PRIVATE, m_ShmFileDescriptor, 0 );

            if( MAP_FAILED == storage ) continue;

            auto* inStorage = reinterpret_cast<Framing::Header*>( storage );
            if( inStorage->m_sequenceNumber > m_sequenceNumber ) {
                m_sequenceNumber = inStorage->m_sequenceNumber;
            }
            else continue;

            memcpy( _frame.header( ), inStorage, sizeof( Framing::Header ) );
            _frame.payload( ) = std::move( std::string( reinterpret_cast<const char *>( & inStorage[1] ), m_payloadSize ) );

            inStorage->m_sequenceNumber = 0;    // mark frame received

            if( -1 == munmap( storage, objSize ) ) {
                throw std::exception( "Receiver: Unable to unmap shared memory object" );
            }

        }

        return *this;
    }

}
