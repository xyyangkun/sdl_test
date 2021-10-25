/******************************************************************************
 *
 *       Filename:  ttf.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2021年10月25日 11时29分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  yangkun (yk)
 *          Email:  xyyangkun@163.com
 *        Company:  yangkun.com
 *        https://stackoverflow.com/questions/33440716/ttf-rendertext-solid-crashing-game-and-visual-studio-csdl-ttf
 *        https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
 * 		  https://thenumbat.github.io/cpp-course/sdl2/02/02.html
 *
 *        gcc ttf.c -o ttf -lSDL2 -lSDL2_ttf 
 * 		  ./ttf /usr/share/fonts/truetype/freefont/FreeMonoOblique.ttf
 *****************************************************************************/
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT (WINDOW_WIDTH)

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(int x, int y, char *text,
        TTF_Font *font, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    //SDL_Color textColor = {255, 255, 255, 0};
    SDL_Color textColor = {0x0, 0x0, 0x0, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    text_width = surface->w;
    text_height = surface->h;

	SDL_SaveBMP(surface, "surface.bmp");

    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

// 怎么构造黑底白字
void get_text_and_rect1(int x, int y, char *text,
        TTF_Font *font, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
	// 设定字体显示颜色
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    text_width = surface->w;
    text_height = surface->h;

	int dst_w = 160;
	int dst_h = 60;

	// 生成一个较大的sufface
	SDL_Surface *temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 
						dst_w, dst_h, 32, \
						//0x00FF0000, 0x0000FF00, 0x000000FF,/*0x00FF0000, 0x0000FF00, 0x000000FF*/
						0x00, 0x00, 0x00,/*0x00FF0000, 0x0000FF00, 0x000000FF*/
						0);
	SDL_Rect src_bounds, dst_bounds;
	if (temp != NULL) {
		src_bounds.x = 0;
		src_bounds.y = 0;
		src_bounds.w = surface->w;
		src_bounds.h = surface->h;

		dst_bounds.x = 0;
		dst_bounds.y = 0;
		dst_bounds.w = surface->w;
		dst_bounds.h = surface->h;

		// 将较小的surface放到较大的sufface中
		if (SDL_LowerBlit(surface, &src_bounds, temp, &dst_bounds) < 0) {
			SDL_FreeSurface(surface);
			SDL_SetError("Couldn't convert image to 16 bpp");
			text = NULL;
		}
		// 再填充另外一块
		dst_bounds.x = 80;
		dst_bounds.y = 30;
		if (SDL_LowerBlit(surface, &src_bounds, temp, &dst_bounds) < 0) {
			SDL_FreeSurface(surface);
			SDL_SetError("Couldn't convert image to 16 bpp");
			text = NULL;
		}
		printf("blit over\n");
	}


	SDL_SaveBMP(surface, "surface.bmp");
	SDL_SaveBMP(temp, "temp.bmp");

    SDL_FreeSurface(temp);
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

int main(int argc, char **argv) {
    SDL_Event event;
    SDL_Rect rect;
    SDL_Window *window;
    char *font_path;
    int quit;

    if (argc == 1) {
        font_path = "FreeSans.ttf";
    } else if (argc == 2) {
        font_path = argv[1];
    } else {
        fprintf(stderr, "error: too many arguments\n");
        exit(EXIT_FAILURE);
    }

    /* Inint TTF. */
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(font_path, 24);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    get_text_and_rect1(0, 0, "hello", font, &rect);

	printf("rect x:%d y:%d, w:%d h:%d\n", rect.x, rect.y, rect.w, rect.h);
  
    TTF_Quit();

    return EXIT_SUCCESS;
}
