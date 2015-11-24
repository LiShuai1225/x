include $(PWD)/../../common.mk

all:
	$(CROSS_COMPILE)gcc globalfifo_epoll.c -o globalfifo_epoll
	$(CROSS_COMPILE)gcc globalfifo_poll.c  -o globalfifo_poll
	$(CROSS_COMPILE)gcc globalfifo_read.c  -o globalfifo_read
clean:
	rm -rf globalfifo_epoll globalfifo_poll globalfifo_read
