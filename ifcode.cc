#include <iostream>
extern "C" char *test_if( )
{
	std::cout << "Hello World\n";
	return "Hola Orb!\n";
}
