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
