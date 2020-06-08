#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define EXECVE_TARGET "execve-target"

int main(int argc, char *argv[], char *envp[])
{
	printf("Running execve PID=%d\n", getpid());
	printf("Executing %s with execve\n", EXECVE_TARGET);
	int rcval = execve(EXECVE_TARGET, argv, envp);

	// current process will be left here if execve was successful

	if (rcval < 0)
		goto out;

	printf("This line will never be hit!\n");

	return 0;

out:
	perror("execve");
	return -1;
}
