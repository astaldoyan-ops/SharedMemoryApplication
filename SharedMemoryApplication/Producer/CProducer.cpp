#include "CProducer.h"

namespace Producers {

	CProducer::CProducer( unsigned long _size, Filler& _filler )
	{}

	Applications::CApplication& CProducer::run( ) {
		return *this;
	}

}
