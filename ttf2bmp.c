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

// 7yuv可以显示rgb24 获取rgba32
// rgb24使用 ffplay -pixel_format rgb24 -video_size 160x60 -i 160_60.rgb -framerate 1
// rgb24使用 ffplay -pixel_format rgba -video_size 160x60 -i 160_60.rgb -framerate 1 // 32位选择t
void save_rgb(const char *name, const char *buf, const unsigned int len) {
	FILE *fp = fopen(name, "wb");
	if(!fp) {
		printf("ERROR to open rgb file\n");
		return;
	}
	fwrite(buf, len, 1, fp);
	fclose(fp);
}

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
    SDL_Color textColor = {255, 255, 255, 0}; // 白字

    surface = TTF_RenderText_Solid(font, text, textColor);
    text_width = surface->w;
    text_height = surface->h;

	int dst_w = 160;
	int dst_h = 60;

	// 生成一个较大的sufface
	SDL_Surface *temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 
						dst_w, dst_h, 32/*24*/, \
						0x00FF0000, 0x0000FF00, 0x000000FF,/*0x00FF0000, 0x0000FF00, 0x000000FF*/
						//0x00, 0x00, 0x00,/*0x00FF0000, 0x0000FF00, 0x000000FF*/
						0xFF000000/*alpha 0 透明, 所以使用ffmpeg全是黑色，所以要选择ff*/);
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
	printf("BytesPerPixel=%d\n", temp->format->BytesPerPixel);
	//save_rgb("160_60.rgb", temp->pixels, temp->w*temp->h*temp->format->BytesPerPixel);

    SDL_FreeSurface(temp);
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

void get_bmp(char **buf, int *w, int *h) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
	// 设定字体显示颜色
    SDL_Color textColor = {255, 255, 255, 0}; // 白字

	const char *text = "OSEE";
	const char *font_path = "wqy-microhei.ttc";

    TTF_Init();
    TTF_Font *font = TTF_OpenFont(font_path, 24);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }



    surface = TTF_RenderText_Solid(font, text, textColor);
    text_width = surface->w;
    text_height = surface->h;

	int dst_w = 160;
	int dst_h = 60;

	// 生成一个较大的sufface
	SDL_Surface *temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 
						dst_w, dst_h, 32/*24*/, \
						0x00FF0000, 0x0000FF00, 0x000000FF,/*0x00FF0000, 0x0000FF00, 0x000000FF*/
						//0x00, 0x00, 0x00,/*0x00FF0000, 0x0000FF00, 0x000000FF*/
						0xFF000000/*alpha 0 透明, 所以使用ffmpeg全是黑色，所以要选择ff*/);
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


	SDL_SaveBMP(surface, "surface1.bmp");
	SDL_SaveBMP(temp, "temp1.bmp");
	printf("BytesPerPixel=%d\n", temp->format->BytesPerPixel);
	//save_rgb("160_60.rgb", temp->pixels, temp->w*temp->h*temp->format->BytesPerPixel);
	*w = temp->w;
	*h = temp->h;
	int size = temp->w*temp->h*temp->format->BytesPerPixel;
	*buf = (char *)malloc(size);	
	memcpy(*buf, temp->pixels, size);

    SDL_FreeSurface(temp);
    SDL_FreeSurface(surface);

	TTF_Quit();
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


	{
	int w;
	int h;
	char *buf;

	get_bmp(&buf, &w, &h);
	printf("w= %d h=%d\n", w, h);
	char file_name[100]= {0};
	sprintf(file_name, "_%d_%d.rgb", w, h);

	save_rgb(file_name, buf, w*h*4);

	// 使用完要释放内存
	free(buf);
	buf = NULL;


	}

    return EXIT_SUCCESS;
}
