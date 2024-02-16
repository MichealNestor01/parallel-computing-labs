//
// Example of how using locks to nest critical regions can result in deadlock.
//
// Compile with:
//
// gcc -Wall -fopenmp -o nestedCriticalRegions nestedCriticalRegions.c
//


//
// Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


//
// The global lock, and a routine that uses it.
//
omp_lock_t lock;

void function()
{
	// A critical region using 'lock.'
	omp_set_lock( &lock );
	printf( "Thread inside inner critical region.\n" );
	omp_unset_lock( &lock );
}


//
// Main
//
int main()
{
	int i;
	omp_init_lock( &lock );

	// Some arbitrary loop.
	#pragma omp parallel for
	for( i=0; i<100; i++ )
	{
		// Start of the outer critical region.
		omp_set_lock( &lock );
		printf( "Thread inside outer critical region.\n" );

		// Call some function, unaware that it also uses 'lock.'
		function();

		// End of the outer critical region.
		omp_unset_lock( &lock );
	}

	// Clear up and quit.
	omp_destroy_lock( &lock );
	return EXIT_SUCCESS;
}
