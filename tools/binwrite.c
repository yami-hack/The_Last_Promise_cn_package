#include <stdlib.h>
#include <stdio.h>

#include <string.h>


//#define GBA_TOOLS


#ifdef GBA_TOOLS


typedef struct{
	union{

		struct{
			unsigned int	addr:25;
			unsigned int	mem:7;
		};
		int	value;
	}v;
}gba_addr;

int	gba_addr2addr(int rom_addr)
{
	gba_addr addr;
	addr.v.value = rom_addr;
	if(addr.v.mem==4)
	{
		return addr.v.addr;
	}
	else{
		return -1;
	}
}

#endif

static char*	help_a[] = {
	"-F 修改的文件 地址[:大小<文件大小>] 二进制数据文件... ",
	"-H 修改的文件 地址[:大小<1>]		 16进制整数,1:byte,2:short,4:int...",
	"-h 帮助",
	(char*)0,
};

void	printf_help()
{
	char** p;
	
	p = help_a;
	while(*p){
		printf("%s\n",*p);
		p++;
	}
}


char* find_c(char*src,char c){
	while(*src++!=c)
	{
		if(*src==0)
			return 0;
	}
	return src;
}

int		check_num_err(const char*num)
{
	int r;
	r = 1;
	if(num[0]=='0'&&(num[1]=='x'||num[1]=='X'))
	{
		num+=2;
	}
	while(*num!=0)
	{
		if((*num>='0'&&*num<='9')|(*num>='a'&&*num<='f')||(*num>='A'&&*num<='F'))
		{
			r++;
		}
		else{
			return r;
		}
		num++;
	}
	r--;
	if(r>8)
		return r;
	else
		return 0;
}

int	write_hex(int argc,char**argv)
{
	char*	in_name;
	char*	in2_name;
	FILE	*in_file;
	int		s_len;
	char*	s_num;
	char*	s_size;
	int		_size;
	int		_addr;
	int 	i;
	unsigned char	buf_[0x50];
	if(argc<3)
	{
		printf("-H 参数过少,只有%d个参数\n",argc);
		printf("格式:\n%s\n",help_a[1]);
		return 1;
	}
	else{
		in_name	=	argv[0];
		in_file	=	fopen(in_name,"rb+");
		if(!in_file){
			printf("打开%s失败\n",in_name);
			return 1;
		}

		s_len	=	strlen(argv[1]);
		s_num	=	(char*)malloc(s_len+3);
		if(!s_num){
			printf("申请内存失败\n");
			fclose(in_file);
			return 1;
		}
		memcpy(s_num,argv[1],s_len+1);
		if((s_size=find_c(s_num,':'))!=0){
			s_num[(s_size-s_num-1)] = '\0';
			if(check_num_err(s_size))
			{
				printf("地址错误或者超过可写入大小\n");
				fclose(in_file);
				free(s_num);
				return 1;
			}
			sscanf(s_size,"%x",&_size);
			if(_size>4)
				_size=4;
		}
		else{
			_size	=	4;
		}
		
		if(check_num_err(s_num))
		{
			printf("地址错误或者超过大小\n");
			fclose(in_file);
			free(s_num);
			return 1;
		}
		else{
			sscanf(s_num,"%x",&_addr);
		}
		
		free(s_num);
		printf("修改的文件:%s\n",in_name);
		printf("修改地址  :%08X\n",_addr);
		printf("修改大小  :%d\n",_size);

#ifdef GBA_TOOLS
		_addr	=	gba_addr2addr(_addr);
		if(_addr<0)
		{
			printf("非GBA的地址,\n");
			fclose(in_file);
			return 1;
		}
#endif

		if(fseek(in_file,_addr,SEEK_SET))
		{
			fseek(in_file,0,SEEK_END);
			printf("地址超过文件大小:%08X\n",ftell(in_file));
			fclose(in_file);
			return 1;
		}

		for(i=2;i<argc;i++){
			sscanf(argv[i],"%x",buf_);
			fwrite(buf_,_size,1,in_file);
		}

		fclose(in_file);
		printf("写入成功\n");
		return 0;

	}
}

int	write_bin(int argc,char**argv)
{
	char*	in_name;
	FILE	*in_file;
	FILE	*in2_file;
	int		s_len;
	char*	s_num;
	char*	s_size;
	int		_size;
	int		_addr;
	int 	i;
	int 	i_size;
	int 	i_rlen;
	unsigned char	buf_[0x100];
	if(argc<3)
	{
		printf("-F 参数过少,只有%d个参数\n",argc);
		printf("格式:\n%s\n",help_a[0]);
		return 1;
	}
	else{

		in_name	=	argv[0];
		in_file	=	fopen(in_name,"rb+");
		if(!in_file){
			printf("打开%s失败\n",in_name);
			return 1;
		}
		
		s_len	=	strlen(argv[1]);
		s_num	=	(char*)malloc(s_len+3);
		if(!s_num){
			printf("申请内存失败\n");
			fclose(in_file);
			return 1;
		}
		memcpy(s_num,argv[1],s_len+1);
		if((s_size=find_c(s_num,':'))!=0){
			s_num[(s_size-s_num-1)] = '\0';
			if(check_num_err(s_size))
			{
				printf("地址错误或者超过可写入大小\n");
				fclose(in_file);
				free(s_num);
				return 1;
			}
			sscanf(s_size,"%x",&_size);
		}
		else{
			_size	=	-1;
		}
		
		if(check_num_err(s_num))
		{
			printf("地址错误或者超过大小\n");
			fclose(in_file);
			free(s_num);
			return 1;
		}
		else{
			sscanf(s_num,"%x",&_addr);
		}
		
		free(s_num);
		printf("修改的文件:%s\n",in_name);
		printf("修改地址  :%08X\n",_addr);
		printf("修改大小  :%d\n",_size);

#ifdef GBA_TOOLS
		_addr	=	gba_addr2addr(_addr);
		if(_addr<0)
		{
			printf("非GBA的地址,\n");
			fclose(in_file);
			return 1;
		}
#endif

		if(fseek(in_file,_addr,SEEK_SET))
		{
			fseek(in_file,0,SEEK_END);
			printf("地址超过文件大小:%08X\n",ftell(in_file));
			fclose(in_file);
			return 1;
		}
		
		for(i=2;i<argc;i++)
		{
			if(_size>0)
			{
				in2_file	=	fopen(argv[i],"rb");
				if(!in2_file)
				{	
					fclose(in_file);
					printf("打开%s失败\n",argv[i]);
					return 1;
				}
				
				i_size		=	0;
				fseek(in2_file,0,SEEK_END);
				i_size		=	ftell(in2_file);
				fseek(in2_file,0,SEEK_SET);
				while(i_size){
					if(i_size>=0x100)
					{
						i_size 	-= 	0x100;
						i_rlen	=	0x100;
					}
					else{
						i_rlen	=	i_size;
						i_size	=	0;
					}
					fread(buf_,i_rlen,1,in2_file);
					if(i_rlen>_size)
					{
						i_rlen	=	_size;
					}
					
					_size	-=	i_rlen;
					fwrite(buf_,i_rlen,1,in_file);
				}
				fclose(in2_file);
			}
			else if(_size==0){
				break;
			}
			else{
				in2_file	=	fopen(argv[i],"rb");
				if(!in2_file)
				{
					printf("打开%s失败\n",argv[i]);
					fclose(in_file);
					return 1;
				}
				i_size		=	0;
				fseek(in2_file,0,SEEK_END);
				i_size		=	ftell(in2_file);
				fseek(in2_file,0,SEEK_SET);

				while(i_size){
					if(i_size>=0x100)
					{
						i_size 	-= 	0x100;
						i_rlen	=	0x100;
					}
					else{
						i_rlen	=	i_size;
						i_size	=	0;
					}
					fread(buf_,i_rlen,1,in2_file);
					fwrite(buf_,i_rlen,1,in_file);
				}
				fclose(in2_file);
			}
		}
		fclose(in_file);
		printf("写入成功\n");
		return 0;
		
	}
}


int	main(int argc,char*argv[])
{
	if(argc<=1)
	{
		printf("参数错误\n");
		printf("参数格式:\n");
		printf_help();
		return -1;
	}
	else if(argc==2)
	{
		if(!memcmp(argv[1],"-h\0",3)){
			printf_help();
			return 0;
		}
		else{
			printf_help();
			return -1;
		}
	}
	else
	{
		if(argv[1][0]=='-'&&argv[1][2]=='\0')
		{
			switch(argv[1][1])
			{
				case 'F':
					return write_bin(argc-2,argv+2);
				break;
				case 'H':
					return write_hex(argc-2,argv+2);
				break;
				case 'h':
					printf_help();
					return 0;
				break;
				default:
					printf_help();
					return -1;
				break;
			}
		}
		else{
			printf_help();
			return 1;
		}
	}
}