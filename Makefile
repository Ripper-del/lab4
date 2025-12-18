# Якщо змінна KDIR не задана, беремо поточне ядро системи
KDIR ?= /lib/modules/$(shell uname -r)/build

obj-m += hello.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
