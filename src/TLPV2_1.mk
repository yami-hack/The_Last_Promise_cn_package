
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
			
			
			
HACK_ADDR	:=	8005B9C \
				8005B60 \
				8012C9C \
				8005C00 \
				8000564 \
				
WRITE		:=	python binwrite.py
ROM			:=	fe7tlpv2.1.gba

				
ROM_BIN			:=	9EF1500.mbin

ROM_TEXT_BIN	:=	9BD0000.mbin


HACK_REMOVE	:=	$(addprefix -R.,$(HACK_ADDR:= )) \
					-R.link\
					-R.data\
					-R.text


HACK_DATA	:=	$(HACK_ADDR:=.sbin)
				

CLEAN_FILE		:=	$(ROM_BIN)\
					$(HACK_DATA)\
					$(ROM_TEXT_BIN)

			
OUT			:=	text.out

LDS			:=	TLPV2_1.ld
			
O_FILES		:=	$(SRC:.c=.o)



.PHONY:build clean bin write

#链接所有文件
$(OUT):$(O_FILES)
	$(CC)	$(CFLAGS) -T$(LDS) $^ -o $@
	
	
text_draw.o:text_draw.c
	$(CC)	-O2 -mcpu=arm7tdmi -mabi=apcs-gnu -nostdlib -c $< -o $@

%.o:%.c
	$(CC) 	$(CFLAGS) -c $< -o $@
	
	
clean:
	-rm $(OUT)
	-rm $(O_FILES)
	-rm $(CLEAN_FILE)
	
bin:$(HACK_DATA) $(ROM_BIN) $(ROM_TEXT_BIN)
	echo	a
	
#输出段数据
%.sbin:$(OUT)
	$(OBJCOPY) -O binary -j.$(@:.sbin=) $< $@
	
$(ROM_TEXT_BIN):$(OUT)
	$(OBJCOPY) -O binary -j.text -j.link -j.data  $< $@
	
$(ROM_BIN):$(OUT)
	$(OBJCOPY) -O binary $(HACK_REMOVE) $< $@

#===============================================================
#写入ROM中
write:$(HACK_DATA) $(ROM_BIN)
	@echo write data....
	$(WRITE)	$(ROM) 8000564.sbin	0x564
	$(WRITE)	$(ROM) 8005C00.sbin	0x5C00
	$(WRITE)	$(ROM) 8005B60.sbin	0x5B60
	$(WRITE)	$(ROM) 8005B9C.sbin	0x5B9C
	$(WRITE)	$(ROM) 8012C9C.sbin	0x12C9C
	$(WRITE)	$(ROM) 9BD0000.mbin 0x1BD0000
	$(WRITE)	$(ROM) 9EF1500.mbin 0x1EF1500
		