file = avr_keys
obj-m += $(file).o

all:
	make -C /usr/src/linux-headers-4.4.13-v7+ M=$(PWD) modules

clean:
	make -C /usr/src/linux-headers-4.4.13-v7+ M=$(PWD) clean

ismod:
	sudo insmod $(file).ko

rmmod:
	sudo rmmod $(file).ko

