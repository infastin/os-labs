#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

void handler(int nsig)
{
	int status;
	pid_t pid = waitpid(-1, &status, 0);

	if (pid < 0) {
		perror("waitpid");
		exit(EXIT_FAILURE);
	} else {
		if (status & 0xff) {
			int sig = status & 0x7f;
			int with_core = status & 0x80;
			printf("Process %d was killed by signal %d %s core file\n", pid, sig, with_core ? "with" : "without");
		} else if (status & 0xff00) {
			int code = (status & 0xff00) >> 8;
			printf("Process %d exited with code %d\n", pid, code);
		}
	}
}

int main(void)
{
	signal(SIGCHLD, handler);

	int signals[5] = { SIGINT, SIGQUIT, SIGKILL, SIGTERM, SIGSEGV };

	for (int i = 0; i < 5; ++i) {
		pid_t pid = fork();

		if (pid < 0) {
			char buf[32];
			snprintf(buf, 32, "fork child %d", i);
			perror(buf);
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			pid = getpid();
			kill(pid, signals[i]);
		}
	}

	for (int i = 1; i < 5; ++i) {
		pid_t pid = fork();

		if (pid < 0) {
			char buf[32];
			snprintf(buf, 32, "fork child %d", i);
			perror(buf);
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			exit(i);
		}
	}

	while (1) {}

	return 0;
}
