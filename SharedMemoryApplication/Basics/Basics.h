#pragma once

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
