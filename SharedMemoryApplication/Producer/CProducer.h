#pragma once
#include "..\Basics\Basics.h"

#include <chrono>
#include <functional>

namespace Producers {

	class CProducer: Applications::CApplication
	{
	public:
		CProducer( unsigned long _size, std::function<void( char* _addr, unsigned long _size )> _filler );
		Applications::CApplication& run( ) override;

	private:

		struct Header {
			std::chrono::high_resolution_clock m_timestamp;
			unsigned long m_sequenceNumber;
			Algorythms::CRC::CRC8 m_Crc8;
		};

		struct Frame {
		public:
			Frame( unsigned long _payloadSize, std::function<void(char *_addr, unsigned long _size)>& _filler );
			Header* header( ) { return &m_header; }
			char* payload( ) { return m_payload.data(); }
		private:
			std::function<void( char* _addr, unsigned long _size )> m_filler;
			Header m_header;
			std::string m_payload;
		};

		bool createIpc( );

		bool emplacePayload( char * _start );
		bool emplaceHeader( Header * _start );
		std::unique_ptr<Frame> createFrame( );
	};

}

