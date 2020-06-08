#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)

int child_func(void *data)
{
	printf("That's the child function...\n");
	printf("It's running in its own process with PID=%d\n", getpid());
	printf("Received data: %d\n", *(int *)data);

	return 0;
}

int main(int argc, char const *argv[])
{
	printf("Creating process with clone(), PID=%d\n", getpid());

	int data = 1001;

	char *stack = malloc(STACK_SIZE);

	if (stack == NULL)
		goto out;

	char *stack_top = stack + STACK_SIZE;

	pid_t pid = clone(child_func, stack_top, SIGCHLD, &data);

	if (pid == -1)
		goto out;

	printf("Cloned process with new PID=%d\n", pid);

	return 0;

out:
	perror("clone");
	return -1;
}
