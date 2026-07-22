// Producer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CProducer.h"

#include <algorithm>
#include <iostream>

static std::string segment{"Constant work and lack of entertainment made Jack a boring guy."};

int main(int _argc, char **_argv)
{
    if( _argc == 2 ) {
        
        std::cout << "Producer is running!" << std::endl;
     
        unsigned long payloadSize = std::atoi( _argv[1] );

        Producers::Filler filler{
            [payloadSize]( std::string& _dest, unsigned long _size )->void {

                auto toWriteSize = std::min( payloadSize, static_cast<unsigned long>( segment.length( ) ) );
                auto rest = payloadSize;

                while( rest > 0 ) {
                    _dest.append( segment, toWriteSize );
                    rest -= toWriteSize;
                };
            }
        };

        Producers::CProducer( payloadSize,filler).run( );
    }
    else {
        std::cerr << "Wrong CLI arguments" << std::endl;
    }
}


