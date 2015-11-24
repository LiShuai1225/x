#include <stdio.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_CLEAR 0x1
#define BUFFER_LEN 20

int main(int argc, char *argv[])
{
	char buf[BUFFER_LEN + 1] = { 0 };
	int ret, fd, mode = O_RDONLY;

	if ((argc > 1) && (strcmp(argv[1], "nonblock") == 0)) {
		mode |= O_NONBLOCK;
		printf("+ nonblock read\n");
	} else {
		printf("+ block read\n");
	}

	fd = open("/dev/globalfifo", mode);
	if (fd != -1) {
		if (ioctl(fd, FIFO_CLEAR, 0) < 0)
			printf("ioctl command failed\n");

		printf("+ before read\n");
		ret = read(fd, buf, BUFFER_LEN);
		printf("+ after read\n");

		if (ret < 0) {
			if (errno == EAGAIN)
				printf("+ no data can be read\n");
			else
				printf("read failure\n");
		} else if (ret == 0) {
			printf("no data\n");
		} else {
			buf[ret] = '\0';
			printf("+ read %d(bytes): %s\n", ret, buf);
		}

		close(fd);
	} else {
		printf("device open failure\n");
	}

	return 0;
}
