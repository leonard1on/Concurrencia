#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int thread_count;
int counter = 0;
sem_t barrier;
sem_t count;
void* thread_func(void* rank);

int main(int argc, char const* argv[]) {
  long thread;
  pthread_t* thread_handles;
  sem_init(&barrier, 0, 0);
  sem_init(&count, 0, 1);

  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count * sizeof(pthread_t));

  for (thread = 0; thread < thread_count; thread++) {
    pthread_create(&thread_handles[thread], NULL, thread_func, (void*)thread);
  }

  for (thread = 0; thread < thread_count; thread++) {
    pthread_join(thread_handles[thread], NULL);
  }
  sem_destroy(&count);
  sem_destroy(&barrier);
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

  sem_wait(&count);
  printf("%ld paso count\n", my_rank);
  if (counter == thread_count - 1) {
    counter = 0;
    sem_post(&count);
    for (int i = 0; i < thread_count - 1; i++) {
      printf("%ld libero 1 barrier\n", my_rank);
      sem_post(&barrier);
    }
  } else {
    counter++;
    sem_post(&count);
    printf("%ld se mamo en barrier\n", my_rank);
    sem_wait(&barrier);
  }
  printf("%ld paso barrier\n", my_rank);

  return NULL;
}