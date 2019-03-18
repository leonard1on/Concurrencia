#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int thread_count;
double sum = 0.0;
pthread_mutex_t mutex;
void* thread_func(void* rank);

int main(int argc, char const* argv[]) {
  long thread;
  pthread_t* thread_handles;
  pthread_mutex_init(&mutex, NULL);
  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count * sizeof(pthread_t));
  for (thread = 0; thread < thread_count; thread++) {
    pthread_create(&thread_handles[thread], NULL, thread_func, (void*)thread);
  }

  for (thread = 0; thread < thread_count; thread++) {
    pthread_join(thread_handles[thread], NULL);
  }
  pthread_mutex_destroy(&mutex);
  sum *= 4;
  printf("Pi es: %f", sum);
  return 0;
}

void* thread_func(void* rank) {
  long my_rank = (long)rank;
  int n = 10000 / thread_count;
  int init = my_rank * n;
  int end = (my_rank + 1) * n;
  double factor;
  if (init % 2 == 0) {
    factor = 1;
  } else {
    factor = -1;
  }

  double partialsum = 0.0;
  for (int i = init; i < end; i++) {
    partialsum += factor / (2 * i + 1);
    factor *= -1;
  }

  pthread_mutex_lock(&mutex);
  sum += partialsum;
  pthread_mutex_unlock(&mutex);

  return NULL;
}