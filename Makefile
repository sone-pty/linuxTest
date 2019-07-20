#make构建的过程是先建立关系树，确定依赖关系，然后自底向上执行，最后生成结果

#通用模板，将当前路径下所有的.c文件编译并链接为一个application

#预处理指令
CPPFLAGS= 

#编译指令
CFLAGS= -g -Wall

#加载库
LDFLAGS=

#编译器
CC=g++

src = $(wildcard *.cpp)
obj = $(patsubst %.cpp,%.o,$(src))
target = app

$(target):$(obj)
	$(CC) $^ $(LDFLAGS) -o $@

#$@表示目标，$<表示第一个依赖，$^表示所有依赖，%表示占位符
#所以%.o:%.c表示目标xx.o依赖于xx.c源文件
%.o:%.cpp
	$(CC) -c $<  $(CFLAGS) $(CPPFLAGS) -o $@

#clean模板
-PHONY:clean
clean:
	-rm -f $(obj)
