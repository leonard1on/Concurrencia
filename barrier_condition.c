#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int thread_count;
int counter = 0;
pthread_cond_t cond;
pthread_mutex_t mutex;
void* thread_func(void* rank);

int main(int argc, char const* argv[]) {
  long thread;
  pthread_t* thread_handles;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count * sizeof(pthread_t));

  for (thread = 0; thread < thread_count; thread++) {
    pthread_create(&thread_handles[thread], NULL, thread_func, (void*)thread);
  }

  for (thread = 0; thread < thread_count; thread++) {
    pthread_join(thread_handles[thread], NULL);
  }
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);
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

  pthread_mutex_lock(&mutex);
  counter++;
  if (counter == thread_count) {
    counter = 0;
    pthread_cond_broadcast(&cond);
  } else {
    printf("%ld esta mamado\n", my_rank);
    while (pthread_cond_wait(&cond, &mutex) != 0) {
    }
  }
  printf("%ld esta liberado\n", my_rank);
  pthread_mutex_unlock(&mutex);

  return NULL;
}