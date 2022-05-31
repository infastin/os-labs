#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int fd[2];

	if (pipe(fd) < 0) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();

	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	sigset_t set;
	int sig;

	sigemptyset(&set);

	if (pid == 0) {
		pid_t ppid = getppid();
		pid = getpid();

		sigaddset(&set, SIGUSR2);
		sigprocmask(SIG_BLOCK, &set, NULL);

		for (int i = 0; i < 5; ++i) {
			char buf[64];
			sprintf(buf, "Message from child (%d)", pid);

			if (write(fd[1], buf, 64) < 0) {
				perror("write in child");
				exit(EXIT_FAILURE);
			}

			if (kill(ppid, SIGUSR1) < 0) {
				perror("kill in child");
				exit(EXIT_FAILURE);
			}

			if (sigwait(&set, &sig) < 0) {
				perror("sigwait in child");
				exit(EXIT_FAILURE);
			}

			if (read(fd[0], buf, 64) < 0) {
				perror("read in child");
				exit(EXIT_FAILURE);
			}

			printf("Child received: %s\n", buf);
		}
	} else {
		pid_t cpid = pid;
		pid = getpid();

		sigaddset(&set, SIGUSR1);
		sigprocmask(SIG_BLOCK, &set, NULL);

		for (int i = 0; i < 5; ++i) {
			char buf[64];

			if (sigwait(&set, &sig) < 0) {
				perror("sigwait in parent");
				exit(EXIT_FAILURE);
			}

			if (read(fd[0], buf, 64) < 0) {
				perror("read in parent");
				exit(EXIT_FAILURE);
			}

			printf("Parent received: %s\n", buf);
			sprintf(buf, "Message from parent (%d)", pid);

			if (write(fd[1], buf, 64) < 0) {
				perror("write in parent");
				exit(EXIT_FAILURE);
			}

			if (kill(cpid, SIGUSR2) < 0) {
				perror("kill in parent");
				exit(EXIT_FAILURE);
			}
		}
	}
}
