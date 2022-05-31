#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 512

int main(int argc, char *argv[])
{
	int src_fd = open(__FILE__, O_RDONLY);
	if (src_fd == -1) {
		perror("open src");
		exit(EXIT_FAILURE);
	}

	int dst_fd = open(__FILE__ ".copy", O_CREAT | O_WRONLY, 0666);
	if (dst_fd == -1) {
		perror("open dst");
		exit(EXIT_FAILURE);
	}

	char buf[BUFSIZ];

	while (1) {
		int nread = read(src_fd, buf, BUFSIZE);
		if (nread == -1) {
			perror("read");
			exit(EXIT_FAILURE);
		}

		int nwrite = write(dst_fd, buf, nread);
		if (nwrite == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}

		if (nread < BUFSIZE)
			break;
	}

	close(src_fd);
	close(dst_fd);

	return 0;
}
