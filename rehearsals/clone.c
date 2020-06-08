/*
 * LKM Sandbox::Rehearsal
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

int main()
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
