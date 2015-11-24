include $(PWD)/../common.mk

obj-m += dts.o

build:
	make -C $(KERNEL) M=$(CURDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) modules
clean:
	make -C $(KERNEL) M=$(CURDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) clean

