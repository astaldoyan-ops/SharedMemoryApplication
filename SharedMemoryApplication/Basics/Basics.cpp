// Basics.cpp : Defines the functions for the static library.
//
#include "Basics.h"


namespace Applications {

}

namespace Framing {


    size_t number( ) {
        static size_t value{ 0 };
        return ++value;
    }


}


