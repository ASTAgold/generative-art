all: sfml-app
	./sfml-app

sfml-app: main.o
	 g++ -fopenacc main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	 g++ -c main.cpp