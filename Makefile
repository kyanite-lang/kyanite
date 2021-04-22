out=bin/
src=src/
obj=$(out)obj/

cc=gcc
ld=gcc

ccopts=-std=c++17
ldopts=-lstdc++

$(out)kyanite: $(obj)kyanite.o
	$(ld) -o $(out)kyanite $(obj)kyanite.o $(ldopts)

$(obj)kyanite.o: $(src)kyanite.cpp $(src)Lexer.cpp
	$(cc) -o $(obj)kyanite.o -c $(src)kyanite.cpp $(ccopts)

prepare:
	mkdir -p $(obj)

clean:
	rm -f $(obj)*.o
	rm -f $(obj)kyanite