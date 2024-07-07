#define FLAG_FPS_Counter false
#define uint_color_red(color)   ((color >> (6*4)) & 0xFF)
#define uint_color_green(color) ((color >> (4*4)) & 0xFF)
#define uint_color_blue(color)  ((color >> (2*4)) & 0xFF)
#define uint_color_alpha(color) (color & 0xFF)
#define uint_rgb_color_from(red, green, blue) ((((uint)red << (6*4)) & 0xFF000000) + (((uint)green << (4*4)) & 0x00FF0000) + (((uint)blue << (2*4)) & 0x0000FF00))
#define uint_rgba_color_from(red, green, blue, alpha) ((((uint)red << (6*4)) & 0xFF000000) + (((uint)green << (4*4)) & 0x00FF0000) + (((uint)blue << (2*4)) & 0x0000FF00) + (alpha & 0xFF))
#define scaled_sdl_rect(x, y, width, height) (SDL_Rect({.x=(int)(x*raw_wolf::drawer::scale)+raw_wolf::drawer::empty_x_area, .y=(int)(y*raw_wolf::drawer::scale) + raw_wolf::drawer::empty_y_area, .w=(int)(width*raw_wolf::drawer::scale), .h=(int)(height*raw_wolf::drawer::scale)}))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define PI 3.1415