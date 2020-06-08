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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_PERMISSION "r"

void read(FILE *fp)
{
	int ch = fgetc(fp);
	while (!feof(fp)) {
		putc(ch, stdout); // possible shortcut: putchar(ch);
		ch = fgetc(fp);
	}
}

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <filename>\n", *argv);

		exit(-EINVAL);
	}

	const char *filename = argv[1];
	FILE *fp = fopen(filename, FILE_PERMISSION);

	if (fp == NULL) {
		fprintf(stderr, "Failed opening file %s, aborting.\n",
			filename);

		exit(-ENOENT);
	}

	read(fp);
	fclose(fp);

	exit(0);
}
