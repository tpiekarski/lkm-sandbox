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
