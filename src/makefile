version = -std=c++11

ALL:main

main:main.o
	g++ main.o -o main
main.o:main.cpp
	g++ $(version) -c main.cpp -o main.o

clean:
	-rm -rf main main.o
