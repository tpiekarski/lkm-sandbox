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
