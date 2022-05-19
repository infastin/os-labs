#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main()
{
	char pathname[] = "task3a.c";

	key_t key = ftok(pathname, 0);
	if (key < 0) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	int new = 1;

	int shmid = shmget(key, 6 * sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
	if (shmid < 0) {
		if (errno != EEXIST) {
			perror("shmget");
			exit(EXIT_FAILURE);
		}

		shmid = shmget(key, 6 * sizeof(int), 0);
		if (shmid < 0) {
			perror("shmget");
			exit(EXIT_FAILURE);
		}

		new = 0;
	}

	int *shm = shmat(shmid, NULL, 0);
	if (shm == (int *) -1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	if (new) {
		/* Устанавливаем флаги готовности */
		shm[0] = 0;
		shm[1] = 1;

		/* Устанавливаем номер очередности */
		shm[2] = 0;

		shm[3] = 0;
		shm[4] = 1;
		shm[5] = 1;
	} else {
		/* Устанавливаем флаг готовности */
		shm[1] = 1;

		/* Устанавливаем номер очередности */
		shm[2] = 0;

		while (shm[0] && shm[2] == 0)
			continue;

		shm[4] += 1;
		shm[5] += 1;

		for (long i = 0; i < 1000000000L; i++)
			continue;
	}

	printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n", shm[3], shm[4], shm[5]);

	/* Сбрасываем флаг готовности */
	shm[1] = 0;

	shmdt(shm);

	return 0;
}
