#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int thread_count;
void* thread_func(void* rank);
void fill();
void print(int* vec);
int vector[] = {2, 3, 5, 1, 2, 7};
int* vector2;
int matrix[6][6];
int main(int argc, char const* argv[]) {
  long thread;
  pthread_t* thread_handles;
  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count * sizeof(pthread_t));
  vector2 = malloc(sizeof(vector));
  fill();
  for (thread = 0; thread < thread_count; thread++) {
    pthread_create(&thread_handles[thread], NULL, thread_func, (void*)thread);
  }

  for (thread = 0; thread < thread_count; thread++) {
    pthread_join(thread_handles[thread], NULL);
  }

  print(vector2);

  return 0;
}

void* thread_func(void* rank) {
  long my_rank = (long)rank;
  int n = sizeof(vector) / sizeof(int) / thread_count;
  int first_row = my_rank * n;
  int last_row = (my_rank + 1) * n;

  for (int i = first_row; i < last_row; i++) {
    for (int j = 0; j < sizeof(vector) / sizeof(int); j++) {
      vector2[i] += matrix[i][j] * vector[j];
    }
  }
  return NULL;
}

void print(int* vec) {
  for (int i = 0; i < sizeof(vec) / sizeof(int); i++) {
    printf("[%d]", vec[i]);
  }
}

void fill() {
  for (int i = 0; i < sizeof(vector) / sizeof(int); i++) {
    for (int j = 0; j < sizeof(vector) / sizeof(int); j++) {
      if (i % 2 == 0) {
        matrix[i][j] = 2;
      } else if (j % 2 == 0) {
        matrix[i][j] = 3;
      } else {
        matrix[i][j] = 1;
      }
    }
  }
}
