#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int is_ore_num(int x)
{
	int count = 2;
	long double one = 1.0;
    long double inv_sum = one / (long double)x + one;
    long double epsilon = 0.0000001;

    long double fraction;
    int d2;
    // printf("X: %d\n", x);
    // printf("Inv sum: %Lf\n", inv_sum);


    // initial calc of inv sum
    int xsqfloor = floor(sqrt(x));
    // printf("xsqfloor: %d\n", xsqfloor);
    
    for (int d = 2; d < xsqfloor; d++) 
    {
    	if (x%d == 0)
    	{
    		count += 2;
    		fraction = one / (long double) d;
	    	inv_sum += fraction;

	    	// printf("d=%d\n", d);
    		// printf("Fraction: %Lf\n", fraction);

	    	d2 = x / d;
	    	fraction = one / (long double)d2;
	    	inv_sum += fraction;
    		
    		// printf("d2=%d\n", d2);
    		// printf("Fraction: %Lf\n", fraction);
   
    	}
    }
    if (x == xsqfloor * xsqfloor)
    {
    	count++;
    	fraction = one / (long double) xsqfloor;
    	inv_sum += fraction;
    }
    else if (x % xsqfloor == 0)
    {
    	count += 2;
    	fraction = one / (long double) xsqfloor;
    	inv_sum += fraction;
    	d2 = x / xsqfloor;
    	fraction = one / (long double)d2;
    	inv_sum += fraction;
    }

    // printf("Inv sum: %Lf\n", inv_sum);
    // printf("count: %d\n", count);
    
    long double harmonic_mean;
    harmonic_mean = (long double)count / inv_sum;
    // printf("Harmonic mean: %Lf\n\n", harmonic_mean);
    
    if (fabs(harmonic_mean - ceil(harmonic_mean)) <= epsilon)
    {
    	return 1;
    }
    return 0;
}

void main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("Incorrect number of parameters! Usage: ./ore_numbers [a] [b] [nr_threads]\n");
		exit(1);
	}
	
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int nr_threads = atoi(argv[3]);

	int i;
	omp_set_num_threads(nr_threads);

	double start = omp_get_wtime();
	
	#pragma omp parallel private (i) 
	{
		int id;
 		id = omp_get_thread_num();

		for (i = a + id; i < b; i+=nr_threads)
		{
			if(is_ore_num(i) == 1)
			{
				printf("[TH%d] %d\n", id, i);
			}
			// printf("[TH%d] %d\n", id, i);
		}
	}
	double stop = omp_get_wtime();
	double time = stop - start;

	printf("Execution time: %lf\n", time);
}