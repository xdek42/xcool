### xcool
xcool是cool语言的编译器，实现语言为c++。词法分析器与语法分析器均为手工编写。
### cool语言
cool语言为一门面向对象的编程语言。为斯坦福编译原理课程cs143所发明的语言。
cool语言虽然小，但其包含许多现代语言的特性如
> * 面向对象
> * 支持继承与多态
> * 静态类型
> * 垃圾回收

[cool语言手册](http://web.stanford.edu/class/cs143/materials/cool-manual.pdf) 

### 项目当前进度
完善代码生成

### 使用
cd ./xcool/src

make

./xcool ../test/helloworld.cool ../test/helloworld.s

cd ../test

as --32 helloworld.s -o helloworld.o

ld -m elf_i386 helloworld.o -dynamic-linker /lib/ld-linux.so.2 -lc

./a.out