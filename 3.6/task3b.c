#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct snd_msg {
	long mtype;
	pid_t pid;
};

struct rcv_msg {
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

	pid_t pid = getpid();
	struct snd_msg snd_buf = {
		.mtype = 1,
		.pid = pid,
	};

	if (msgsnd(msgid, &snd_buf, sizeof(pid_t), 0) < 0) {
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}

	printf("%d: Sent request to the server\n", pid);

	struct rcv_msg rcv_buf;
	ssize_t len = msgrcv(msgid, &rcv_buf, 32, pid, 0);
	if (len < 0) {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	printf("%d: Got answer: %s\n", pid, rcv_buf.text);

	return 0;
}
