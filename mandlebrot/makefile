all: sfml-app
	./sfml-app

sfml-app: main.o
	 g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -ffast-math

main.o: main.cpp
	 g++ -c main.cpp -ffast-math