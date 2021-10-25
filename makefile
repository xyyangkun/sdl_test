
all:
	gcc ttf.c -o ttf -lSDL2 -lSDL2_ttf 
	gcc ttf2bmp.c -o ttf2bmp -lSDL2 -lSDL2_ttf 
