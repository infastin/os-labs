#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

struct A {
	double f;
	double f2;
};

int main(int argc, char *argv[])
{
	int fd = open("mapped.dat", O_RDWR | O_CREAT, 0666);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	int len = 100000;
	int len_bytes = len * sizeof(struct A);

	ftruncate(fd, len_bytes);

	struct A *ptr = mmap(NULL, len_bytes, PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	close(fd);

	struct A *tmpptr = ptr;
	for (int i = 1; i <= len; ++i) {
		tmpptr->f = i;
		tmpptr->f2 = i * i;
		tmpptr++;
	}

	munmap(ptr, len_bytes);

	return 0;
}
