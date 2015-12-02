
#引用gba的头文件

include		gcc-gba.mk

SRC			:=	font.c\
			text_decode.c\
			text_font.c\
			text_show.c\
			texts.c\
			link.c \
			font2.c \
			text_draw.c \
			
			
				
ROM			:=	fe7tlpv2.1.gba



			
OUT			:=	fe7tlp.elf

LDS			:=	fe7tlprom.ld
			
OBJS		:=	$(SRC:.c=.o) 

OBJCOPYFLAGS	:=	--set-section-flags .rom="r,c,a"


.PHONY:build clean elf gba all

gba:$(ROM)

elf:$(OUT)
	

$(ROM):$(OUT)
	$(OBJCOPY)	-R.link $<
	$(OBJCOPY) $(OBJCOPYFLAGS) $<
	$(OBJCOPY) -O binary $< $@
	

	
#链接所有文件
$(OUT):$(OBJS) fe7tlp.rom.o
	$(CC)	$(CFLAGS) -T$(LDS) $^ -o $@
	
	
text_draw.o:text_draw.c
	$(CC)	-O2 -mcpu=arm7tdmi -mabi=apcs-gnu -nostdlib -c $< -o $@

%.o:%.c
	$(CC) 	$(CFLAGS) -c $< -o $@
	
	
clean:
	-rm $(OUT)
	-rm $(OBJS)


# bin:$(HACK_DATA) $(ROM_BIN) $(ROM_TEXT_BIN)
	# echo	a
	
#输出段数据
# %.sbin:$(OUT)
	# $(OBJCOPY) -O binary -j.$(@:.sbin=) $< $@
	
# $(ROM_TEXT_BIN):$(OUT)
	# $(OBJCOPY) -O binary -j.text -j.link -j.data  $< $@
	
# $(ROM_BIN):$(OUT)
	# $(OBJCOPY) -O binary $(HACK_REMOVE) $< $@

# ===============================================================
# 写入ROM中
# write:$(HACK_DATA) $(ROM_BIN) $(ROM_TEXT_BIN)
	# @echo write data....
	# $(WRITE)	$(ROM) 8000564.sbin	0x564
	# $(WRITE)	$(ROM) 8005C00.sbin	0x5C00
	# $(WRITE)	$(ROM) 8005B60.sbin	0x5B60
	# $(WRITE)	$(ROM) 8005B9C.sbin	0x5B9C
	# $(WRITE)	$(ROM) 8012C9C.sbin	0x12C9C
	# $(WRITE)	$(ROM) 9BD0000.mbin 0x1BD0000
	# $(WRITE)	$(ROM) 9EF1500.mbin 0x1EF1500
		