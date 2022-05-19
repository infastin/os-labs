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
	int fd = open("mapped.dat", O_RDONLY | O_CREAT, 0666);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	int len = 100'000;
	int len_bytes = len * sizeof(struct A);

	struct A *ptr = mmap(NULL, len_bytes, PROT_READ, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	close(fd);

	unsigned long sum = 0;

	struct A *tmpptr = ptr;
	for (int i = 0; i < len; ++i) {
		sum += tmpptr->f2;
		tmpptr++;
	}

	munmap(ptr, len_bytes);

	printf("Sum: %lu\n", sum);

	return 0;
}
