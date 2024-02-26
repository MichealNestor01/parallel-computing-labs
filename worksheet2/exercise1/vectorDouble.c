//
// An example of vector addition in MPI-C. Compile with:
//
// mpicc -Wall -o vectorAddition vectorAddition.c
//
// and execute with (on one machine with 4 cores):
//
// mpiexec -n 4 ./vectorAddition
//
// For more instructions see the notes at the end of Lecture 8.
//


//
// Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


//
// Problem size.
//
#define N 100


//
// Main
//
int main( int argc, char **argv )
{
	int i, p;

	// Initialise MPI and find the total number of processes and the rank of 'this' process.
	int rank, numProcs;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &numProcs );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank     );

	// Check that the problem size is a multiple of the number of processes.
	if( N%numProcs )
	{
		if( rank==0 ) printf( "Problem size '%d' is not a multiple of the number of processes '%d.'\n", N, numProcs );
		MPI_Finalize();
		return EXIT_FAILURE;
	}

	// Initialise the full vector a on rank 0 only.
	float *a=NULL;
	if( rank==0 )
	{
		// Allocate memory for the vectors.
		a = (float*) malloc( N*sizeof(float) );
		if( !a )
		{
			printf( "Could not allocate memory for the vectors.\n" );
			MPI_Finalize();
			return EXIT_FAILURE;
		}

		// Put values into vector a only.
		for( i=0; i<N; i++ ) a[i] = i+1;
	}

	// Initialise the local vectors on all processes except rank 0. Note that rank 0 can use the
	// same arrays a for the full vectors, but it helps keep things easier to understand
	// to use new names for the local variables and arrays.
	int localSize = N / numProcs;
	float *local_a=NULL;
	if( rank>0 )
	{
		local_a = (float*) malloc( localSize*sizeof(float) );
	}

	//
	// Step 1. Rank 0 sends segments of the full vector a to their respective processes;
	// those processes must in turn receive the data.
	//
	if( rank==0 )
	{
		for( p=1; p<numProcs; p++ )
		{
			MPI_Send( &a[p*localSize], localSize, MPI_FLOAT, p, 0, MPI_COMM_WORLD );
		}
	}
	else
	{
		MPI_Recv( local_a, localSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
	}

	//
	// Step 2. Each process performs vector double on their sections of a
	//
	if( rank==0 )
		for( i=0; i<localSize; i++ ) a[i] *= 2;
	else
		for( i=0; i<localSize; i++ ) local_a[i] *= 2;

	//
	// Step 3. Recombine the full c array back on rank 0.
	//
	if( rank==0 )
	{
		for( p=1; p<numProcs; p++ )
			MPI_Recv( &a[p*localSize], localSize, MPI_FLOAT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
	}
	else
	{
		MPI_Send( local_a, localSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD );
	}

	// Check the answer on rank 0. Only display the first few array elements if N is large.
	if( rank==0 )
	{
		for( i=0; i<(N>20?20:N); i++ )
			printf( "a[%d]= %g.\n", i, a[i]);
		if( N>20 )
			printf( "(remaining elements not displayed)\n" );
	}

	// Clear up.
	if( rank==0 )
	{
		free( a );
	}
	else
	{
		free( local_a );
	}
	MPI_Finalize();
	return EXIT_SUCCESS;
}
