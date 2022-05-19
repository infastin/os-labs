#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

int main()
{
	char pathname[] = "task2a.c";

	key_t key = ftok(pathname, 0);
	if (key < 0) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if (semid < 0) {
		if (errno != EEXIST) {
			perror("semget");
			exit(EXIT_FAILURE);
		}

		semid = semget(key, 1, 0);
		if (semid < 0) {
			perror("semget");
			exit(EXIT_FAILURE);
		}
	} else {
		if (semctl(semid, 0, SETVAL, 1) < 0) {
			perror("semctl setval");
			exit(EXIT_FAILURE);
		}
	}

	int new = 1;

	int shmid = shmget(key, 3 * sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
	if (shmid < 0) {
		if (errno != EEXIST) {
			perror("shmget");
			exit(EXIT_FAILURE);
		}

		shmid = shmget(key, 3 * sizeof(int), 0);
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

	/* Выполняем операцию down */
	struct sembuf mybuf = {
		.sem_flg = 0,
		.sem_num = 0,
		.sem_op = -1
	};

	if (semop(semid, &mybuf, 1) < 0) {
		perror("semop");
		exit(EXIT_FAILURE);
	}

	if (new) {
		shm[0] = 1;
		shm[1] = 0;
		shm[2] = 1;
	} else {
		shm[0] += 1;
		shm[2] += 1;

		for (long i = 0; i < 1000000000L; i++)
			continue;
	}

	printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n", shm[0], shm[1], shm[2]);

	shmdt(shm);

	/* Выполняем операцию up */
	mybuf.sem_op = 1;
	semop(semid, &mybuf, 1);

	return 0;
}
