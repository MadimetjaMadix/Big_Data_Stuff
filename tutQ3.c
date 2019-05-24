#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define num_threads 8

int getElementLocation2D(int index[2], int N)
{
	int row    = index[0];
	int column = index[1];

	return ((row*N)+column);
}

int* allocateMarix(int number_of_elements)
{	
	int *result = (int*)calloc(number_of_elements,sizeof(int));
	return result;
}

int retrieveElement2D(int* matrix, int index[2], int N)
{
	return *(matrix+getElementLocation2D(index, N));
}

int* rank2TensorMult(int *matrixA, int *matrixB, int N)
{
	int number_of_elements = 4*N; // let N represent k
	int* result = allocateMarix(number_of_elements);
	if(result!=NULL)
	{
		int total_multiplication_operations = N*number_of_elements;
		
		#pragma omp parallel for 
		for(int m=0; m<total_multiplication_operations; m++)
		{
			int i = m/(2*N);
			int j = m%(2*N);
			int k = m/(2*N*N);
			
			//printf("i = %d, j = %d, k = %d\n", i, j, k);
			
			int indexA[2] = {i, k}; 
			int indexB[2] = {i, j};
			
			int elementA = matrixA[i];
			int elementB = matrixB[((i%2)*(2*N))+j];
			
			//printf("A = %d, B = %d\n", elementA, elementB);
			
			int indexC[2] = {k, j};
			int* total = result+getElementLocation2D(indexC, 2*N);
			*total+=(elementA*elementB);
		}
	}
	else	
	{
		printf("Memory allocation failed.");
	}
	return result;
}

void print2DMatrix(int *matrix, int N, int M)
{
	if(matrix != NULL)
	{	
		printf("\n");
		for(int i=0; i< N*M; ++i)
		{
			if( i%M == 0) printf("\n");
	
			printf("%4d ", *matrix);
			++matrix;
		}
		printf("\n\n");

	}else printf(" The Matrix is empty");
	
}

int main()
{
	int A[32] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
	int B[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	
	int k = 4;
	print2DMatrix(A,2, k);
	print2DMatrix(B,k, 2*k);
	
	int *results = rank2TensorMult(A, B,k);
	print2DMatrix(results,2, 2*k);



	return 0;
}

