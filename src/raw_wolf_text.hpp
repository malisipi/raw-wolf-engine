//The flag is created for only debugging purposes. Not recommend to use.
//#define RAW_WOLF_DISABLE_FONTS

namespace raw_wolf::text {
    bool is_init = false;
    TTF_Font* font = NULL;

    void load_font(char* path){
        if(!is_init){
            TTF_Init();
            is_init = true;
        }
        font = TTF_OpenFont(path, 256);
    }

    TextTexture* create(char* text, uint color){
        SDL_Color White = {(Uint8)uint_color_red(color), (Uint8)uint_color_green(color), (Uint8)uint_color_blue(color)};
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, (char*)text, White);
        SDL_Texture* the_text_texture = SDL_CreateTextureFromSurface(window::renderer, text_surface);
        SDL_FreeSurface(text_surface);

        // Font Ratio Calculation
        int font_width = 1;
        int font_height = 1;
        TTF_SizeText(font, text, &font_width, &font_height);
        float font_ratio = (float)font_width/(float)font_height;

        TextTexture* text_texture = new TextTexture {
            .font_ratio = font_ratio,
            .texture = the_text_texture
        };

        return text_texture;
    }

    void text_texture_free(TextTexture* text_texture){
        SDL_DestroyTexture(text_texture->texture);
        free(text_texture);
    }

    void draw(TextTexture* text_texture, int x, int y, int width, int height){
        if(width == 0 && height == 0) return;
        if(width < 0 || height < 0) return;

        if(width == 0){
            width = (int)((float)height * text_texture->font_ratio);
        } else if(height == 0) {
            height = (int)((float)width / text_texture->font_ratio);
        } else {
            if((int)((float)height * text_texture->font_ratio) >= width){ /* If width is bigger than gived width, Use override height */
                int gived_height = height;
                height = (int)((float)width / text_texture->font_ratio);
                y += (gived_height - height) / 2;
            } else {
                int gived_width = width;
                width = (int)((float)height * text_texture->font_ratio);
                x += (gived_width - width) / 2;
            }
        }

        SDL_Rect rect = scaled_sdl_rect(x, y, width, height);
        SDL_RenderCopy(window::renderer, text_texture->texture, NULL, &rect);
    }

    // width or height can be 0. If it's, It will choose auto from gived value.
    void create_and_draw(char* text, int x, int y, int width, int height, uint color){
        TextTexture* text_texture = create(text, color);
        draw(text_texture, x, y, width, height);
        text_texture_free(text_texture);
    }
}
