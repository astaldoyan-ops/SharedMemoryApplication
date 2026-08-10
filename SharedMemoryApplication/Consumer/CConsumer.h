#pragma once

#include "../Basics/Basics.h"

namespace Consumers {

	class CConsumer: public Applications::CApplication
	{
	public:

		class CReceiver : public Ipcs::CIpcUnit
		{
		public:
			CReceiver( );
			CIpcUnit& process( Framing::Frame& _frame ) override;

		private:
			size_t m_payloadSize;
			size_t m_sequenceNumber;

            size_t getFrameSize();

            bool checkIfHeaderValid(const Framing::Header& _header) const;
            Framing::Header m_header;
		};

		CConsumer( );
		CConsumer& processFrame( );

		CApplication& run( ) override;

	private:
		CReceiver m_ipc;
		
		bool toStop( ) const;
	};

}
