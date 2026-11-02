#include "controller/GameController.hpp"

#include <exception>
#include <iostream>

int main()
{
    try
    {
        GameController application;
        return application.run();
    }
    catch (const std::exception& error)
    {
        std::cerr << "SnakeArcade could not start: " << error.what() << '\n';
        return 1;
    }
}
