file = avr_keys
obj-m += $(file).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

insmod:
	sudo insmod $(file).ko

rmmod:
	sudo rmmod $(file).ko

