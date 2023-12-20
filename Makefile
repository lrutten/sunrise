sunrise: sunset.o main.o
	g++ -o sunrise main.o sunset.o


sunset.o: sunset.h sunset.cpp
	g++ -c sunset.cpp

main.o: main.cpp sunset.h
	g++ -c main.cpp

clean:
	rm -vf sunrise
	rm -vf *.o

