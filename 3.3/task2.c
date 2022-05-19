#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	DIR *dir = opendir(".");
	if (dir == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	errno = 0;

	while (1) {
		struct dirent *entry = readdir(dir);
		if (!entry)
			break;

		printf("%s\n", entry->d_name);
	}

	if (errno != 0) {
		perror("readdir");
		exit(EXIT_FAILURE);
	}

	closedir(dir);

	return 0;
}
