obj-m += openFileSearch.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc ioctl.c -o ioctl
	insmod openFileSearch.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ioctl
	rmmod openFileSearch
