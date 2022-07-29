#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define Expected 1.202056903159594

int main(int argc, char* argv[])
{
	int myid, numprocs;
	MPI_Status status;
	long long int n;
	long long int myFirstN;
	long long int myLastN;
	long double sum = 0.0, mySum = 0.0;
	int remainder;
	long double p3;
	long double i;
	double myStart, myFinish, myElapsed, elapsed;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if (myid == 0) {
		printf("Enter value of n: ");
		scanf("%lld", &n);
		myStart = MPI_Wtime();
	}

	//MPI_Barrier(MPI_COMM_WORLD);
	

	MPI_Bcast(&n, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

	remainder = n % numprocs;
	if (myid < remainder) {
		myFirstN = myid * (n / numprocs + 1) + 1;
		myLastN = myFirstN + (n / numprocs) + 1;
	}
	else {
		myFirstN = remainder * (n / numprocs + 1) + (myid - remainder) * (n / numprocs) + 1;
		myLastN = myFirstN + n / numprocs;
	}

	for (i = myFirstN; i < myLastN; i++) {
		p3 = i * i * i;
		mySum += 1.0 / p3;
	}
	//printf("id: %d, first: %d, last: %d sum: %.15llf\n", myid, myFirstN, myLastN, mySum);

	MPI_Reduce(&mySum, &sum, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	//MPI_Reduce(&myElapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (myid == 0) {
		myFinish = MPI_Wtime();
		myElapsed = myFinish - myStart;
		printf("Summution Result: %.15llf\n", sum);
		sum = Expected - sum;
		
		printf("Error: %.15llf\n", sum);
		printf("Elapsed = %.6f\n", myElapsed);
	}

	MPI_Finalize();
	return 0;
}

