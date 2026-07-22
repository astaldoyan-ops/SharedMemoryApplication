#pragma once
#include "..\Basics\Basics.h"


#include <functional>

namespace Producers {

	using Filler = std::function<void( std::string& dest, unsigned long _size )>;

	class CProducer: public Applications::CApplication
	{
		

	public:
		CProducer( unsigned long _size, Filler& _filler );
		Applications::CApplication& run( ) override;

	private:

		struct Frame {
		public:
			
			Frame( unsigned long _payloadSize, Filler& _filler );
			Framing::Header* header( ) { return &m_header; }
			const std::string& payload( ) const { return m_payload; }
		private:
			Filler& m_filler;
			Framing::Header m_header;
			std::string m_payload;
		};

		unsigned long m_payloadSize;
		Filler m_filler;

		bool createIpc( );

		bool emplacePayload( char * _start );
		bool emplaceHeader( Framing::Header * _location );
		std::unique_ptr<Frame> createFrame( );

		void processFrame( );

		bool toStop( ) const;
	};

}

