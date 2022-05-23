#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/poll.h>
#include <unistd.h>

struct rcv_msg {
	long mtype;
	pid_t pid;
};

struct snd_msg {
	long mtype;
	char text[32];
};

int main(int argc, char *argv[])
{
	key_t key = ftok("task3a.c", 0);
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
		struct rcv_msg rcv_buf;
		ssize_t len = msgrcv(msgid, &rcv_buf, sizeof(pid_t), 1, 0);
		if (len < 0) {
			perror("msgrcv");
			exit(EXIT_FAILURE);
		}

		printf("Got request from %d\n", rcv_buf.pid);

		struct snd_msg snd_buf = {
			.mtype = rcv_buf.pid,
		};

		snprintf(snd_buf.text, 32, "Message for %d", rcv_buf.pid);

		if (msgsnd(msgid, &snd_buf, 32, 0) < 0) {
			perror("msgsnd");
			exit(EXIT_FAILURE);
		}

		printf("Sent message to %d\n", rcv_buf.pid);
	}

	return 0;
}
