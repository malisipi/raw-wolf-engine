namespace raw_wolf::settings {
    // Config
    uint background_color = 0xFFFFFF00;

    bool fullscreen_state = false;

    const char* resolutions[] = {
        "640x360",
        "640x480",
        "800x600",
        "1024x768",
        "1280x720",
        "1280x800",
        "1280x1024",
        "1360x768",
        "1366x768",
        "1440x900",
        "1536x864",
        "1600x900",
        "1600x1200",
        "1680x1050",
        "1920x1080",
        "3840x2160",
        "[Screen Size]"
    };
    const int resolutions_len = 17;

    void settings(bool is_first_frame) {
        if(is_first_frame){
            drawer::scaling::width = 1280;
            drawer::scaling::height = 720;
            window::mouse_handler = simple_ui::mouse_handler;
        };
        drawer::rescale(); // TODO: Call only window/scaling size changed
        drawer::rect(0, 0, 1280, 720, background_color);

        static TextTexture* fullscreen_text = simple_ui::static_text((char*)"Fullscreen");
        if(simple_ui::checkbox(fullscreen_text, 80, 480, 75, &fullscreen_state)){
            #ifndef __EMSCRIPTEN__
                SDL_SetWindowFullscreen(raw_wolf::window::window, fullscreen_state);
            #else
                if(fullscreen_state){
                    emscripten_run_script((char*)"document.body.requestFullscreen();");
                } else {
                    emscripten_run_script((char*)"document.exitFullscreen();");
                }
            #endif
        };

        static TextTexture* return_text = simple_ui::static_text((char*)"Return");
        if(simple_ui::button(return_text, 80, 580, 300, 75)){
            __loop_return = (void*)1;
        };

        static int max_screen_width = 0;
        static int max_screen_height = 0;
        if(max_screen_width == 0 && max_screen_height == 0){
            auto [_max_screen_width, _max_screen_height] = get_screen_size();
            max_screen_width = _max_screen_width;
            max_screen_height = _max_screen_height;
        };

        static TextTexture* resolution_text = simple_ui::static_text((char*)"Resolution");
        static int selected = 4; // 1280x720
        if(simple_ui::selector(resolution_text, (char**)resolutions, resolutions_len, &selected, 80, 380, 300, 75)){
            if(std::strchr(resolutions[selected], 'x') != NULL){
                char selected_resolution[50];
                sprintf(selected_resolution, "%s", resolutions[selected]);
                const char split[2] = "x";
                char* width_char = strtok(selected_resolution, split);
                char* height_char = strtok(NULL, split);
                int width = atoi(width_char);
                int height = atoi(height_char);
                auto [screen_width, screen_height] = get_screen_size();
                if(
                    (!fullscreen_state && (width<=screen_width && height<=screen_height)) ||
                    (fullscreen_state && (width<=max_screen_width && height<=max_screen_height))
                ){
                    SDL_SetWindowSize(raw_wolf::window::window, width, height);
                }
            } else {
                auto [screen_width, screen_height] = get_screen_size();
                if(!fullscreen_state){
                    SDL_SetWindowSize(raw_wolf::window::window, screen_width, screen_height);
                } else {
                    SDL_SetWindowSize(raw_wolf::window::window, max_screen_width, max_screen_height);
                }
            }
        };
    }
}
