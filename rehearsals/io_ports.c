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
