// Producer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CProducer.h"

#include <algorithm>
#include <iostream>

static const std::string segment{"Constant work and lack of entertainment made Jack a boring guy."};

int main(int _argc, char **_argv)
{
    if( _argc == 2 ) {
        
        std::cout << "Producer is running!" << std::endl;
     
        size_t payloadSize = std::atoi( _argv[1] );

        Framing::Filler filler{
            [payloadSize]( std::string& _dest, size_t _size )->void {

                _dest.clear();

                auto toWriteSize = std::min( payloadSize, static_cast<size_t>( segment.length( ) ) );
                auto rest = payloadSize;

                while( static_cast<int>(rest) > 0 ) {
                    _dest.append( segment, 0, toWriteSize );
                    rest -= toWriteSize;
                };

                std::cout << __FUNCTION__ << " " << _dest << std::endl;
            }
        };

        Producers::CProducer( payloadSize,filler).run( );
    }
    else {
        std::cerr << "Wrong CLI arguments" << std::endl;
    }
}


