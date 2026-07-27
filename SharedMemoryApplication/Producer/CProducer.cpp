#include "CProducer.h"

#include <cstring>
#include <exception>
#include <stdexcept>

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

	void CProducer::processFrame( ) {
		auto frame = createFrame( );
		m_ipc.process( *frame);
	}

	bool CProducer::toStop( ) const
	{
        //if( _kbhit( ) && ( _getch( ) == Signals::chEscape ) ) return true;
        //else return false;

        return false;
	}

	CProducer::CSubmitter::CSubmitter( size_t _payloadSize )
		: m_payloadSize( _payloadSize )
		, m_frameCounter(0)
	{
		
	}

	Ipcs::CIpcUnit& CProducer::CSubmitter::process( Framing::Frame& _frame )
	{
		bool lastFrameReceived{ false };

		while( !lastFrameReceived ) {

			Ipcs::CGuard g;

			char* storage = reinterpret_cast<char*>(
				mmap( nullptr, sizeof( Framing::Header ) + m_payloadSize, PROT_READ, MAP_PRIVATE, m_ShmFileDescriptor, 0 )
			);

			if( MAP_FAILED == storage ) continue;

			auto* inStorage = reinterpret_cast<Framing::Header*>(storage);
			if( size_t(0) == inStorage->m_sequenceNumber ) lastFrameReceived = true;
			else continue;

			m_frameCounter = _frame.header( )->m_sequenceNumber;

			memcpy( storage, _frame.header(), sizeof(Framing::Header) );
			memcpy( storage + sizeof( Framing::Header ), _frame.payload().c_str( ), m_payloadSize );

			if( -1 == munmap( storage, sizeof( Framing::Header ) + m_payloadSize ) ) {
                throw std::runtime_error( "Submitter: Unable to unmap shared memory object" );
			}
		}

		return *this;
	}

}
