#include <signal.h>
#include <stdio.h>

void handler(int sig)
{
	switch (sig) {
	case SIGINT:
		printf("Received SIGINT signal: %d\n", sig);
		break;
	case SIGQUIT:
		printf("Received SIGQUIT signal: %d\n", sig);
		break;
	default: break;
	}
}

int main()
{
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);

	while (1) {}

	return 0;
}
