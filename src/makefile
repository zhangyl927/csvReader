main:main.o DateFrame.o
	g++ -std=c++11 -pg main.o DateFrame.o -o main

main.o:main.cpp
	g++ -std=c++11 -c main.cpp -o main.o

DateFrame.o:DateFrame.cpp
	g++ -std=c++11 -c DateFrame.cpp -o DateFrame.o

clean:
	-rm -rf main main.o DateFrame.o ../outPut/*
