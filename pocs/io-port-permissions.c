#include <pthread.h> // pthreat_t, pthread_create, pthread_join
#include <stdio.h> // (f)printf
#include <string.h> // strcmp
#include <sys/io.h> // iopl, ioperm
#include <unistd.h> // sleep

#define PORT 42

void *read_from_child()
{
	printf("Reading anything from a new thread to test permissions.\n");

	// This should work, because child threads inherit permissions
	printf("%x\n", inb(PORT));

	return NULL;
}

void *read_from_sleepy_child()
{
	sleep(2);
	printf("Reading anything delayed from a new thread to test permission.\n");

	// This won't work, because child thread is created before permissions are
	// acquired by iopl or ioperm. So I/O port permissions are _not_ set by
	// process but by thread. The are only valid inside a thread and its
	// children after the permission got acquired.
	printf("%x\n", inb(PORT));

	return NULL;
}

int main(int argc, char const *argv[])
{
	int retval = 0;

	if (argc != 2) {
		fprintf(stderr, "usage: %s [iopl|ioperm|anything]\n", argv[0]);

		return -2;
	}

	pthread_t reading_thread_1; // delayed by 2 seconds.
	pthread_t reading_thread_2;

	pthread_create(&reading_thread_1, NULL, read_from_sleepy_child, NULL);

	if (strcmp(argv[1], "iopl") == 0) {
		printf("Using iopl to get I/O port permissions.\n");

		retval = iopl(3);
	} else if (strcmp(argv[1], "ioperm") == 0) {
		printf("Using ioperm to get I/O port permissions.\n");

		retval = ioperm(0, 0xFFFF, 1);
	} else {
		printf("Using nothing to get I/O port permissions.\n");

		retval = 0; // Test for segfault
	}

	if (retval != 0) {
		printf("Something just did not work out the way expected.\n");
		printf("Last return code was %d.\n", retval);

		return retval;
	}

	printf("Reading anything from default thread to test permissions.\n");
	printf("%02x\n", inb(PORT));

	pthread_create(&reading_thread_2, NULL, read_from_child, NULL);
	pthread_join(reading_thread_1, NULL);
	pthread_join(reading_thread_2, NULL);

	return retval;
}
