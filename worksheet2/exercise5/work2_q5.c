//
// Approximate ln(2) by summing on separate processes, each with the same stride.
//
// Compile with (note you may need to add '-lm' to link to the maths library):
//
// mpicc -Wall -lm -o work2_q6 work2_q6.c
//
// Execute with two processes:
//
// mpiexec -n 2 ./work2_q6 10000
//
// Where the final value is the number of terms in the summation.
//


//
// Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"


//
// Main.
//
int main(int argc, char** argv)
{
	// Variables.
	int i, N, numProcs, rank;
	float partialSum = 0.0, finalSum;

	// Start up.
	MPI_Init(&argc, &argv);
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &numProcs );

	// Get the number of terms in total.
	if( argc != 2 )
	{
		if( rank==0 ) printf( "Need sole argument = no. of terms\n" );
		MPI_Finalize();
		return -1;
	}
	N = atoi(argv[1]);
	if( N<=0 )
	{
		if( rank==0 ) printf( "Need positive no. of terms (or could not parse argument as an integer)\n" );
		MPI_Finalize();
		return -1;
	}

	// Each process calculates terms along the series with a stride of numProcs.
	for( i = rank; i<N; i += numProcs )
		partialSum += ( i%2 ? -1.0 : 1.0 ) / ( i+1.0 );

	// Uncomment to display each partial sum (note the order in which these messages
	// appear cannot be easily controlled and is essentially random).
//	printf( "Partial sum for rank %i:\t%g\n", rank, partialSum );

	// Use MPI_Reduce() to get the total sum.
	MPI_Reduce( &partialSum, &finalSum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD );

	// Display final sum and compare to known answer.
	if( rank==0 ) printf("An estimate of ln(2) is %g (should be %g)\n",finalSum,log(2.0));

	// Clear up.
	MPI_Finalize();
	return EXIT_SUCCESS;
}
