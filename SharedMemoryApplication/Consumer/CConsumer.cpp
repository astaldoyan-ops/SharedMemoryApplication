#include "CConsumer.h"

#include <cstring>
#include <exception>
#include <stdexcept>

#include <iostream>
#include <thread>

namespace Ipcs {
    class CMutexReceive: public CMutex
    {
    private:
        CMutexReceive() {
            m_object = sem_open(m_name.c_str(), 0);
        }
    public:
        virtual ~CMutexReceive() {
            sem_close(m_object);
        }
        static CMutexReceive& instance() {
            static CMutexReceive s_instance;
            return s_instance;
        }
    };

    class CGuardReceive
    {
    public:
        CGuardReceive( ) { CMutexReceive::instance( ).lock(); }
        virtual ~CGuardReceive( ) { CMutexReceive::instance( ).release( ); }
    };
}

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
        auto ch = Signals::kbhit_getKey();
        if( ch == Signals::chEscape ) {
            return true;
        }
        else {
            return false;
        }
    }

    CConsumer::CReceiver::CReceiver( )
        : m_payloadSize(0)
        , m_sequenceNumber(0)
    {
        std::cout << Ipcs::s_objectName.c_str() << std::endl;
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

            Ipcs::CGuardReceive g;
            auto objSize = getFrameSize();
            m_payloadSize = objSize - sizeof( Framing::Header );
            auto *storage = mmap( nullptr, objSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_ShmFileDescriptor, 0 );

            if( MAP_FAILED == storage ) continue;

            auto* inStorage = reinterpret_cast<Framing::Fields*>( storage );
            if( inStorage->m_sequenceNumber > m_sequenceNumber ) {
                m_sequenceNumber = inStorage->m_sequenceNumber;
                frameUpdated = true;
            }
            else {
                std::cerr << "Frame not updated by producer" << std::endl;
                continue;
            }

            memcpy( _frame.header( ), inStorage, sizeof( Framing::Fields ) );
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
