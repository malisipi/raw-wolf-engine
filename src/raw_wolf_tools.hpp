namespace raw_wolf::tools {
    void open(char* url, bool is_relative){
        if(std::strchr(url, '\'') != NULL) return;
        if(std::strchr(url, '\"') != NULL) return;
        if(std::strchr(url, '$') != NULL) return;
        if(std::strchr(url, '(') != NULL) return;
        if(std::strchr(url, ')') != NULL) return;
        if(std::strchr(url, '[') != NULL) return;
        if(std::strchr(url, ']') != NULL) return;
        if(std::strchr(url, '%') != NULL) return;
        if(std::strchr(url, '&') != NULL) return;
        if(std::strchr(url, '`') != NULL) return;

        char buffer[1023];
        if(is_relative){
            const char* current_path = std::filesystem::current_path().string().c_str();
            sprintf(buffer, "%s/%s", current_path, url);
        } else {
            sprintf(buffer, "%s", url);
        }

        #ifdef _WIN32
            ShellExecute(NULL, "open", buffer, NULL, NULL, SW_SHOW);
        #elif __linux__
            sprintf(buffer, "xdg-open \"%s\"", url);
            system(buffer);
        #elif __EMSCRIPTEN__
            sprintf(buffer, "window.open(\"%s\", \"_blank\");", url);
            emscripten_run_script(buffer);
        #endif
    }

    void open(char* url){
        return open(url, false);
    }

    float float_rand(){
        return (float)rand()/(float)RAND_MAX;
    }

    unsigned char color_mix_one_color(unsigned char color1, unsigned char color2, float mix_value){
        if(color1-color2 < 0) return color_mix_one_color(color2, color1, 1-mix_value);
        return color1-(color1-color2)*mix_value;
    }

    uint color_mix(uint color1, uint color2, float mix_value){
        if(mix_value == 0) return color1;
        if(mix_value == 1) return color2;
        return uint_rgba_color_from(
            color_mix_one_color(uint_color_red(color1), uint_color_red(color2), mix_value),
            color_mix_one_color(uint_color_green(color1), uint_color_green(color2), mix_value),
            color_mix_one_color(uint_color_blue(color1), uint_color_blue(color2), mix_value),
            color_mix_one_color(uint_color_alpha(color1), uint_color_alpha(color2), mix_value)
        );
    }

    void image_mix(SDL_Texture* image1, SDL_Texture* image2, float mix_value, int x, int y, int width, int height, int rotation){
        if(mix_value == 0) return raw_wolf::drawer::image(image1, x, y, width, height, rotation);
        if(mix_value == 1) return raw_wolf::drawer::image(image2, x, y, width, height, rotation);
        raw_wolf::drawer::image(image1, x, y, width, height, rotation, mix_value);
        raw_wolf::drawer::image(image2, x, y, width, height, rotation, 1-mix_value);
    }

    inline void image_mix(SDL_Texture* image1, SDL_Texture* image2, float mix_value, int x, int y, int width, int height){
        return image_mix(image1, image2, mix_value, x, y, width, height, 0);
    }
}
