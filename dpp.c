#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_ITERATIONS 1000
#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5
#define LOWER_WAIT 1000
#define UPPER_WAIT 1000000

pthread_t philosopher[NUM_PHILOSOPHERS];
pthread_mutex_lock fork[NUM_FORKS];
pthread_mutex_lock timings = PTHREAD_MUTEX_INITIALIZER;

//timing helpers
long generate_random_wait(long lower, long upper, int p_id)
{
  srand((unsigned)time(p_id));
  return lower + (rand() % (upper - lower + 1)); 
}

void wait(long duration)
{
  usleep(duration);
}

long get_avg_wait(){return 0;}
long get_peak_wait(){return 0;}

//statial behaviour cycle
void think(int p_id)
{
  wait(generate_random_wait(LOWER_WAIT, UPPER_WAIT, p_id));
}

int get_left_fork_id(int p_id)
{ 
  if(p_id == 0)
    return 4;
  else if(p_id == 1)
    return 0;
  else if(p_id == 2)
    return 1;
  else if(p_id == 3)
    return 2;
  else if(p_id == 4)
    return 3;
}

int get_right_fork_id(int p_id)
{
  if(p_id == 0)
    return 1;
  else if(p_id == 1)
    return 2;
  else if(p_id == 2)
    return 3;
  else if(p_id == 3)
    return 4;
  else if(p_id == 4)
    return 0;
}

int acquire_forks(int p_id)
{
  if(pthread_mutex_trylock(&fork[get_left_fork_id(p_id)) &&
      pthread_mutex_trylock(&fork[get_right_fork_id(p_id)))
  {
    //acquired -- invoke eating
    return 1;
  }
  else
  {
    //already acquired by another agent -- return to thinking
    return 0;
  }
}

void eat(int p_id)
{
  if(acquire_forks(p_id) == 1)
  {
    pthread_mutex_lock(&fork[get_left_fork(p_id)]);
    pthread_mutex_lock(&fork[get_right_fork(p_id)]);
    wait(generate_random_wait(LOWER_WAIT, UPPER_WAIT, p_id));
    release_forks(p_id);
    think();
  }
  else
  {
    think();
  }
}

void* release_forks(int p_id)
{
    pthread_mutex_unlock(&fork[get_left_fork(p_id)]);
    pthread_mutex_unlock(&fork[get_right_fork(p_id)]);
}

void* generate_state_choice(void* arg)
{
  srand((unsigned)time(NULL));
  int num = rand() % 100 + 1);
  int p_id = (int*) arg;
  if(num < 50)
  {
    think();
  }
  else
  {
    eat();
  }
}

int main(void)
{
	int f, p, rc;
	for(f = 0; f < NUM_FORKS; f++)
	{
	  fork[f] = PTHREAD_MUTEX_INITIALIZER;
	}
	
	for(p = 0; p < NUM_PHILOSOPHERS; p++)
	{
	  rc = pthread_create(&philosopher[p], NULL, think, (void*) p);
	  if(rc)
	  {
	    printf("\ncreate error\n");
	    return -1;
	  }
	}
	
	for(p = 0; p < NUM_PHILOSOPHERS; p++)
	{
	  rc = pthread_join(philosopher[p], NULL);
	}
	
	while(iterations < NUM_ITERATIONS)
	{
	  //cycle through states and checking
	}
	
	for(f = 0; f < NUM_FORKS; f++)
	{
    pthread_mutex_destroy(&fork[f]);
	}
}
