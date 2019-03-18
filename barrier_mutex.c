#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int thread_count;
int counter = 0;

pthread_mutex_t barrier;
void* thread_func(void* rank);

int main(int argc, char const* argv[]) {
  long thread;
  pthread_t* thread_handles;
  pthread_mutex_init(&barrier, NULL);

  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count * sizeof(pthread_t));

  for (thread = 0; thread < thread_count; thread++) {
    pthread_create(&thread_handles[thread], NULL, thread_func, (void*)thread);
  }

  for (thread = 0; thread < thread_count; thread++) {
    pthread_join(thread_handles[thread], NULL);
  }
  pthread_mutex_destroy(&barrier);
  return 0;
}

void* thread_func(void* rank) {
  long my_rank = (long)rank;
  int n = sizeof(array) / sizeof(int) / thread_count;
  int init = my_rank * n;
  int end = (my_rank + 1) * n;
  int partialsum = 0;

  for (int i = init; i < end; i++) {
    partialsum += array[i];
  }

  pthread_mutex_lock(&barrier);
  counter++;
  printf("%ld esta mamado\n", my_rank);

  pthread_mutex_unlock(&barrier);
  while (counter < thread_count) {
  }
  printf("%ld es libre\n", my_rank);

  return NULL;
}