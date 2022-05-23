#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define FINISH_MESSAGE 255

struct message {
	long mtype;
	int integer;
	float real;
};

int main(int argc, char *argv[])
{
	srand(time(0));

	key_t key = ftok("task1a.c", 0);
	if (key < 0) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	int msgid = msgget(key, 0666 | IPC_CREAT);
	if (msgid < 0) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 5; ++i) {
		struct message buf = {
			.mtype = 1,
			.integer = rand(),
			.real = ldexpf((float) rand(), -32),
		};

		if (msgsnd(msgid, &buf, 8, 0) < 0) {
			perror("msgsnd");
			exit(EXIT_FAILURE);
		}
	}

	struct message buf = {
		.mtype = FINISH_MESSAGE
	};

	msgsnd(msgid, &buf, 0, 0);

	return 0;
}
