#-*- coding: utf-8 -*-  
#coding=utf-8
import sys

def BINWrite(File,data,addr,len):
	File.seek(addr,0);
	File.write(data);

#命令行参数   binwrite.py dstBin srcBin addr [size]



# f = open("test.bin","rb+");
# s = "abcdef";
# BINWrite(f,s,0,len(s));
# f.close();
argc    =   len(sys.argv);
if (argc<4):
	print("错误参数,格式是,binwrite.py ROM文件 修改文件 十六进制地址");
	exit();

dstFile	=	open(sys.argv[1],"rb+");
srcFile	=	open(sys.argv[2],"rb");
address	=	int (sys.argv[3],16);
srcData	=	srcFile.read();
srcFile.close();
BINWrite(dstFile,srcData,address,len(srcData));
dstFile.close();
