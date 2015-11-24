#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

#define EVENT_FILE    "/dev/input/event0"

int main(int argc, char *argv[])
{
	int fd;
	int ret;
	struct input_event buf;

	fd = open(EVENT_FILE, O_RDONLY);
	if (fd < 0) {
		printf("open %s fail\n", EVENT_FILE);
		return -1;
	}

	printf("waiting key event...\n");
	while (1) {
		ret = read(fd, &buf, sizeof(struct input_event));
		if (ret <= 0) {
			printf("read key event fail\n");
			break;
		}

		if (buf.code > 0)
			printf("key.code=%d key.value=%d\n", buf.code,
			       buf.value);
	}

	close(fd);

	return 0;
}
