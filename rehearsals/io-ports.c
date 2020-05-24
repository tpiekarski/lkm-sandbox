#include <stdio.h>
#include <sys/io.h>
#include <unistd.h>

#define PORT 0x378

int main()
{
	int retval = ioperm(PORT, 3, 1);

	if (retval != 0)
		goto out;

	outb(2, PORT);
	sleep(2);
	printf("Status: %d\n", inb(PORT + 1));

	retval = ioperm(PORT, 3, 0);

	if (retval != 0)
		goto out;

	return 0;

out:
	perror("ioperm");

	return retval;
}
