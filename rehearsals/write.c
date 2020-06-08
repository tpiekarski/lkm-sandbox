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
#include <string.h>

#define FILE_PERMISSION "a"

void write(FILE *fp, const char *message)
{
	fprintf(fp, "%s\n", message);
}

int main(int argc, char const *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "usage: %s <filename> \"<message>\"\n", *argv);

		exit(-EINVAL);
	}

	const char *filename = argv[1];
	const char *message = argv[2];

	FILE *fp = fopen(filename, FILE_PERMISSION);

	if (fp == NULL) {
		fprintf(stderr, "Failed opening file %s, aborting.\n",
			filename);

		exit(-EACCES);
	}

	write(fp, message);
	fclose(fp);

	exit(0);
}
