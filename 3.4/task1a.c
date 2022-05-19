#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	const char *pathname = "task1a.c";

	key_t key = ftok(pathname, 0);
	if (key < 0) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	struct stat st;
	if (stat(pathname, &st) < 0) {
		perror("stat");
		exit(EXIT_FAILURE);
	}

	size_t filesize = st.st_size;

	int shmid = shmget(key, filesize, IPC_CREAT | IPC_EXCL | 0666);
	if (shmid < 0) {
		if (errno != EEXIST) {
			perror("shmget");
			exit(EXIT_FAILURE);
		}

		shmid = shmget(key, filesize, 0);
		if (shmid < 0) {
			perror("shmget");
			exit(EXIT_FAILURE);
		}
	}

	char *shm = shmat(shmid, NULL, 0);
	if (shm == (char *) -1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	int fd = open(pathname, O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	char *file = mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd, 0);
	if (file == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	close(fd);

	memcpy(shm, file, filesize);

	munmap(file, filesize);
	shmdt(shm);

	return 0;
}
