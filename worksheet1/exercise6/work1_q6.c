//
// Simple code for atomics. In fact, it is so simple that there are few benefits to parallelisation,
// but is a useful demonstration of the use of atomics.
//
// Compile with:
// gcc -fopenmp -Wall -o work1_q6.c work1_q6.c
//


//
// Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


//
// Problem size and the divisor.
//
#define N 100000
#define DIV 3


//
// Main
//
int main()
{
	int i;

	// Initialise the array of size n such that a[i]=i
	int *a = (int*) malloc( N*sizeof(int) );
	for( i=0; i<N; i++ ) a[i] = i;

	// Start the timer.
	double startTime = omp_get_wtime();

	// Loop over all values of the array, counting those divisible by some number.
	int count = 0;
	#pragma omp for
	for( i=0; i<N; i++ ) {
		if( a[i]%DIV == 0 )  {
			# pragma omp atomic
			count++;
		}
	}	

	// Output the result and timing.
	double us = omp_get_wtime() - startTime;
	printf( "In the range 0 to %d inclusive, %d numbers are divisible by %d.\n", N-1, count, DIV );
	printf( "Time taken: %g secs.\n", us );

	// Clear up and quit
	free( a );
	return EXIT_SUCCESS;
}
