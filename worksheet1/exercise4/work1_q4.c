//
// Loop with cyclic dependency in serial; you are asked to make it parallel.
// Compile with:
//
//  gcc -fopenmp -Wall -o work1_q4 work1_q4.c
//


//
// Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


// The problem (i.e. array) size.
#define n 5


// Utility routine to print a vector of given size.
void printVector( int size, float *x )
{
	int i;
	for( i=0; i<size; i++ )
	{
		if( i%20==0 ) printf( "\n" );
		printf( "%.1f\t", x[i] );
	}
	printf( "\n" );
}

void serial() {
	printf("SERIAL\n");
	int i;

	// Allocate memory for the array and initialise it.
	float *a = (float*) malloc( n*sizeof(float) );
	for( i=0; i<n; i++ ) a[i] = i;

	// Display the initial vector.
	printf( "Initial vector a:" );
	printVector( n, a );

	// The cyclic loop.
	float temp = a[n-1];
	for( i=n-1; i>0; i-- ) a[i] = a[i-1];
	a[0] = temp;

	// Display the final vector.
	printf( "Final vector a:" );
	printVector( n, a );

	// Clear up and quit.
	free( a );
}

void parallel() {
	printf("PARALLEL\n");
	int i;

	// Allocate memory for the array and initialise it.
	float *a = (float*) malloc( n*sizeof(float) );
	for( i=0; i<n; i++ ) a[i] = i;

	// Display the initial vector.
	printf( "Initial vector a:" );
	printVector( n, a );

    // Allocate memory for the copy
    float *a_cpy = (float*) malloc(n * sizeof(float));
	// copy contents
	#pragma omp parallel for
	for (i=0; i<n; i++) a_cpy[i] = a[i];
	

	float temp = a[n-1];
	#pragma omp parallel for
	for( i=n-1; i>0; i-- ) a[i] = a_cpy[i-1];
	a[0] = temp;

	// Display the final vector.
	printf( "Final vector a:" );
	printVector( n, a );

	// Clear up and quit.
	free( a );
}

//
// Main
//
int main()
{
	serial();
	parallel();
	return EXIT_SUCCESS;
}



