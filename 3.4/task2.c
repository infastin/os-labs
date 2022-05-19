#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int a = 0;

void *mythread(void *dummy)
{
	pthread_t my_tid;

	my_tid = pthread_self();
	a++;

	printf("Thread %lu, Calculation result = %d\n", my_tid, a);

	return NULL;
}

#define N_THREADS 3

int main(int argc, char *argv[])
{
	pthread_t tids[N_THREADS];

	for (int i = 1; i < N_THREADS; ++i) {
		int result = pthread_create(&tids[i], NULL, mythread, NULL);
		if (result != 0) {
			printf("pthread_create: %s\n", strerror(result));
			exit(EXIT_FAILURE);
		}

		printf("Thread created, tid = %lu\n", tids[i]);
	}

	tids[0] = pthread_self();
	a++;

	printf("Main thread %lu, Calculation result = %d\n", tids[0], a);

	for (int i = 1; i < N_THREADS; ++i) {
		pthread_join(tids[i], NULL);
	}

	return 0;
}
