#include "CProducer.h"

namespace Producers {

	CProducer::CProducer( unsigned long _size, std::function<void( char* _addr, unsigned long _size )> _filler )
	{}

	Applications::CApplication& CProducer::run( ) {
		return *this;
	}

}
