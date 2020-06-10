/*
 * LKM Sandbox::permissions-revisited
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

#define _GNU_SOURCE

#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 0x378 // lp0
#define STACK_SIZE 1024 * 1024
#define EXECVE_TARGET "permissions-with-execve-target"

struct method {
	const char *permissions;
	const char *creation;
};

int get_permissions(const char *method)
{
	int retval = -1;
	printf("Getting I/O port permissions with %s.\n", method);

	if (strcmp(method, "iopl") == 0)
		retval = iopl(3);
	else if (strcmp(method, "ioperm") == 0)
		retval = ioperm(0, 0xFFFF, 1);

	return retval;
}

void test_read(const char *family, pid_t pid)
{
	printf("Trying to access port from %s (PID=%d)\n", family, pid);
	printf(" > Reading from %x: %x\n", PORT, inb(PORT));
}

int clone_func(void *retval)
{
	test_read("child", getpid());

	return 0;
}

int permissions_with_clone()
{
	printf("Testing I/O Permissions inside process created with clone()\n");

	int retval = 0;
	char *stack = malloc(STACK_SIZE);
	if (stack == NULL)
		return -1;
	char *stack_top = stack + STACK_SIZE;
	pid_t cpid = clone(clone_func, stack_top, SIGCHLD, &retval);

	if (cpid == -1)
		retval = -1;

	test_read("parent", getpid());

	return retval;
}

int permissions_with_execve(char *argv[], char *envp[])
{
	printf("Testing I/O Permissions inside process created with execve()\n");

	test_read("parent", getpid());
	int retval = execve(EXECVE_TARGET, argv, envp);

	return retval;
}

int permissions_with_fork()
{
	printf("Testing I/O Permissions inside process created with fork()\n");

	int retval = 0;
	pid_t fpid = fork();

	if (fpid == 0)
		test_read("child", getpid());
	else if (fpid > 0)
		test_read("parent", getpid());
	else
		retval = -1;

	return retval;
}

void *pthread_func()
{
	printf("Accessing by thread (TID=%d)\n", gettid());
	test_read("child", getpid());

	return NULL;
}

int permissions_with_pthread()
{
	printf("Testing I/O Permissions inside process created with pthread\n");

	int retval = 0;
	pthread_t thread;
	pthread_create(&thread, NULL, pthread_func, NULL);
	test_read("parent", getpid());
	pthread_join(thread, NULL);

	return retval;
}

int main(int argc, char *argv[], char *envp[])
{
	if (argc != 3)
		goto usage;

	struct method m = { argv[1], argv[2] };

	// todo: fix failsave with XOR
	/*
	if (strncmp(m.permissions, "iopl", 4) != 0 ||
	    strncmp(m.permissions, "ioperm", 6) != 0)
		goto usage;
	*/

	int permissions_granted = get_permissions(m.permissions);

	if (permissions_granted != 0) {
		printf("Granting permission with %s failed, aborting.\n",
		       m.permissions);

		goto error;
	}

	int retval = 0;

	if (strncmp(m.creation, "clone", 5) == 0)
		retval = permissions_with_clone();
	else if (strncmp(m.creation, "execve", 6) == 0)
		retval = permissions_with_execve(argv, envp);
	else if (strncmp(m.creation, "fork", 4) == 0)
		retval = permissions_with_fork();
	else if (strncmp(m.creation, "pthread", 7) == 0)
		retval = permissions_with_pthread();
	else
		goto usage;

	if (retval != 0)
		goto error;

	return 0;

error:
	perror("permissions");

	return -2;

usage:
	printf("Usage: %s [iopl|ioperm] [clone|fork|execve|pthread]\n",
	       argv[0]);

	return -1;
}
