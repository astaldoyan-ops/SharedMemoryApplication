#pragma once

#include "../Basics/Basics.h"

#include <memory>
#include <functional>

namespace Producers {
	

	class CProducer: public Applications::CApplication
	{
	public:
		CProducer( size_t _size, Framing::Filler& _filler );
		Applications::CApplication& run( ) override;

	private:

		class CSubmitter : public Ipcs::CIpcUnit
		{
		public:
			CSubmitter( size_t _payloadSize );
			CIpcUnit& process( Framing::Frame& _frame ) override;
		private:
			size_t m_payloadSize;
			size_t m_frameCounter;
            bool m_firstFrame;
		};

		size_t m_payloadSize;
		Framing::Filler m_filler;

		CSubmitter m_ipc;

		std::unique_ptr<Framing::Frame> createFrame( );

		void processFrame( );

		bool toStop( ) const;
	};

}

