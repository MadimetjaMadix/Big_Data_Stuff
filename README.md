# Big_Data_Stuff

## Question 1

(i.) What is Flynn’s toxonomy of parrallel programs. Briefly explain each toxonomy
### ans:
Flynn’s taxonomy distinguishes multi-processor computer architectures along two independent dimensions of Instruction and Data
	
**Single Instruction, Single Data (SISD)**

- A serial (non-parallel) compute.

- Single instruction: CPU acts on a single stream of instructions during any one clock cycle.

- Single data: only one data stream is being used as input during any one clock cycle.

**Single Instruction, Multiple Data (SIMD)**

- All processing units execute the same instruction at any given clock cycle.

- Multiple data: Each processing unit can operate on a different data element.

**Multiple Instruction, Single Data (MISD)**

- A single data stream is fed into multiple processing units.

- Each processing unit operates on the data independently via independent instruction stream.

**Multiple Instruction, Multiple Data (MIMD)**

- Currently, the most common type of parallel computer.

- Most modern computers fall into this category.
 
- Multiple Instruction: every processor may be executing a different instruction stream.

- Multiple Data: every processor may be working with a different data stream.
	
(ii.) Consider a 4-dimensional hypercube computer architecture of p processors. What is:
(a) the diameter?
(b) the connectivit?,
(c) the bisection width.

### ans:

(a) the diameter?
- Diameter is log(p)

(b) the connectivit?,
- Arc connectivity is log(p)

(c) the bisection width.
- Bisection width is p/2
	
(iii.) Repeat the above for a 3-Dimensional cube of p processors?

-----------------------------------------------------------------------------------------------
## Question 2

(i.) Derive Amdahl’s law.
### ans:
**Amdahl’s derivation** Assumes strong scaling
Assuming a sequential execution time of To. Only Tf execution time can be parallized in Ts. so the fraction of code that can be impoved is gives as P = Tf/Ts.
The improved execution time Ta is gives as Ta = (1-P)To + (P/N) (To)

The overall system speed up(S) is S = To/Ta = 1/((1-P)+P/N)  

**Amdahl’s special case** states that in parallelization, if P is the proportion of a system or program that can be made parallel, and 1-P is the proportion that remains serial, then the maximum speedup that can be achieved using N number of processors is 1/((1-P)+(P/N).

If N tends to infinity then the maximum speedup tends to 1/(1-P)

(ii.) Assuming you can use as many processors as you want. For the type of calculation sketched
in Figure 1, is there a limit to the speed-up you can achieve? What does that mean for
the parallel efficiency of computations using large numbers of processors?
### ans:
using the equaton from (i.), 1/(1-80%) = 5.
Speedup is limited by the total time needed for the sequential (serial) part of the program. For 100% of computing, if we can parallelize 80% of computing and 20% cannot be parallelized, then our maximum speedup is limited to 5x.

-----------------------------------------------------------------------------------------------
## Question 3

a) How will you define a Thread in parallel computing?
### ans:
- A process is an instance of a program that is executing more or less autonomously on a physical processor. Each process has its own private memory (which is which they are used in distributed memory programming).Communication between processes is done through message passing.

- A thread is defined as an independent stream of instructions that can be scheduled to run as such by the operating system.  Each thread is a different stream of control that can execute its instructions independently, allowing a multithreaded process to perform numerous tasks concurrently. Threads are on the same shared memory address (which is why they are used for shared memory programming) or heap so one must care of synchronization in order to avoid race conditions.
### The Values of Using Threads
- Performance gains from multiprocessing hardware (parallelism)
- Increased application throughput
- Increased application responsiveness
- Replacing process-to-process communications
- Efficient use of system resources
- Simplified realtime processing
- Simplified signal handling
- The ability to make use of the inherent concurrency of distributed objects
- There is one binary that runs well on both uniprocessors and multiprocessors
- The ability to create well-structured programs
- There can be a single source for multiple platforms

### What Kind of Programs to Thread?
- Independent task
- Servers
- Repetitive task
- Numerical programs
- where its possible to use divede and conquer strategy


b) Give four characteristic features of a thread.
### ans:
1. Runtime stack
2. Its own resources
3. Changes the occur in a thread are visible to all other threads
4. Reading and writing to the same memory address possible but requires synchronisation to prevent race conditions


c) Write an openMP function:
void matrixMult( . . . )
to compute the matrix multiplication of C = A × B . where A is a 2 × k matrix and B is a
k × 2k using p threads.

### ans:
```
void matrixMult(int *matrixA, int *matrixB, int N)
{
	int number_of_elements = 4*N;  // let n represent k
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
			
			#pragma omp critical
			*total+=(elementA*elementB);
		}
	}
	else	
	{
		printf("Memory allocation failed.");
	}
	return result;
}
```
-----------------------------------------------------------------------------------------------
## Question 4



Assuming we have an A = N × N upper triangular matrix  held by process of rank 0.

We wish to transform it into a lower triangular matrix by N processes.

Write an MPI program to achieve this with N processes.

### ans:
#######
#     #
      #
     #
    #
   #
   
   #

-----------------------------------------------------------------------------------------------
## Question 5


Consider the special case binary search tree in which all keys are stored in a leaf node. The
internal nodes only store keys to direct the search path. Processes traverse the tree to determine if a key exists; insert keys if the key does not exist, and delete keys if a key exists.
Sketch algorithms in pseudo code for openMP processes that:
• perform simple lookup; OpenMP Find key(...);
• or perform an insertion; OpenMP Insert key(...);
You may assume that the tree structure is in shared memory.
### ans:

#######
#     #
      #
     #
    #
   #
   
   #

-----------------------------------------------------------------------------------------------
## Question 6


Write an MPI program that does collective parallel IO by writing, the content of an N × N
matrix A[N][N] in row-major order in a file

### ans:
#######
#     #
      #
     #
    #
   #
   
   #


-----------------------------------------------------------------------------------------------
## Question 7


Write an OpenMP program to compute the value of π, by:
i.) numerical integration of a function


f(x) = Z 1
		0
			4
		  1 + x^2  = π

### ans:
```

...
static long nsteps = 10000000;
double pi = 0.0;
int main()
{
	#pragma omp parallel
	{ /* Start of parallel block */
		int i;
		double x;
		int id = omp_get_thread_num();
		step = 1.0/(double) nsteps ;
		double partialSums = 0.0 ;
		for (i = id, i <= nsteps; i += NumThreads){
			x = (i-0.5) * step;
			partialSums += 4.0 / (1.0 + x * x);
		}
		#pragma omp critical /* Critical Region
		pi += partialSums ;
	} /* End of parallel block */
}

```

ii.) By Monte-Carlo simulation

### ans:
```
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
 
int main(int argc, char* argv[])
{
    int niter = 1000000;            //number of iterations per FOR loop
    double x,y;                     //x,y value for the random coordinate
    int i;                          //loop counter
        int count=0;                //Count holds all the number of how many good coordinates
    double z;                       //Used to check if x^2+y^2<=1
    double pi;                      //holds approx value of pi
        int numthreads = 16;
 
    #pragma omp parallel firstprivate(x, y, z, i) shared(count) num_threads(numthreads)
    {
        srandom((int)time(NULL) ^ omp_get_thread_num());    //Give random() a seed value
        for (i=0; i<niter; ++i)              //main loop
        {
            x = (double)random()/RAND_MAX;      //gets a random x coordinate
            y = (double)random()/RAND_MAX;      //gets a random y coordinate
            z = sqrt((x*x)+(y*y));          //Checks to see if number is inside unit circle
        if (z<=1)
        {
                ++count;            //if it is, consider it a valid random point
        }
    }
        //print the value of each thread/rank
    }
        pi = ((double)count/(double)(niter*numthreads))*4.0;
        printf("Pi: %f\n", pi);
 
    return 0;
}

```

-----------------------------------------------------------------------------------------------
## Question 8


a) The Figure 5 illustrates the MapReduce process to count the number of distinct words in
	a text of three lines:
  
	Deer Bear River
	Car Car River
	Deer Car Bear
  
You may assume that we have 3 nodes that are used for generating the mapping. Each
node is sent a line of text as shown. There are 4 nodes that do the reduction with the final
result sent to an output node. Your goal is to compute the co-occurrences of all pairs of
adjacent words. The Count(A, B) gives the number of occurrences of A and B in sequence.
The pair (A, B) should be considered the same as (B, A) and as such counted together.
Describe the process of using MapReduce technique, with an illustrative diagram similar
to the one shown above, to generate the counts of the pairs of words for the text below.
Assume the same node arrangement is used as in the case of a single word count.
  
	Deer Car River Deer
	Bear Car Car Deer River
	Deer Car Bear

### ans:
#######
#     #
      #
     #
    #
   #
   
   #



b) Big Data is typically characterized by 3Vs. What does this mean?
### ans:

- **Volume** - Data volume is increasing exponentially, From 0.8 zettabytes to 35ZB.

- **Variety** - A single application can be generating/collecting many types of data.

- **Velocity** - Data is being generated fast and need to be processed fast.

c) Briefly describe five technologies appropriate for processing big-data.
### ans:
**Distributed file systems (GFS, HDFS, CEPH, etc.)**

  - MapReduce and other models for distributed programming
  - NoSQL databases
  - Grid & cloud computing
  - Large-scale machine learning
  - High performance computing
  - Parallel File Systems (Lustre, GPFS, BeeGFS, SIONLib, ADIOS, PHDF5, etc.)

d) Give 4 characteristic features of NoSQL databases.
### ans:

  - Not using the relational model (nor the SQL language)
  - Designed to run on large clusters (horizontally scalable)
  - No schema - fields can be freely added to any record
  - Open source

















