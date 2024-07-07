namespace raw_wolf::welcome {
    // Config
    SDL_Texture* background_image = NULL;
    uint background_color = 0x00000000;
    char* title = NULL;
    uint title_color = 0xFFFFFFFF;
    std::function<void(void)> credits = NULL;
    std::function<void(void)> custom_controller = NULL;

    namespace returning {
        const int new_game = 1;
    }

    typedef struct {
        SDL_Texture* icon;
        std::function<void(void)> function_call;
    } mini_button;

    const int MAX_MINI_BUTTON = 8;
    mini_button* mini_button_list[MAX_MINI_BUTTON];
    int mini_button_index = 0;

    void register_mini_button(SDL_Texture* icon, std::function<void(void)> function_call){
        if(mini_button_index>=MAX_MINI_BUTTON) return;
        mini_button_list[mini_button_index++] = new mini_button{
            .icon = icon,
            .function_call = function_call
        };
    };

    void welcome(bool is_first_frame) {
        if(is_first_frame){
            reset_scene:
            drawer::scaling::width = 1280;
            drawer::scaling::height = 720;
            window::mouse_handler = simple_ui::mouse_handler;
            if(title == NULL){
                title = details::app_name;
            }
            if(title_color == 0xFFFFFFFF){
                title_color = details::theme_color;
            }
        };

        drawer::rescale(); // TODO: Call only window/scaling size changed

        if(background_image != NULL){
            drawer::image(background_image, 0, 0, 1280, 720);
        } else {
            drawer::rect(0, 0, 1280, 720, background_color);
        }
        if(text::font != NULL){
            static TextTexture* title_text = text::create(title, title_color);
            text::draw(title_text, 40, 40, 0, 80);
        }

        static TextTexture* play_text = simple_ui::static_text((char*)"Play");
        static TextTexture* credits_text = simple_ui::static_text((char*)"Credits");

        if(credits != NULL){
            if(simple_ui::button(play_text, 80, 355, 300, 60)){
                __loop_return = (void*)(returning::new_game);
            };
            if(simple_ui::button(credits_text, 80, 430, 300, 60)){
                credits();
            };
        } else {
            if(simple_ui::button(play_text, 80, 430, 300, 60)){
                __loop_return = (void*)(returning::new_game);
            };
        }

        static TextTexture* settings_text = simple_ui::static_text((char*)"Settings");
        if(simple_ui::button(settings_text, 80, 505, 300, 60)){
            void* state = raw_wolf::loop(raw_wolf::settings::settings);
            goto reset_scene;
        };

        #ifndef __EMSCRIPTEN__
            static TextTexture* quit_text = simple_ui::static_text((char*)"Quit");
            if(simple_ui::button(quit_text, 80, 580, 300, 60)){
                exit(0);
            };
        #endif

        for(int mini_button_index=0;mini_button_index<MAX_MINI_BUTTON;mini_button_index++){
            if(mini_button_list[mini_button_index] != NULL){
                if(simple_ui::button(mini_button_list[mini_button_index]->icon, 1200 - mini_button_index*80, 640, 60, 60)){
                    if(mini_button_list[mini_button_index]->function_call != NULL){
                        mini_button_list[mini_button_index]->function_call();
                    };
                };
            };
        };

        if(custom_controller != NULL){
            custom_controller();
        }
    }
}
