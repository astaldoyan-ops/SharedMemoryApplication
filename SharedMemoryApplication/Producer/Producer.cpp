// Producer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CProducer.h"

#include <algorithm>
#include <iostream>
#include <string>


int main(int _argc, char **_argv)
{
    std::string segment;

    if( _argc == 2 ) {
        
        std::cout << "Producer is running!" << std::endl;
     
        size_t payloadSize = std::atoi( _argv[1] );

        for(size_t i=0; i<payloadSize; ++i) {
            segment.append(std::to_string(i % 10));
        }

        Framing::Filler filler{
            [&segment, payloadSize]( std::string& _dest, size_t _size )->void {

                static size_t startIndex = 0;

                _dest.clear();

                if(startIndex + 1 > segment.size()) {
                     startIndex = 0;
                }

                auto toEnd = segment.size() - startIndex;

                _dest.append( segment, startIndex, toEnd );
                _dest.append( segment, 0, startIndex );

                ++startIndex;

                std::cout << __FUNCTION__ << " " << _dest << std::endl;
            }
        };

        Producers::CProducer( payloadSize,filler).run( );
    }
    else {
        std::cerr << "Wrong CLI arguments" << std::endl;
    }
}


