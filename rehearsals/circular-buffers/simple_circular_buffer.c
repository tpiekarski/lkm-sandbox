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

	if (index_read == BUFFER_SIZE) {
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

void print_buffer()
{
	printf("Contents of buffer: ");
	for (int i = 0; i < BUFFER_SIZE; i++)
		printf("%i ", buffer[i]);
	printf("\n");
}

int main()
{
	int i = 0;

	print_buffer();

	printf("Writing %i values.\n", BUFFER_SIZE);
	for (i = 1; i < BUFFER_SIZE + 1; i++)
		write(i);

	print_buffer();

	printf("Reading %i values.\n", BUFFER_SIZE);
	for (i = 0; i < BUFFER_SIZE; i++)
		printf("%i ", read());
	printf("\n");

	print_buffer();

	printf("Writing another single value.\n");
	write(9);

	print_buffer();

	return 0;
}
