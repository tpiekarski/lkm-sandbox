#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void print_pids()
{
	printf("Parent PID=%d, current PID=%d\n", getppid(), getpid());
}

void pause_execution()
{
	printf("Press a key to continue...");
	getc(stdin);
}

int main()
{
	print_pids();
	pause_execution();
	printf("Forking process with fork...\n");
	pid_t fpid = fork();

	
	if (fpid == 0)
		printf(" > That's the child process, fpid=%d\n", fpid);
	else if (fpid > 0)
		printf(" > That's the parent process, fpid=%d\n", fpid);
	else
		goto out;

	print_pids();

	return 0;

out:
	perror("fork");
	return -1;
}
