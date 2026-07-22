#include "CProducer.h"

#include <conio.h>

namespace Producers {

	

	CProducer::CProducer( unsigned long _size, Filler& _filler ): m_payloadSize(_size), m_filler(_filler)
	{}

	Applications::CApplication& CProducer::run( ) {

		while( !toStop( ) ) {
			processFrame( );
		}

		return *this;
	}

	bool CProducer::createIpc( )
	{
		// To Do
		return true;
	}

	void CProducer::processFrame( ) {
		// To Do
	}

	bool CProducer::toStop( ) const
	{
		if( _kbhit( ) && ( _getch( ) == Signals::chEscape ) ) return true;
		else return false;
	}

}
