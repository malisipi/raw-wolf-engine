namespace raw_wolf::drawer {
    namespace scaling {
        int width = 1280;
        int height = 720;
    }

    float scale_width;
    float scale_height;
    float scale;
    int empty_x_area;
    int empty_y_area;

    void __mouse_handler (MouseEvent event) {
        int scaled_x = (event.x-empty_x_area)/scale;
        if(scaled_x < 0 || scaled_x > scaling::width) return;
        int scaled_y = (event.y-empty_y_area)/scale;
        if(scaled_y < 0 || scaled_y > scaling::height) return;
        if(window::mouse_handler == NULL) return;
        window::mouse_handler(MouseEvent {
            .x = scaled_x,
            .y = scaled_y,
            .button = event.button,
            .click = event.click
        });
    }

    void rescale() {
        scale_width = (float)window::width/scaling::width;
        scale_height = (float)window::height/scaling::height;
        scale = std::min(scale_width, scale_height);
        empty_x_area = (int)(((scale_width-scale)*scaling::width)/2);
        empty_y_area = (int)(((scale_height-scale)*scaling::height)/2);
    }

    void clean_outside(){
        SDL_SetRenderDrawColor(window::renderer,
            uint_color_red(details::theme_color),
            uint_color_green(details::theme_color),
            uint_color_blue(details::theme_color),
            uint_color_alpha(details::theme_color)
        );
        if(empty_x_area == 0){
            SDL_Rect rect = SDL_Rect({0, 0, window::width, empty_y_area});
            SDL_RenderFillRect(window::renderer, &rect);
            rect.y = window::height - empty_y_area;
            SDL_RenderFillRect(window::renderer, &rect);
        } else if(empty_y_area == 0){
            SDL_Rect rect = SDL_Rect({0, 0, empty_x_area, window::height});
            SDL_RenderFillRect(window::renderer, &rect);
            rect.x = window::width - empty_x_area;
            SDL_RenderFillRect(window::renderer, &rect);
        }
    }

    void rect(int x, int y, int width, int height, uint color) {
        SDL_SetRenderDrawColor(window::renderer,
            uint_color_red(color),
            uint_color_green(color),
            uint_color_blue(color),
            uint_color_alpha(color)
        );
        SDL_Rect rect = scaled_sdl_rect(x, y, width, height);
        SDL_RenderFillRect(window::renderer, &rect);
    }

    void line(int x1, int y1, int x2, int y2, uint color) {
        SDL_SetRenderDrawColor(window::renderer,
            uint_color_red(color),
            uint_color_green(color),
            uint_color_blue(color),
            uint_color_alpha(color)
        );
        SDL_Rect r1 = scaled_sdl_rect(x1, y1, 0, 0);
        SDL_Rect r2 = scaled_sdl_rect(x2, y2, 0, 0);
        SDL_RenderDrawLine(window::renderer, r1.x, r1.y, r2.x, r2.y);
    }

    SDL_Texture* load_bmp_image(char* path){
        SDL_Surface* image_surface = SDL_LoadBMP(path);
        SDL_Texture* image_texture = SDL_CreateTextureFromSurface(window::renderer, image_surface);
        SDL_FreeSurface(image_surface);
        return image_texture;
    }

    SDL_Texture* load_image(char* path){
        SDL_Surface* image_surface = IMG_Load(path);
        SDL_Texture* image_texture = SDL_CreateTextureFromSurface(window::renderer, image_surface);
        SDL_FreeSurface(image_surface);
        return image_texture;
    }

    void image(SDL_Texture* image, int x, int y, int width, int height){
        SDL_Rect rect = scaled_sdl_rect(x, y, width, height);
        SDL_RenderCopyEx(window::renderer, image, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    }

    void image(SDL_Texture* image, int x, int y, int width, int height, int rotation){ // Function overloading
        SDL_Rect rect = scaled_sdl_rect(x, y, width, height);
        SDL_RenderCopyEx(window::renderer, image, NULL, &rect, rotation, NULL, SDL_FLIP_NONE);
    }

    void image(SDL_Texture* image, int x, int y, int width, int height, int rotation, float transparency){ // Function overloading x2
        SDL_Rect rect = scaled_sdl_rect(x, y, width, height);
        SDL_SetTextureAlphaMod(image, 255-(int)(transparency*255));
        SDL_RenderCopyEx(window::renderer, image, NULL, &rect, rotation, NULL, SDL_FLIP_NONE);
        SDL_SetTextureAlphaMod(image, 255);
    }
}
