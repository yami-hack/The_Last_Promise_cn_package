The Last Promise
===================
老外Fire Emblem改版汉化项目,主要是把ROM汉化,或许会有些许bug,但会修复

编译前
----------------------
	搭建devkitpro环境,
	[devkitpro](http://sourceforge.net/projects/devkitpro/)
		devkitpro安装目录不能包括空格
		
	添加下列目录
	devkitpro\
	devkitPro\devkitARM\bin\
	devkitPro\msys\
	devkitPro\msys\bin
	到"path"系统环境变量
	
	然后重启电脑,或者注销再登录使系统环境生效
	
	打开cmd控制台,检查
		make
		arm-none-eabi-gcc
		这些命令是否可用,可用就代表环境搭建成功
	
编译
-----------------------
	在"src\"目录下打开cmd控制台
	然后执行
		make -f TLPV2_1.mk bin
		会生成几个.bin文件,然后依次往ROM地址导入他们
	或者执行
		make -f TLPV2_1.mk write
		当你要保证ROM在本目录,并且ROM名是"fe7tlpv2.1.gba",还需要额外安装python2.7

作者说明
----------------------
	源码是完全免费开放的,不遵循任何协议。
翻译者贡献
----------------------------
	这里针对翻译者
	需要创建一个分支,然后自己管理仓库
	我建议
		先创建一个分支
		然后在"src/TLPcn/"创建一个文件,文件名设置为想要翻译部分(比如章节),
			文件内容先概述描述时间,创建日期,作者,联系方式。还有大概简单的描述对话的情况,然后提交代码到仓库中
			这个主要是方便那些翻译者知道哪些人分工了哪部分
		当翻译完了,再次提交代码仓库
			而我会隔一段时间查看分支的情况
		
		
