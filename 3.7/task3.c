#include <signal.h>
#include <stdio.h>

void (*si_handler)(int sig);
void (*sq_handler)(int sig);

void handler(int sig)
{
	static int si_count = 0;
	static int sq_count = 0;

	switch (sig) {
	case SIGINT:
		printf("Received SIGINT signal: %d\n", sig);
		si_count++;
		break;
	case SIGQUIT:
		printf("Received SIGQUIT signal: %d\n", sig);
		sq_count++;
		break;
	default: break;
	}

	if (si_count == 5)
		signal(SIGINT, si_handler);

	if (sq_count == 2)
		signal(SIGQUIT, sq_handler);
}

int main()
{
	si_handler = signal(SIGINT, handler);
	sq_handler = signal(SIGQUIT, handler);

	while (1) {}

	return 0;
}
