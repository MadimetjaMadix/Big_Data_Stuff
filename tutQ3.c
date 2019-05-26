#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

#define num_threads 8

int getElementLocation2D(int index[2], int number_of_columns)
{
	int row    = index[0];
	int column = index[1];

	return ((row*number_of_columns)+column);
}

int* allocateMarix(int number_of_elements)
{	
	int *result = (int*)calloc(number_of_elements,sizeof(int));
	return result;
}

int retrieveElement2D(int* matrix, int index[2], int number_of_columns)
{
	return *(matrix+getElementLocation2D(index, number_of_columns));
}

int* rank2TensorMult(int *matrixA, int dimsA[2], int *matrixB, int dimsB[2], int dimsC[2])
{
	if(dimsA[1] != dimsB[0]) 
	{
		printf("Matrices are not the same size!");
		return (int*)NULL;
	}
	
	int rows_in_A = dimsA[0];
	int cols_in_A = dimsA[1];
	int number_of_elements = dimsC[0]*dimsC[1]; // let N represent k
	int* result = allocateMarix(number_of_elements);
	if(result!=NULL)
	{
		int total_multiplication_operations = cols_in_A*number_of_elements;
		
		omp_set_num_threads(num_threads);
		#pragma omp parallel
		{	
			int chunk_size = round(number_of_elements/(float)omp_get_num_threads())*cols_in_A;
			// printf("Calculated chunk_size = %d \n", chunk_size);
			int rowA = 0;
			int colA = 0;
			
			int rowB = 0;
			int colB = 0;
			
			int rowC = 0;
			int colC = 0;
			/* statically allocate chunks so that each thread works on a certain range of numbers
			   in matrixC. This will avoid any race conditions as we ensure that a thread does all the 
			   multiplication and addition to compute an element in matrix C.
		       Basically, each thread does the entire computation to get an element. They repeat this for the 
			   chunk given to them.
			*/
			#pragma omp for schedule(static, chunk_size) 
			for(int mult_op=0; mult_op<total_multiplication_operations; mult_op++)
			{
				// Get matrix A indexes:
				int tmp = mult_op/(float)(total_multiplication_operations/(float)rows_in_A);
				if(tmp % rows_in_A) rowA = tmp;
				colA = mult_op % cols_in_A;
				
				// Get matrix B indexes:
				rowB = colA;
				colB = ((int)(mult_op/dimsB[0]))%dimsB[1]; // multiplication_index divided by row in matrix B 
				//and mod by column length of matrix B.
	
				
				// Get matrix C indexes:
				rowC = rowA;
				colC = colB;
				/*
				printf("Thread = %d \n", (int)omp_get_thread_num());
				printf("Multiplication index %d\n", mult_op);
				printf("A indexes i = %d, j = %d\n", rowA, colA);
				printf("B indexes i = %d, j = %d\n", rowB, colB);
				printf("C indexes i = %d, j = %d\n", rowC, colC);
				*/
				
				
				int indexA[2] = {rowA, colA}; 
				int indexB[2] = {rowB, colB};
				
				int elementA = retrieveElement2D(matrixA, indexA, cols_in_A);
				int elementB = retrieveElement2D(matrixB, indexB, dimsB[1]);
			
				//printf("A = %d, B = %d\n", elementA, elementB);
				
				int indexC[2] = {rowC, colC};
				int* total = result+getElementLocation2D(indexC, dimsC[1]);
				*total+=(elementA*elementB);

				//printf("========================\n");
			}
		}
		//printf("Done!\n");
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
	int B[32] = {10, 25,  3,  4, 16, 27, 18,  1, 
		     10, 33, 45,  5, 46, 17, 38, 71, 
		     22,  3, 40, 51, 63, 75,  8,  1, 
		      2, 30, 49, 53, 66, 72, 83,  5};
	
	int A[8] = {1, 2, 3, 4, 12, 25, 37, 49};
	
	int rowA = 2, colA = 4;
	int rowB = colA, colB = 8;
	int rowC = rowA, colC = colB;
	
	int dimsA[2] = {rowA,colA}, dimsB[2] = {rowB,colB}, dimsC[2] = {rowC, colC};
	
	printf("Matrix A \n");
	print2DMatrix(A,rowA, colA);
	printf("Matrix B \n");
	print2DMatrix(B,rowB, colB);
	
	printf("\nMatrix C \n");
	int *results = rank2TensorMult(A, dimsA, B, dimsB, dimsC);
	print2DMatrix(results,rowC, colC);
	
	free(results);

	return 0;
}
