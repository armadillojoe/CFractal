Fractal: fractal.o palette.o jBitMap.o
	gcc -Wall -g -std=c11 -o ../bin/Fractal ../obj/fractal.o ../obj/palette.o ../obj/jBitMap.o -lm -lpthread
fractal.o: fractal.c
	mkdir ../obj && mkdir ../bin && gcc -Wall -g -std=c11 -c -o ../obj/fractal.o fractal.c -lpthread
palette.o: palette.c palette.h jColor.h
	gcc -Wall -g -std=c11 -c -o ../obj/palette.o palette.c
jBitMap.o: jBitMap.c jBitMap.h jColor.h
	gcc -Wall -g -std=c11 -c -o ../obj/jBitMap.o jBitMap.c
clean:
	rm -r ../obj ../bin
