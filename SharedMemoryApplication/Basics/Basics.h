#pragma once

#include <string>
#include <chrono>
#include <functional>

#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>

//#include "MyPosix.h"

namespace Signals {
	const char chEscape{ 0x1B };

    static bool kbhit() {
        struct termios oldt, newt;
        int ch;
        int oldf;

        // Get current terminal settings
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;

        // Disable canonical mode (buffered i/o) and local echo
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        // Make stdin non-blocking
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        // Try to read a character
        ch = getchar();

        // Restore original terminal settings and blocking mode
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        // If a character was successfully read, a key was pressed
        if (ch != EOF) {
            ungetc(ch, stdin); // Put the character back into the stream
            return true;
        }

        return false;
    }

    static char getKey() {
        char product;
        read(STDIN_FILENO, &product, 1);
        return product;
    }
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

    static size_t number( ) {
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
        Frame( size_t _payloadSize )
            : m_payloadSize(_payloadSize)
        {}
        Frame( size_t _payloadSize, Filler& _filler )
            : m_payloadSize(_payloadSize)
            , m_filler(_filler)
        {
            m_filler(m_payload, m_payloadSize);
        }
		Framing::Header* header( ) { return &m_header; }
		std::string& payload( ) { return m_payload; }
		Frame& finish( ) { m_header.finish( m_payload ); return *this; }
	private:
        size_t m_payloadSize;
        Filler m_filler;
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
        CMutex( ) = default;
		pthread_mutex_t m_object;
	};

	static const std::string s_objectName = "Created Boa Dedicated Deviro Shared Memory object";

	class CIpcUnit
	{
	public:
        CIpcUnit( ) {}

        virtual ~CIpcUnit() {
            close(m_ShmFileDescriptor);
            shm_unlink(s_objectName.c_str());
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


