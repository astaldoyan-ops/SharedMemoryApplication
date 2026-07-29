// Consumer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CConsumer.h"

#include <iostream>

int main()
{
    std::cout << "Consumer is running!" << std::endl;

    Consumers::CConsumer().run();
}

