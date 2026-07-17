// Producer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CProducer.h"

#include < algorithm >
#include <iostream>

static std::string segment{"Constant work and lack of entertainment made Jack a boring guy."};

int main(int _argc, char **_argv)
{
    if( _argc == 1 ) {
        
        std::cout << "Producer is running!" << std::endl;
     
        unsigned long payloadSize = std::atoi( _argv[1] );
        Producers::CProducer( payloadSize,
            [payloadSize] ( char* _addr, unsigned long _size )->void {
                long runPayloadSize = payloadSize;
                auto runAddr = _addr;
                while( runPayloadSize > 0 ) {
                    auto portionSize = std::min( payloadSize, static_cast<unsigned long>( segment.length( ) ) );
                    memcpy( runAddr, segment.c_str( ), portionSize );
                    runPayloadSize -= portionSize;
                    runAddr += portionSize;
                }
            }
        ).run( );
    }
    else {
        std::cerr << "Wrong CLI arguments" << std::endl;
    }
}


