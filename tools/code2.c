/*
 * 码表和字模生成工具
 *
 * */


/**
 * @file		code.c
 * @author		yami
 * @date		2015-04-19 13:12:52
 * @brief
 * 是以前版本的code.c复制过来的,
 * 1,添加生成第二种字体
 */

#define		V_1_0
#define		V_TLP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{
	unsigned	short	rom_code;
	unsigned	char	flag;
}_code;


#ifdef _MSC_VER
#define PACKED
#pragma pack(1)
#else

#define PACKED __attribute__ ((packed))
#endif

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef struct{
    u8          gb[2];
    u8          u[2];
    u8          size[2];
    u8          u4_[0xA];
    struct{
        u8      X[0x10];
    }Y[0x10];
}PACKED fft;


typedef struct{
	union{
		struct{
			unsigned	char	X0:2;
			unsigned	char	X1:2;
			unsigned	char	X2:2;
			unsigned	char	X3:2;
			unsigned	char	X4:2;
			unsigned	char	X5:2;
			unsigned	char	X6:2;
			unsigned	char	X7:2;
			unsigned	char	X8:2;
			unsigned	char	X9:2;
			unsigned	char	XA:2;
			unsigned	char	XB:2;
		}Y[0xC];
		unsigned	char		data[0x24];
	};
}rom_FFT_data;


char				*fft_name	=	"d.fft";
_code				gb_rom_code[0x10000];
int					font_base_addr = 0x8577FF4;


void		init_gb_rom_code(){
	memset(gb_rom_code,0,sizeof(gb_rom_code));
}

int		rom_code_handle(char*fname){
	char			buf[0x20];
	unsigned char	txt_buf[0x13];
	int				code;
	FILE*			code_txt;
	int				i;
	unsigned short	index;
	code_txt	=	fopen(fname,"r");
	if(!code_txt){
		printf("打开%s文件失败\n",fname);
		return 1;
	}
	while(fgets(buf,0x50,code_txt)){
		sscanf(buf,"%4X%s",&code,txt_buf);
		for(i=0;i<0xf;i++){
			if(txt_buf[i]>0x80&&txt_buf[i+1]>0x80){
				index	=	*((unsigned short*)(txt_buf+i));
				index	&=	0xFFFF;
				code	=	((code&0xFF)<<8)|((code>>8));
				gb_rom_code[index].rom_code 	= 	code;
				gb_rom_code[index].flag		=	1;
				break;
			}
		}
	}

	fclose(code_txt);

	return 0;
}


int		out_struct()
{
	printf("%s\n","#define PACKED __attribute__ ((packed))");
	printf("%s\n","typedef struct{");

#ifdef V_TLP
	printf("%s\n","		void*						next;		");
	printf("%s\n","		unsigned	char			codeB;		");
	printf("%s\n","		unsigned	char			w;			");
	printf("%s\n","		unsigned	char			h;");
	printf("%s\n","		unsigned	char			unk_;");
	printf("%s\n","		unsigned	char			data[0x40];		");

#else
	printf("%s\n","		unsigned	char			data_w;		");
	printf("%s\n","		unsigned	char			y;		");
	printf("%s\n","		unsigned	char			font_w;		");
	printf("%s\n","		unsigned	char			u_;");
	printf("%s\n","		unsigned	char			data[0x50];		");
#endif

	printf("%s\n","}rom_FFT;");

	printf("\n");
	printf("\n");

#ifdef V_TLP

#else
	printf("%s\n","typedef struct{");
	printf("%s\n","		int*			point;");
	printf("%s\n","		unsigned short	code;");
	printf("%s\n","}fe_font;");
#endif

	printf("\n");
	printf("\n");

	return 0;
}


#define CC_(xx)		dst->Y[y].X##xx	=	src->Y[y].X[0x##xx]==1?3:((src->Y[y].X[0x##xx-1]==1?2:0))
void		convert_(rom_FFT_data*dst,fft*src){
	int		y;
	memset(dst,0,sizeof(rom_FFT_data));
	for(y=0;y<0xC;y++){
		dst->Y[y].X0	=	src->Y[y].X[0]==1?3:0;
		CC_(1);
		CC_(2);
		CC_(3);
		CC_(4);
		CC_(5);
		CC_(6);
		CC_(7);
		CC_(8);
		CC_(9);
		CC_(A);
		CC_(B);

	}
}

int		out_font(fft*ft){
	rom_FFT_data 	r_fft;
	int		y;
	printf("rom_FFT _f_%02X_%02X = {\n",
			ft->gb[0],
			ft->gb[1]
	);

	printf("\t\t0x%02X,0x%02X,0x%02X,0x0,\n",
			ft->size[0],
			ft->size[1],
			ft->size[0]
	);

	convert_(&r_fft,ft);

	printf("\t\t\t{\n");
	for(y=0;y<0xC;y++){

		printf("\t\t\t\t0x%02X,0x%02X,0x%02X,\n",
				r_fft.data[y*3+0],
				r_fft.data[y*3+1],
				r_fft.data[y*3+2]
		);
	}
	printf("\t\t\t}\n");

	printf("\t};\n\n\n");
	return 0;
}

#ifdef V_TLP
u32		nextCodeB[0x100];

typedef struct{
	void*		next;
	u8			codeB;
	u8			w;
	u8			unk_1;
	u8			unk_2;
	union{
		struct{
			unsigned	char	X0:2;
			unsigned	char	X1:2;
			unsigned	char	X2:2;
			unsigned	char	X3:2;
			unsigned	char	X4:2;
			unsigned	char	X5:2;
			unsigned	char	X6:2;
			unsigned	char	X7:2;
			unsigned	char	X8:2;
			unsigned	char	X9:2;
			unsigned	char	XA:2;
			unsigned	char	XB:2;
			unsigned	char	XC:2;
			unsigned	char	XD:2;
			unsigned	char	XE:2;
			unsigned	char	XF:2;
		}Y[0x10];
		u8		data[0x40];
	};
}tlp_fft;

int		convert_TLP(fft*src,tlp_fft*dst){
	int		y;
	memset(dst,0,sizeof(*dst));
	for(y=0;y<0xC;y++){
		dst->Y[y].X0	=	src->Y[y].X[0]==1?3:0;
		CC_(1);
		CC_(2);
		CC_(3);
		CC_(4);
		CC_(5);
		CC_(6);
		CC_(7);
		CC_(8);
		CC_(9);
		CC_(A);
		CC_(B);
		CC_(C);
		CC_(D);
		CC_(E);
		CC_(F);
	}
	return 0;
}

//黑色是2
//白色是3
//1不知道

#define		CC_V_1_

#ifdef		CC_V_1_
#else
#define CC2_(xx)		\
	dst->Y[dy].X##xx	=	\
	src->Y[y].X[0x##xx]==1\
		?2:((src->Y[y].X[0x##xx-1]==1?3:\
				((src->Y[y-1].X[0x##xx])==1?3:0)\
				))

#define	CC2x_(xx)		\
	dst->Y[dy].X##xx =	\
	dst->Y[dy].X##xx?dst->Y[dy].X##xx:\
			(src->Y[y].X[0x##xx+1]==1?3:\
					0					\
					)

#define CC2y_(xx)		\
	dst->Y[dy-1].X##xx	=	\
	dst->Y[dy-1].X##xx?dst->Y[dy-1].X##xx:\
			(src->Y[y].X[0x##xx]==1?3:\
					0				  \
					)

#define CC2yl_(xx)		\
	dst->Y[dy].X##xx	=	\
	src->Y[y-1].X[0x##xx]==1?3:0

#define CC22_(xx)		dst->Y[dy].X##xx	=	src->Y[y].X[0x##xx]==1?3:((src->Y[y].X[0x##xx-1]==1?2:0))
#endif


#ifdef		CC_V_1_
#define		CC2x__(xx)		dst->Y[dy].X##xx 	= \
								src->Y[y].X[0x##xx]==1?2:\
										dst->Y[dy].X##xx


#define		CC2x_1_(xx,xxx)		dst->Y[dy].X##xx 	= \
								dst->Y[dy].X##xx?dst->Y[dy].X##xx:\
										src->Y[y].X[0x##xxx]==1?3:\
												0

#define		CC2y_1_(xx)			dst->Y[dy-1].X##xx	=	\
								dst->Y[dy-1].X##xx?dst->Y[dy-1].X##xx:\
										src->Y[y].X[0x##xx]==1?3:\
												0

#define		CC2y_2_(xx)			dst->Y[dy+1].X##xx	=	\
								dst->Y[dy+1].X##xx?dst->Y[dy-1].X##xx:\
										src->Y[y].X[0x##xx]==1?3:\
										0


#endif

int		convert_TLP2__(fft*src,fft*dst,int mx,int my){
	int		x,y;
	int		dx,dy;
	for(y=0;y<0x10;y++)
		for(x=0;x<0x10;x++){
			if(src->Y[y].X[x]==1){
				dst->Y[y + my].X[x + mx]=1;
				if(y + my>0){
					//上
					if(dst->Y[y+my - 1].X[x+mx]==1){

					}
					else{
						dst->Y[y+my - 1].X[x+mx]=2;
					}
				}

				if(y + my<16){
					//下
					if(dst->Y[y+my + 1].X[x+mx]==1){

					}
					else{
						dst->Y[y+my + 1].X[x+mx]=2;
					}
				}

				if(x + mx>0){
					//左
					if(dst->Y[y+my].X[x+mx - 1]==1){

					}
					else{
						dst->Y[y+my].X[x+mx - 1]=2;
					}
				}

				if(x + mx<16){
					//左
					if(dst->Y[y+my].X[x+mx + 1]==1){

					}
					else{
						dst->Y[y+my].X[x+mx + 1] = 2;
					}
				}

			}
		}
	return 0;
}

int		convert_TLP2(fft*src,tlp_fft*dst){
	//方框字生成

	int		y;
	int		dy;
	fft		dfft;

	memset(&dfft,0,sizeof(dfft));
	memset(dst,0,sizeof(*dst));

	convert_TLP2__(src,&dfft,2,2);


	y	=	0;
	dy	=	y+2;
	dst->Y[dy].X0	=	src->Y[y].X[0]==1?2:0;




	for(y=0;y<0x10;y++){
		dst->Y[y].X0	=	dfft.Y[y].X[0]==1?2:
				(dfft.Y[y].X[0]==2?3:0);
#define		CCC_(xx)	dst->Y[y].X##xx		=	\
						dfft.Y[y].X[0x##xx]==1?2:\
						(dfft.Y[y].X[0x##xx]==2?3:0)

		CCC_(0);
		CCC_(1);
		CCC_(2);
		CCC_(3);
		CCC_(4);
		CCC_(5);
		CCC_(6);
		CCC_(7);
		CCC_(8);
		CCC_(9);
		CCC_(A);
		CCC_(B);
		CCC_(C);
		CCC_(D);
		CCC_(E);
		CCC_(F);

//
//
//		CC2x_1_(0,1);
//		CC2x_1_(1,2);
//		CC2x_1_(2,3);
//		CC2x_1_(3,4);
//		CC2x_1_(4,5);
//		CC2x_1_(5,6);
//		CC2x_1_(6,7);
//		CC2x_1_(7,8);
//		CC2x_1_(8,9);
//		CC2x_1_(9,A);
//		CC2x_1_(A,B);
//		CC2x_1_(B,C);
//		CC2x_1_(C,D);
//		CC2x_1_(D,E);
//		CC2x_1_(E,F);
//
//		CC2x_1_(1,0);
//		CC2x_1_(2,1);
//		CC2x_1_(3,2);
//		CC2x_1_(4,3);
//		CC2x_1_(5,4);
//		CC2x_1_(6,5);
//		CC2x_1_(7,6);
//		CC2x_1_(8,7);
//		CC2x_1_(9,8);
//		CC2x_1_(A,9);
//		CC2x_1_(B,A);
//		CC2x_1_(C,B);
//		CC2x_1_(D,C);
//		CC2x_1_(E,D);
//		CC2x_1_(F,E);
//
//		CC2y_1_(0);
//		CC2y_1_(1);
//		CC2y_1_(2);
//		CC2y_1_(3);
//		CC2y_1_(4);
//		CC2y_1_(5);
//		CC2y_1_(6);
//		CC2y_1_(7);
//		CC2y_1_(8);
//		CC2y_1_(9);
//		CC2y_1_(A);
//		CC2y_1_(B);
//		CC2y_1_(C);
//		CC2y_1_(D);
//		CC2y_1_(E);
//		CC2y_1_(F);
//
//		CC2y_2_(0);
//		CC2y_2_(1);
//		CC2y_2_(2);
//		CC2y_2_(3);
//		CC2y_2_(4);
//		CC2y_2_(5);
//		CC2y_2_(6);
//		CC2y_2_(7);
//		CC2y_2_(8);
//		CC2y_2_(9);
//		CC2y_2_(A);
//		CC2y_2_(B);
//		CC2y_2_(C);
//		CC2y_2_(D);
//		CC2y_2_(E);
//		CC2y_2_(F);

#ifdef	CC_V_1_

#else
		CC2_(1);
		CC2_(2);
		CC2_(3);
		CC2_(4);
		CC2_(5);
		CC2_(6);
		CC2_(7);
		CC2_(8);
		CC2_(9);
		CC2_(A);
		CC2_(B);
		CC2_(C);
		CC2_(D);
		CC2_(E);
		CC2_(F);

		CC2x_(0);
		CC2x_(1);
		CC2x_(2);
		CC2x_(3);
		CC2x_(4);
		CC2x_(5);
		CC2x_(6);
		CC2x_(7);
		CC2x_(8);
		CC2x_(9);
		CC2x_(A);
		CC2x_(B);
		CC2x_(C);
		CC2x_(D);
		CC2x_(E);

		CC2y_(0);
		CC2y_(1);
		CC2y_(2);
		CC2y_(3);
		CC2y_(4);
		CC2y_(5);
		CC2y_(6);
		CC2y_(7);
		CC2y_(8);
		CC2y_(9);
		CC2y_(A);
		CC2y_(B);
		CC2y_(C);
		CC2y_(D);
		CC2y_(E);
		CC2y_(F);

		CC2yl_(0);
		CC2yl_(1);
		CC2yl_(2);
		CC2yl_(3);
		CC2yl_(4);
		CC2yl_(5);
		CC2yl_(6);
		CC2yl_(7);
		CC2yl_(8);
		CC2yl_(9);
		CC2yl_(A);
		CC2yl_(B);
		CC2yl_(C);
		CC2yl_(D);
		CC2yl_(E);
		CC2yl_(F);
#endif
	}

	return 0;
}


int		out_TLP_font(fft*ft,u32 last){
	tlp_fft		r_fft;
	int			y;
	printf("rom_FFT _f2_%02X_%02X = {\n",
			ft->gb[0],
			ft->gb[1]
	);

	if(last){
		printf("(void*)&_f2_%02X_%02X,\n",
				ft->gb[0],
				last
				);
	}
	else{
		printf("0,\n");
	}
	printf("0x%02X,\t%s\n",ft->gb[1],"//codeB");
	printf("0x%02X,\t%s\n",ft->size[0],"//w");

	printf("0x%02X,\t%s\n",0,"//h");
	printf("0x%02X,\t%s\n",0,"//unk_");


	convert_TLP2(ft,&r_fft);

	printf("\t\t\t{\n");
	for(y=0;y<0x10;y++){
//这个问题困扰了我很久,现在才解决
		printf("\t\t\t\t0x%02X,0x%02X,0x%02X,0x%02X,\n",
				r_fft.data[y*4+0],
				r_fft.data[y*4+1],
				r_fft.data[y*4+2],
				r_fft.data[y*4+3]
		);
	}
	printf("\t\t\t}\n");

	printf("\t};\n\n\n");
	return 0;

}

#endif

int		init()
{
	memset(nextCodeB,0,sizeof(nextCodeB));
	return 0;
}

int		out_src()
{
	FILE			*fft_file;
	int				fft_size;
	int				c_size;
	int				i;
	void*			fft_data_;
	fft*			fft_data;

	fft_file	=	fopen(fft_name,"rb");
	if(!fft_file){
		printf("打开文件  %s失败\n",fft_name);
		return -1;
	}
	fseek(fft_file,0,SEEK_END);
	fft_size	=	ftell(fft_file);
	fseek(fft_file,0,SEEK_SET);

	fft_data_	=	malloc(fft_size);

	if(!fft_data_){
		printf("申请内存失败\n");
		return -1;
	}
	fread(fft_data_,1,fft_size,fft_file);
	fclose(fft_file);

	//输出结构定义
	out_struct();

	fft_data	=	(fft*)fft_data_;
	c_size		=	fft_size;

#ifdef V_TLP
	while(c_size>0){
//		if(nextCodeB[fft_data->gb[0]]){
//
//		}
		out_TLP_font(fft_data,nextCodeB[fft_data->gb[0]]);
		nextCodeB[fft_data->gb[0]]	=	fft_data->gb[1];	//下一个code
		c_size	-=	sizeof(fft);
		fft_data++;
	}


#else
	while(c_size>0){

		if(gb_rom_code[((fft_data->gb[1]<<8)|(fft_data->gb[0]))].flag)
		{
			//已有字模
		}
		else
		{
			//没有字模
			out_font(fft_data);
		}

		c_size	-=	sizeof(fft);
		fft_data++;
	}
#endif


	printf("\n");
	printf("\n");
	printf("\n");

#ifdef 	V_TLP
#else
	printf("#define ROM_CODE(a,b)");
	printf("\t(0x%X + (((((a)-0x81)*0x80)+((b)-0x80))*0x54))\n",font_base_addr);

	fft_data	=	(fft*)fft_data_;
	c_size		=	fft_size;

	while(c_size>0){
		unsigned	short	rom_code;
		printf("#define\t");
		rom_code	=	gb_rom_code[((fft_data->gb[1]<<8)|(fft_data->gb[0]))].rom_code;
		if(gb_rom_code[((fft_data->gb[1]<<8)|(fft_data->gb[0]))].flag)
		{
			//已有字模
			printf("_f_%02X_%02X_p_\t",
					fft_data->gb[0],
					fft_data->gb[1]
			);

			printf("(int*)(ROM_CODE(0x%02X,0x%02X))",
					rom_code&0xff,
					(rom_code>>0x8)&0xff
			);
		}
		else
		{
			//没有字模
			printf("_f_%02X_%02X_p_\t",
					fft_data->gb[0],
					fft_data->gb[1]
			);

			printf("(int*)(&_f_%02X_%02X)",
					fft_data->gb[0],
					fft_data->gb[1]
			);
		}

		printf("\n");

		c_size	-=	sizeof(fft);
		fft_data++;
	}

#endif

	//输出指针
	fft_data	=	(fft*)fft_data_;
	c_size		=	fft_size;
	printf("\n");
	printf("\n");
	printf("\n");

#ifdef	V_TLP
	printf("%s","rom_FFT*\t fonts2[0x100]={\n");
	for(i=0;i<0x100;i++){
		if(nextCodeB[i]){
			printf("\t\t[0x%02X]\t=\t",i);
			//输出最后一个
			printf("&_f2_%02X_%02X,\n",
					i,
					nextCodeB[i]
					);

		}
	}
	printf("\t};");

#else
	printf("%s","fe_font\t fonts[]={\n");

	while(c_size>0){

//		if(gb_rom_code[((fft_data->gb[1]<<8)|(fft_data->gb[0]))].flag)
//		{
//			//已有字模
//		}
//		else
//		{
//			//没有字模
//			out_font(fft_data);
//		}

		printf("\t\t[0x%08X] = ",
				((fft_data->gb[0]-0x81)*0x80)+(fft_data->gb[1]-0x80)		//每0x80一个区
		);

		printf("{(int*)_f_%02X_%02X_p_,0x%04X},    /* %02X%02X */  ",
				fft_data->gb[0],
				fft_data->gb[1],
				gb_rom_code[((fft_data->gb[1]<<8)|(fft_data->gb[0]))].rom_code,
				fft_data->gb[0],
				fft_data->gb[1]
		);

		printf("\n");

		c_size	-=	sizeof(fft);
		fft_data++;
	}

	printf("\n");
	printf("\n");
	printf("\n");

	for(i=0;i<0xffff;i++){
		if(gb_rom_code[i].rom_code){
			unsigned	short	rom_code;
			int		codeA;
			int		codeB;
			int		code;
			rom_code	=	gb_rom_code[i].rom_code;
			codeA	=	rom_code&0xff;
			codeB	=	rom_code>>0x8;
			rom_code	=	gb_rom_code[i].rom_code;
			code	=	((codeA-0x81)*0x80+(codeB-0x80));
			printf("\t\t[0x%08X] = ",
					code
					);

			printf("{(int*)(ROM_CODE(0x%02X,0x%02X)),0x%04X},		/* %02X%02X */  ",
					codeA,
					codeB,
					rom_code,
					codeA,
					codeB
			);

			printf("\n");
		}
	}


	printf("\t};");
#endif



	free(fft_data_);
	return 0;
}


int		main(int c,char**v){
	if(c==2){
		if(rom_code_handle(v[1])){
			return 1;
		}
	}
	if(out_src()){
		return 1;
	}
	return 0;
}
