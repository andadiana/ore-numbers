#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define BILLION  1000000000L;

typedef struct arg_struct {
    int a;
    int b;
    int th_index;
    int step;
}arg_struct;


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

void *tfunc(void* arg)
{
	arg_struct* args = (arg_struct*) arg;
	int a = args->a;
	int b = args->b;
	int th_index = args->th_index;
	int step = args->step;

	printf("Thread th%d started\n", th_index);
	printf("Thread th%d computing from %d to %d with a step of %d\n", th_index, a, b, step);

	for (int i = a; i < b; i+=step)
	{
		if(is_ore_num(i) == 1)
		{
			printf("[TH%d] %d\n", th_index, i);
		}
		// printf("\n");
	}

}


int is_perfect(int n)
{
	int sum = 0;
	for (int i = 1; i <= n/2; i++)
	{
		if (n % i == 0)
		{
			sum += i;
		}
	}
	if (sum == n)
		return 1;
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

	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * nr_threads);
	arg_struct** th_args = (arg_struct**)malloc(sizeof(arg_struct*) * nr_threads);
	for (int i = 0; i < nr_threads; i++)
	{
		th_args[i] = (arg_struct*)malloc(sizeof(arg_struct) * nr_threads);
		th_args[i]->a = a + i;
		th_args[i]->b = b;
		th_args[i]->th_index = i;
		th_args[i]->step = nr_threads;
	}

	struct timespec requestStart, requestEnd;
	clock_gettime(CLOCK_REALTIME, &requestStart);

	for (int i = 0; i < nr_threads; i++)
	{
		pthread_create(&threads[i],NULL,tfunc, th_args[i]);
	}

	for (int i = 0; i < nr_threads; i++)
	{
		pthread_join(threads[i], NULL);
	}

	free(th_args);
	free(threads);	

	clock_gettime(CLOCK_REALTIME, &requestEnd);
	double accum = ( requestEnd.tv_sec - requestStart.tv_sec ) + ( requestEnd.tv_nsec - requestStart.tv_nsec ) / (float)BILLION;
	printf( "Execution time: %lf\n", accum );

}
	