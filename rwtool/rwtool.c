#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE    512

int f_read(const char *filename)
{
	int fd, ret;
	char buf[BUF_SIZE + 1] = { 0 };

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("File %s open failure, errno=%d\n", filename, errno);
		return -1;
	}

	printf("Before read function\n");
	ret = read(fd, buf, BUF_SIZE);
	printf("After read function\n");

	if (ret < 0) {
		printf("File %s read failure, errno=%d\n", filename, errno);
	} else if (ret == 0) {
		printf("No data in file %s\n", filename);
	} else {
		buf[ret] = '\0';
		printf("Read %d(bytes): %s\n", ret, buf);
	}

	close(fd);

	return ret < 0 ? -1 : 0;
}

int f_write(const char *filename, char *str)
{
	int fd, ret;

	fd = open(filename, O_WRONLY);
	if (fd < 0) {
		printf("File %s open failure, errno=%d\n", filename, errno);
		return -1;
	}

	printf("Before write function\n");
	ret = write(fd, str, strlen(str));
	printf("After write function\n");

	if (ret < 0) {
		printf("File %s write failure, errno=%d\n", filename, errno);
	} else if (ret == 0) {
		printf("No data was writen into file %s\n", filename);
	} else {
		printf("Write %d(bytes): %s\n", ret, str);
	}

	close(fd);

	return ret < 0 ? -1 : 0;

}

int main(int argc, char *argv[])
{
	if (argc < 3)
		goto inval;

	if (strcmp(argv[2], "r") == 0)
		return f_read(argv[1]);

	if ((argc > 3) && (strcmp(argv[2], "w") == 0))
		return f_write(argv[1], argv[3]);

inval:
	fprintf(stderr, "Usage: %s <filename> <r|w> [string]\n", argv[0]);
	return -1;
}
