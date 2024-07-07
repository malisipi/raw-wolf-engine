namespace raw_wolf::simple_ui {
    const uint text_color = 0x00000000;
    MouseEvent last_event;

    void mouse_handler(MouseEvent event){
        last_event = event;
    }

    inline TextTexture* static_text(char* text){
        return text::create(text, text_color);
    }

    bool button(TextTexture* text, SDL_Texture* image, int x, int y, int width, int height){ // Will return click state
        drawer::rect(x, y, width, height, 0x00000000);
        drawer::rect(x+4, y+4, width-8, height-8, 0xFFFFFF00);
        if(text != NULL){
            text::draw(text, x+6, y+6, width-12, height-12);
        } else if(image != NULL) {
            drawer::image(image, x+6, y+6, width-12, height-12);
            //TODO: drawer::image_max_bounds(image, x+6, y+6, width-12, height-12);
        }
        if(
            last_event.click == 1 &&
            (last_event.x >= x && last_event.x <= x+width) &&
            (last_event.y >= y && last_event.y <= y+height)
        ){
            last_event = MouseEvent{};
            return true;
        }
        return false;
    }

    inline bool button(TextTexture* text, int x, int y, int width, int height){
        return button(text, NULL, x, y, width, height);
    }

    bool button(char* text, int x, int y, int width, int height){
        TextTexture* text_texture = text::create(text, text_color);
        bool result = button(text_texture, NULL, x, y, width, height);
        text::text_texture_free(text_texture);
        return result;
    }

    inline bool button(SDL_Texture* image, int x, int y, int width, int height){
        return button(NULL, image, x, y, width, height);
    }

    bool checkbox(TextTexture* text, int x, int y, int size, bool* state){ // Will return changed state
        drawer::rect(x, y, size, size, 0x00000000);
        drawer::rect(x+4, y+4, size-8, size-8, 0xFFFFFF00);
        if(*state) drawer::rect(x+8, y+8, size-16, size-16, 0x00000000);
        text::draw(text, x+size+10, y+2, 0, size-4);
        if(
            last_event.click == 1 &&
            (last_event.x >= x && last_event.x <= x+size) &&
            (last_event.y >= y && last_event.y <= y+size)
        ){
            last_event = MouseEvent{};
            *state = !(*state);
            return true;
        }
        return false;
    }

    bool checkbox(char* text, int x, int y, int size, bool* state){
        TextTexture* text_texture = text::create(text, text_color);
        bool result = checkbox(text_texture, x, y, size, state);
        text::text_texture_free(text_texture);
        return result;
    }

    bool selector(TextTexture* text, char** options, int options_len, int* selected, int x, int y, int width, int height){
        if(width < 160) return false;
        if(*selected >= options_len) return false;
        if(*selected < 0) return false;

        text::draw(text, x+46, y, width-80-12, height/3);
        text::create_and_draw(options[*selected], x+46, y+height/3, width-80-12, height*2/3, text_color);
        bool is_smaller_clicked = button((char*)"<", x, y, 40, height);
        bool is_bigger_clicked = button((char*)">", x+width-40, y, 40, height);
        if(is_smaller_clicked){
            (*selected)--;
            if((*selected) < 0) (*selected) = options_len-1;
        }
        if(is_bigger_clicked){
            (*selected)++;
            if((*selected) >= options_len) (*selected) = 0;
        }
        return is_smaller_clicked || is_bigger_clicked;
    }
}
