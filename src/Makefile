Fractal: fractal.o palette.o
	gcc -Wall -g -std=c11 `pkg-config --cflags gtk+-3.0` -o ../bin/Fractal ../obj/fractal.o ../obj/palette.o `pkg-config --libs gtk+-3.0` -lm -lpthread
fractal.o: fractal.c
	gcc -Wall -g -std=c11 `pkg-config --cflags gtk+-3.0` -c -o ../obj/fractal.o fractal.c `pkg-config --libs gtk+-3.0` -lpthread
palette.o: palette.c palette.h
	gcc -Wall -g -std=c11 -c -o ../obj/palette.o palette.c
clean:
	rm -r ../obj/*.o ../bin/*
