#2015-03-10 00:33:10
#最简单的
PREFIX	:= arm-none-eabi-

export 	CC		:= 	$(PREFIX)gcc
export	LD		:= 	$(PREFIX)ld
export AS		:=	$(PREFIX)as
export OBJCOPY	:= $(PREFIX)objcopy


ARCH	:= -mthumb 

#命令是arm-none-eabi-gcc -O2 -mcpu=arm7tdmi -mabi=apcs-gnu -nostdlib -mthumb 

export CFLAGS	:=	-O2 \
			-mcpu=arm7tdmi \
			-mabi=apcs-gnu \
			-nostdlib \
			$(ARCH)
			
