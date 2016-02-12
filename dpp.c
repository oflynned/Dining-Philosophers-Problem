#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_ITERATIONS 1000
#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5
#define LOWER_WAIT 1000
#define UPPER_WAIT 1000000

pthread_t philosopher[NUM_PHILOSOPHERS];
static pthread_mutex_t fork[NUM_FORKS] = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};
pthread_mutex_t iteration_lock = PTHREAD_MUTEX_INITIALIZER;

int iterations = 0;

//timing helpers
long generate_random_wait(long lower, long upper, int p_id);
void wait(long duration);
long get_avg_wait();
long get_peak_wait();
void* generate_state_choice(void* arg);

//statial behaviour cycle
int get_left_fork(int p_id);
int get_right_fork(int p_id);
int acquire_forks(int p_id);
void release_forks(int p_id);
void eat(int p_id);
void think(int p_id);

int main(void)
{
	int f, p, rc;
	for(p = 0; p < NUM_PHILOSOPHERS; p++)
	{
	  rc = pthread_create(&philosopher[p], NULL, generate_state_choice, (void*) p);
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
	
	for(f = 0; f < NUM_FORKS; f++)
	{
    pthread_mutex_destroy(&fork[f]);
	}
}

//timing helpers
long generate_random_wait(long lower, long upper, int p_id)
{
  srand((unsigned)time(NULL));
  long long time = lower + (rand() % (upper - lower + 1)); 
  
  printf("%d is waiting for %ld", p_id, time);
  return time;
}

void wait(long duration)
{
  usleep(duration);
}

long get_avg_wait(){return 0;}
long get_peak_wait(){return 0;}

void* generate_state_choice(void* arg)
{
  srand((unsigned)time(NULL));
  int num = (rand() % 100) + 1;
  int *p_id = (int*) arg;
  if(num < 50)
  {
    think(p_id);
  }
  else
  {
    eat(p_id);
  }
}

//statial behaviour cycle
int get_left_fork(int p_id)
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

int get_right_fork(int p_id)
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
  if(pthread_mutex_trylock(&fork[get_left_fork(p_id)]) &&
      pthread_mutex_trylock(&fork[get_right_fork(p_id)]))
  {
    //acquired -- invoke eating
    pthread_mutex_lock(&fork[get_left_fork(p_id)]);
    pthread_mutex_lock(&fork[get_right_fork(p_id)]);
    return 1;
  }
  else
  {
    //already acquired by another agent -- return to thinking
    return 0;
  }
}

void release_forks(int p_id)
{
    pthread_mutex_unlock(&fork[get_left_fork(p_id)]);
    pthread_mutex_unlock(&fork[get_right_fork(p_id)]);
}

void eat(int p_id)
{
  if(acquire_forks(p_id) == 1)
  {
    printf("\n%d is eating\n", p_id);
    printf("forks %d and %d are locked!", get_left_fork(p_id), get_right_fork(p_id));
    wait(generate_random_wait(LOWER_WAIT, UPPER_WAIT, p_id));
    release_forks(p_id);
    printf("forks %d and %d are unlocked!", get_left_fork(p_id), get_right_fork(p_id));
    
    pthread_mutex_lock(&iteration_lock);
    iterations++;
    printf("iteration #%d", iterations);
    pthread_mutex_unlock(&iteration_lock);
    think(p_id);
  }
  else
  {
    think(p_id);
  }
}

void think(int p_id)
{
  printf("\n%d is thinking\n", p_id);
  wait(generate_random_wait(LOWER_WAIT, UPPER_WAIT, p_id));
  eat(p_id);
}

