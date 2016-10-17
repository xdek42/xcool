#target
xcool: lexer.h parser.h error.h auxiliary.o lexer.o AstTree.o parser.o
	g++ auxiliary.o lexer.o AstTree.o parser.o main.cpp -o xcool -std=c++11

#auxiliary function
auxiliary.o : auxiliary.h auxiliary.cpp
	g++ auxiliary.cpp -c -std=c++11

#error handle
error.h: auxiliary.h

#lexer module
lexer.o : lexer.h error.h lexer.cpp
	g++ lexer.cpp -c -std=c++11

#Abstract syntax tree definition
AstTree.o: AstTree.h AstTree.cpp
	g++ AstTree.cpp -c -std=c++11

#parser module
parser.h: AstTree.h 
parser.o : parser.h error.h auxiliary.h parser.cpp
	g++ parser.cpp -c -std=c++11

clean:
	rm *.o xcool

