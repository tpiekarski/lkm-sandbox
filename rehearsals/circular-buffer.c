#include <stdio.h>

#define BUFFER_SIZE 8

static int buffer[BUFFER_SIZE];
static int buffer_length = 0;
static int index_read = 0;
static int index_write = 0;

int read(void)
{
	if (buffer_length == 0) {
		fprintf(stderr, "Can not read, buffer is empty, aborting.\n");

		return -1;
	}

	int value = buffer[index_read++];
	buffer_length--;

	if (index_read = BUFFER_SIZE) {
		index_read = 0;
	}

	return value;
}

void write(int value)
{
	if (buffer_length == BUFFER_SIZE) {
		fprintf(stderr, "Can not write, buffer is full, aborting\n");

		return;
	}

	buffer[index_write++] = value;
	buffer_length++;

	if (index_write == BUFFER_SIZE) {
		index_write = 0;
	}
}

int main(int argc, char const *argv[])
{
	return 0;
}
