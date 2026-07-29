#include "CConsumer.h"

#include <cstring>
#include <exception>
#include <stdexcept>

#include <iostream>

namespace Consumers {

    CConsumer::CConsumer( )
    {
    }

    CConsumer& CConsumer::processFrame( ) {

        Framing::Frame frame( m_ipc.storageSize( ) );
        m_ipc.process( frame );
        std::cout << frame.payload() << std::endl;

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
        if( Signals::kbhit( ) && ( Signals::getKey()  == Signals::chEscape ) ) return true;
        else return false;
    }

    CConsumer::CReceiver::CReceiver( )
        : m_payloadSize(0)
        , m_sequenceNumber(0)
    {
        m_ShmFileDescriptor = shm_open( Ipcs::s_objectName.c_str(), O_RDWR, 0666 );
    }

    size_t CConsumer::CReceiver::getFrameSize() {
        struct stat stats;
        fstat( m_ShmFileDescriptor, &stats );
        return stats.st_size;
    }

    Ipcs::CIpcUnit& CConsumer::CReceiver::process( Framing::Frame& _frame )
    {
        bool frameUpdated{ false };

        while( !frameUpdated ) {

            Ipcs::CGuard g;
            auto objSize = getFrameSize();
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
            _frame.finish( );

            inStorage->m_sequenceNumber = 0;    // mark frame received

            if( -1 == munmap( storage, objSize ) ) {
                throw std::runtime_error( "Receiver: Unable to unmap shared memory object" );
            }


        }

        return *this;
    }

}
