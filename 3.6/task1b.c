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

	while (1) {
		struct message buf;
		ssize_t len;

		len = msgrcv(msgid, &buf, 8, 0, 0);
		if (len < 0) {
			perror("msgrcv");
			exit(EXIT_FAILURE);
		}

		if (buf.mtype == FINISH_MESSAGE) {
			msgctl(msgid, IPC_RMID, NULL);
			return 0;
		}

		printf("Message type = %ld\n", buf.mtype);
		printf("Integer = %d\n", buf.integer);
		printf("Real = %f\n", buf.real);
	}

	return 0;
}
