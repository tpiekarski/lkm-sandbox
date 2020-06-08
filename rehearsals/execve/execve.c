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
