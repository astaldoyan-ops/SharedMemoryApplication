#include "CProducer.h"

#include <cstring>
#include <exception>
#include <stdexcept>
#include <thread>

#pragma pack (1)

namespace Ipcs {

    class CMutexSubmit: public CMutex
    {
    private:
        CMutexSubmit() {
            sem_unlink(m_name.c_str());
            m_object = sem_open(m_name.c_str(), O_CREAT, 0666, 1);
        }
    public:
        virtual ~CMutexSubmit() {
            sem_close(m_object);
            sem_unlink(m_name.c_str());
        }
        static CMutexSubmit& instance() {
            static CMutexSubmit s_instance;
            return s_instance;
        }
    };

    class CGuardSubmit
    {
    public:
        CGuardSubmit( ) { CMutexSubmit::instance( ).lock(); }
        virtual ~CGuardSubmit( ) { CMutexSubmit::instance( ).release( ); }
    };
}

namespace Producers {

	CProducer::CProducer( size_t _size, Framing::Filler& _filler )
		: m_payloadSize(_size)
		, m_filler(_filler)
		, m_ipc( _size )
	{}

	Applications::CApplication& CProducer::run( ) {

		while( !toStop( ) ) {
			processFrame( );
		}

		return *this;
	}

    std::unique_ptr<Framing::Frame> CProducer::createFrame( ) {
        auto product = std::make_unique<Framing::Frame>(m_payloadSize, m_filler);
        product->finish();

        return std::move(product);
    }

	void CProducer::processFrame( ) {
		auto frame = createFrame( );
		m_ipc.process( *frame);
	}

	bool CProducer::toStop( ) const
	{
        auto ch = Signals::kbhit_getKey( );
        if(  ch == Signals::chEscape ) return true;
        else return false;

	}

	CProducer::CSubmitter::CSubmitter( size_t _payloadSize )
		: m_payloadSize( _payloadSize )
		, m_frameCounter(0)
        , m_firstFrame(true)
	{
        m_ShmFileDescriptor = shm_open( Ipcs::s_objectName.c_str(), O_CREAT | O_RDWR, 0666 );
        if( -1 == ftruncate(m_ShmFileDescriptor, sizeof(Framing::Header) + m_payloadSize)) {
            throw std::runtime_error("Can not establish Shared Memory Size");
        }
	}

	Ipcs::CIpcUnit& CProducer::CSubmitter::process( Framing::Frame& _frame )
	{
		bool lastFrameReceived{ false };

        //std::this_thread::sleep_for(std::chrono::milliseconds(1));

		while( !lastFrameReceived ) {

            Ipcs::CGuardSubmit g;

            auto size = sizeof( Framing::Fields ) + m_payloadSize;

            auto * storage = reinterpret_cast<Framing::Fields*>(
                mmap( nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, m_ShmFileDescriptor, 0 )
			);

			if( MAP_FAILED == storage ) continue;

            if( m_firstFrame || (size_t(0) == storage->m_sequenceNumber)) {
                lastFrameReceived = true;
            }
            else {
                std::cerr << "Last Frame not received" << std::endl;
                continue;
            }

			m_frameCounter = _frame.header( )->m_sequenceNumber;

            memcpy( storage, _frame.header(), sizeof(Framing::Fields) );
            memcpy( &storage[1], _frame.payload().c_str( ), m_payloadSize );

            m_firstFrame = false;

            if( -1 == munmap( storage, sizeof( Framing::Fields ) + m_payloadSize ) ) {
                throw std::runtime_error( "Submitter: Unable to unmap shared memory object" );
			}

            std::cout << "Stored: " << _frame.payload() << std::endl;
            std::cout << "Frame: " << _frame.header()->m_sequenceNumber << std::endl;
		}

		return *this;
	}

}
