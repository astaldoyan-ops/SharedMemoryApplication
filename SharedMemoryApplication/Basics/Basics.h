#pragma once

#include <chrono>

namespace Applications {

	class CApplication
	{
	public:
		CApplication( ) = default;
		CApplication( CApplication& ) = delete;

		virtual CApplication& run( ) = 0;
	};

}

namespace Ipcs {

}

namespace Algorythms {

	class CRC {
		
	public:
		typedef char CRC8;
		CRC( char* _start, unsigned long _length );
		operator CRC8( ) const;

	private:
		CRC8 calculate(  );

	};

}

namespace Framing {

	struct Header {
		std::chrono::high_resolution_clock m_timestamp;
		unsigned long m_sequenceNumber;
		Algorythms::CRC::CRC8 m_Crc8;
	};
}

namespace Signals {
	const char chEscape{ 0x1B };
}
