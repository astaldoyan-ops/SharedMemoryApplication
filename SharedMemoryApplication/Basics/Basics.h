#pragma once

#include <chrono>
#include <functional>

#include "MyPosix.h"

namespace Signals {
	const char chEscape{ 0x1B };
}

namespace Algorythms {

	class CRC {

	public:
		typedef char CRC8;
		CRC( char* _start, size_t _length );
		operator CRC8( ) const;

	private:
		CRC8 calculate( );

	};
}

namespace Applications {

	class CApplication
	{
	public:
		CApplication( ) = default;
		CApplication( CApplication& ) = delete;

		virtual CApplication& run( ) = 0;
	};

}

namespace Framing {

	using Filler = std::function<void( std::string& dest, size_t _size )>;

	size_t number( ) {
		static size_t value{ 0 };
		return ++value;
	}

	struct Header {
		Header( )
			: m_timestamp( std::chrono::high_resolution_clock::now() )
			, m_sequenceNumber( number() )
			, m_hash( 0 )
		{}

		Header& finish( const std::string& _payload) {
			m_hash = std::hash<std::string>{}( _payload );
			return *this;
		}
		std::chrono::time_point<std::chrono::high_resolution_clock> m_timestamp;
		size_t m_sequenceNumber;
		size_t m_hash;
	};

	struct Frame {
	public:
		Frame( size_t _payloadSize );
		Frame( size_t _payloadSize, Filler& _filler );
		Framing::Header* header( ) { return &m_header; }
		std::string& payload( ) { return m_payload; }
		Frame& finish( ) { m_header.finish( m_payload ); return *this; }
	private:
		Filler& m_filler;
		Framing::Header m_header;
		std::string m_payload;
	};
}

namespace Ipcs {

	class CMutex
	{
	public:
		static CMutex& instance( ) {
			static CMutex s_instance;
			return s_instance;
		};
		CMutex& lock( ) { pthread_mutex_lock( &m_object ); return *this; }
		CMutex& release( ) { pthread_mutex_unlock( &m_object ); return *this; }

	private:
		CMutex( );
		pthread_mutex_t m_object;
	};

	static const std::string s_objectName = "Created Boa Dedicated Deviro Shared Memory object";

	class CIpcUnit
	{
	public:
		CIpcUnit( ) {
			m_ShmFileDescriptor = shm_open( s_objectName.c_str(), O_CREAT | O_RDWR, 0666 );
		}

		size_t storageSize( ) const {
			struct stat stats;
			if( 0 == fstat( m_ShmFileDescriptor, &stats ) ) return stats.st_size;
			else return -1;
		}

		virtual CIpcUnit& process( Framing::Frame& _frame ) = 0;

	protected:
		int m_ShmFileDescriptor;
	};

	class CGuard
	{
	public:
		CGuard( ) { CMutex::instance( ).lock( ); }
		~CGuard( ) { CMutex::instance( ).release( ); }
	};

}


