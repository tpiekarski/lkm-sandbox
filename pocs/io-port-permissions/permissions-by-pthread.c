/*
 * LKM Sandbox::permissions-by-pthread
 * <https://github.com/tpiekarski/lkm-sandbox>
 * ---
 * Copyright 2020 Thomas Piekarski <t.piekarski@deloquencia.de>
 *
 * This file is part of LKM Sandbox.
 * 
 * LKM Sandbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * LKM Sandbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with LKM Sandbox. If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include <pthread.h> // pthreat_t, pthread_create, pthread_join
#include <stdio.h> // (f)printf
#include <string.h> // strcmp
#include <sys/io.h> // iopl, ioperm
#include <unistd.h> // sleep

#define PORT 0x378 // lp0

void *read_from_child()
{
	printf("Reading anything from a new thread to test permissions.\n");

	// The inb() will succeed due to permissions are inherited to
	// childs after they got acquired with either iopl or ioperm
	printf("%x\n", inb(PORT));

	return NULL;
}

void *read_from_sleepy_child()
{
	sleep(2);
	printf("Reading anything delayed from a new thread to test permissions.\n");

	// The inb() will fail due to missing permissions and it'll segfault
	// although permissions are acquired before threads are joined.
	// When permissions are set per process this should work.
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

	pthread_t delayed_thread; // delayed by a few seconds.
	pthread_t thread;

	pthread_create(&delayed_thread, NULL, read_from_sleepy_child, NULL);

	if (strcmp(argv[1], "iopl") == 0) {
		printf("Using iopl to get I/O port permissions.\n");

		retval = iopl(3);
	} else if (strcmp(argv[1], "ioperm") == 0) {
		printf("Using ioperm to get I/O port permissions.\n");

		retval = ioperm(0, 0xFFFF, 1);
	} else {
		printf("Using nothing to get I/O port permissions.\n");

		retval = 0; // Test for segfault ;)
	}

	if (retval != 0) {
		printf("Something just did not work out the way expected.\n");
		printf("Last return code was %d.\n", retval);

		return retval;
	}

	printf("Reading anything from default thread to test permissions.\n");

	// The inb() will succeed due to being the main, default thread
	// where permissions got acquired in first place
	printf("%x\n", inb(PORT));

	pthread_create(&thread, NULL, read_from_child, NULL);
	pthread_join(delayed_thread, NULL);
	pthread_join(thread, NULL);

	return retval;
}
