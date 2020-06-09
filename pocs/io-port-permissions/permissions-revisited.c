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

#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void permissions_by_child()
{
	printf("I/O Permissions by child()\n");
}

void permissions_by_execve()
{
	printf("I/O Permissions by execve()\n");
}

void permissions_by_fork()
{
	printf("I/O Permissions by fork()\n");
}

int main(int argc, char const *argv[])
{
	if (argc != 2)
		goto usage;

	const char *method = argv[1];

	if (strncmp(method, "child", 5) == 0)
		permissions_by_child();
	else if (strncmp(method, "execve", 6) == 0)
		permissions_by_execve();
	else if (strncmp(method, "fork", 4) == 0)
		permissions_by_fork();
	else
		goto usage;

	return 0;

error:
	perror("permissions");

	return -2;

usage:
	printf("Usage: %s [child|fork|execve]\n", argv[0]);

	return -1;
}
