
#引用gba的头文件

include		gcc-gba.mk

SRC			:=	font.c\
			text_decode.c\
			text_font.c\
			text_show.c\
			texts.c\
			link.c \
			font2.c
			
			
			
HACK_ADDR	:=	8005B9C \
				8005B60 \
				8012C9C \
				8005C00 \

				
ROM_BIN			:=	text.mbin

ROM_TEXT_BIN	:=	text.mtbin

HACK_REMOVE	:=	$(addprefix -R.,$(HACK_ADDR:= ))

HACK_DATA	:=	$(HACK_ADDR:=.sbin)
				

CLEAN_FILE		:=	$(ROM_BIN)\
					$(HACK_DATA)

			
OUT			:=	text.out

LDS			:=	text.ld
			
O_FILES		:=	$(SRC:.c=.o)



.PHONY:build clean bin

#链接所有文件
$(OUT):$(O_FILES)
	$(CC)	$(CFLAGS) -T$(LDS) $^ -o $@

%.o:%.c
	$(CC) 	$(CFLAGS) -c $< -o $@
	
	
clean:
	-rm $(OUT)
	-rm $(O_FILES)
	-rm $(CLEAN_FILE)
	
bin:$(HACK_DATA) $(ROM_BIN)
	echo	a
	
#输出段数据
%.sbin:$(OUT)
	$(OBJCOPY) -O binary -j.$(@:.sbin=) $< $@
	
%.mbin:$(OUT)
	$(OBJCOPY) -O binary $(HACK_REMOVE) $< $@

	