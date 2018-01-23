#include "Utils/DLHandler.hpp"
#include "Utils/PseudoRandom.hpp"
#include <cstdio>


int main(int argc, char **argv)
{
    typedef int (*MainFcn) (int, char **);

    GP::RandomSeed();

    if(argc < 2)
    {
	printf("usage: Runner <program_name> [list_of_program_args]\n");
    }
    else
    {
	MainFcn fcn = (MainFcn) GP::DLHandler::GetSymbol(argv[1]);
	if(fcn)
	{
	    printf("Executing program <%s>\n", argv[1]);
	    return fcn(argc - 1, &(argv[1]));
	}
	else
	    printf("Program <%s> not found\n", argv[1]);
    }    

    return 0;
}
