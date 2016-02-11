#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5
#define ONE_SEC 1000

pthread_t philosopher[NUM_PHILOSOPHERS];
pthread_mutex_lock fork[NUM_FORKS];

//statial behaviour cycle
void* think(void* arg){}
void* eat(void*arg){}
void* acquire_left_fork(void* arg){}
void* acquire_right_fork(void* arg){}
void* release_forks(void* arg){}

//timing helpers
void wait(){}
long get_avg_wait(){return 0;}
long get_peak_wait(){return 0;}

int main(void)
{
	int j, rc;
	for(j = 0; j < NUM_PHILOSOPHERS; j++)
	{
		rc = pthread_create(&philosopher[j], NULL, NULL, (void*) j);
	}
}
