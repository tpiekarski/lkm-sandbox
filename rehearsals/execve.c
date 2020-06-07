#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
	printf("Executing another program with execve()...\n");
	int rcval = execve("fork", argv, envp);

	// current process will be left here if execve was successful

	if (rcval < 0)
		goto out;

	printf("Still here... exiting program.\n");

	return 0;

out:
	perror("execve");
	return -1;
}
