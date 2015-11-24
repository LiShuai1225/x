include $(PWD)/../common.mk

all:
	$(CROSS_COMPILE)gcc server.c -o target_server
	$(CROSS_COMPILE)gcc client.c -o target_client
	gcc server.c -o host_server
	gcc client.c -o host_client
clean:
	rm -rf target_server target_client host_client host_server
