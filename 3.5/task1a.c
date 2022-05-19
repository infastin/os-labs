#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main()
{
	char pathname[] = "task1a.c";

	key_t key = ftok(pathname, 0);
	if (key < 0) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	int semid = semget(key, 1, IPC_CREAT | 0666);
	if (semid < 0) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

	struct sembuf mybuf;

	mybuf.sem_op = -5;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;

	if (semop(semid, &mybuf, 1) < 0) {
		perror("semop");
		exit(EXIT_FAILURE);
	}

	printf("Condition is present\n");

	return 0;
}
