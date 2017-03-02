// main program for the Document Analysis Project

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "analysis.h"
#include "unit_tests.h"


// Run the unit tests or as a "normal program".
// You can run this as a "normal program" if you want for a simple test of the sort_two_positions function.
int main( int argc, char *argv[] )
{

	// Execute unit tests if program name contains "test".
	if ( strstr( argv[0], "test" ) )
		return test() ;

	// You can write a user interface here if you want but it will not be graded.
	
	return 0 ;
}
