#include "CConsumer.h"

#include <conio.h>

namespace Consumers {

    void CConsumer::processFrame( ) {

    }

    Applications::CApplication& CConsumer::run( )
    {
        while( !toStop( ) ) {
            processFrame( );
        }
        return *this;
    }

    bool CConsumer::toStop( ) const
    {
        if( _kbhit( ) && ( _getch( ) == Signals::chEscape ) ) return true;
        else return false;
    }

}
