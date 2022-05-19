#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

char *names[] = {
	"Adam",
	"Adrian",
	"Anna",
	"Charlotte",
	"Frédérique",
	"Ilaria",
	"Jakub",
	"Jennyfer",
	"Julia",
	"Justin",
	"Mario",
	"Miriam",
	"Mohamed",
	"Nourimane",
	"Owen",
	"Peter",
	"Petra",
	"Rachid",
	"Rebecca",
	"Sarah",
	"Thibault",
	"Wolfgang",
};

int main(int argc, char *argv[])
{
	srand(time(0));

	key_t key = ftok("task3.c", 0);
	if (key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	int fd[2];
	if (pipe(fd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	int semid = semget(key, 1, IPC_CREAT | 0666);
	if (semid < 0) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

	switch (fork()) {
	case 0: {
		close(fd[1]);

		struct sembuf sem = {
			.sem_flg = 0,
			.sem_num = 0,
			.sem_op = -1
		};

		for (int i = 0; i < 5; ++i) {
			if (semop(semid, &sem, 1) < 0) {
				perror("semop child");
				exit(EXIT_FAILURE);
			}

			char buf[32];
			int size = read(fd[0], buf, 32);
			if (size != 32) {
				perror("read");
				exit(EXIT_FAILURE);
			}

			printf("%s\n", buf);
		}

		close(fd[0]);
		semctl(semid, 0, IPC_RMID);
	} break;
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
	default: {
		close(fd[0]);

		struct sembuf sem = {
			.sem_flg = 0,
			.sem_num = 0,
			.sem_op = 1
		};

		for (int i = 0; i < 5; ++i) {
			char buf[32];
			char *name = names[rand() % 22];
			snprintf(buf, 32, "Hello, %s!", name);

			int size = write(fd[1], buf, 32);
			if (size != 32) {
				perror("write");
				exit(EXIT_FAILURE);
			}

			if (semop(semid, &sem, 1) < 0) {
				perror("semop parent");
				exit(EXIT_FAILURE);
			}
		}

		printf("Parent exit\n");

		close(fd[1]);
	} break;
	}

	return 0;
}
