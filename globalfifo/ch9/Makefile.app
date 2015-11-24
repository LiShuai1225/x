include $(PWD)/../../common.mk

all:
	$(CROSS_COMPILE)gcc globalfifo_test.c -o globalfifo_test
	$(CROSS_COMPILE)gcc aior.c -o aior
clean:
	rm -rf globalfifo_test aior
