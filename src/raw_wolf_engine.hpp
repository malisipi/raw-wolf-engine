#include <iostream>
#include <functional>
#if __has_include(<filesystem>)
    #include <filesystem>
#else
    #include <experimental/filesystem>
    namespace std {
        namespace filesystem = experimental::filesystem;
    }
#endif
#include <cstddef>
#include <cstring>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    namespace std {
        namespace filesystem = std::__fs::filesystem;
    }
#endif
#ifdef _WIN32
    #include <windows.h>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <raw_wolf_macros.hpp>
#include <raw_wolf_types.hpp>
#include <raw_wolf_details.hpp>
#include <raw_wolf_stack.hpp>
#include <raw_wolf_collision.hpp>
#ifdef DISCORD_CLIENT_ID
#include <raw_wolf_discord.hpp>
#endif

namespace raw_wolf {
    namespace window {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        bool keep_open = false;
        int width = 1280;
        int height = 720;
        std::function<void(MouseEvent)> mouse_handler = NULL; // Will be sended by drawer.hpp
        std::function<void(KeyboardEvent)> keyboard_handler = NULL; // Will be sended keyboard.hpp

        void set_icon(char* icon_path){
            SDL_Surface* icon_surface = IMG_Load(icon_path);
            SDL_SetWindowIcon(window, icon_surface);
            SDL_FreeSurface(icon_surface);
        }
    }

    namespace drawer {
        void init();
        void __mouse_handler (MouseEvent);
        void rect(int, int, int, int, uint);
        void clean_outside();
    }

    namespace text {
        extern TTF_Font* font;
        void create_and_draw(char*, int, int, int, int, uint);
    }

    std::tuple<int, int> get_screen_size(){
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        return std::make_pair(DM.w, DM.h);
    }

    int init(){
        if (SDL_Init(SDL_INIT_VIDEO) != 0){
            std::cout << "SDL_Init failed: " << SDL_GetError() << "\n";
            return 1;
        }

        window::window = SDL_CreateWindow(raw_wolf::details::app_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window::width, window::height, 0);
        window::renderer = SDL_CreateRenderer(window::window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

        #ifdef DISCORD_CLIENT_ID
        discord::init(DISCORD_CLIENT_ID);
        #endif

        return 0;
    }

    void* __loop_return = NULL; // Don't call it directly except return data between scenes

    void* loop(std::function<void(int)>drawer_function){
        window::keep_open = true;
        int is_first_frame = true;
        #if FLAG_FPS_Counter
            float timer_old = SDL_GetTicks();
            float last_fps_updated = SDL_GetTicks();
            int fps = 0;
            int fps_index = 0;
            int total_fps = 0;
            char fps_text[4] = "";
        #endif
        while(window::keep_open){
            SDL_Event event;
            while(SDL_PollEvent(&event) > 0){
                switch(event.type){
                    case SDL_QUIT: {
                        window::keep_open = false;
                        exit(0);
                    }
                    case SDL_MOUSEBUTTONDOWN: {
                        int button = -1;
                        if((int)event.button.button == 2) button = 3;
                        else if((int)event.button.button == 3) button = 2;
                        else button = (int)event.button.button;
                        drawer::__mouse_handler(MouseEvent{
                            .x = event.button.x,
                            .y = event.button.y,
                            .button = button,
                            .click = 1
                        });
                        break;
                    }
                    case SDL_MOUSEBUTTONUP: {
                        int button = -1;
                        if((int)event.button.button == 2) button = 3;
                        else if((int)event.button.button == 3) button = 2;
                        else button = (int)event.button.button;
                        drawer::__mouse_handler(MouseEvent{
                            .x = event.motion.x,
                            .y = event.motion.y,
                            .button = button,
                            .click = 0
                        });
                        break;
                    }
                    case SDL_MOUSEMOTION: {
                        drawer::__mouse_handler(MouseEvent{ // Handler is placed into drawer.hpp, because of raw mouse event needs to be mapped
                            .x = event.motion.x,
                            .y = event.motion.y,
                            .button = -1,
                            .click = -1
                        });
                        break;
                    }
                    case SDL_KEYDOWN: {
                        if(window::keyboard_handler != NULL) window::keyboard_handler(KeyboardEvent{
                            .scancode = event.key.keysym.scancode,
                            .keycode = event.key.keysym.sym,
                            .modifier = event.key.keysym.mod,
                            .press = (int)(event.key.repeat != 0) * -2 + 1 // I probably never remember what i do in later
                        });
                        break;
                    }
                    case SDL_KEYUP: {
                        if(window::keyboard_handler != NULL) window::keyboard_handler(KeyboardEvent{
                            .scancode = event.key.keysym.scancode,
                            .keycode = event.key.keysym.sym,
                            .modifier = event.key.keysym.mod,
                            .press = 0
                        });
                        break;
                    }
                }
            }

            SDL_GetWindowSize(window::window, &window::width, &window::height);
            drawer_function(is_first_frame);
            drawer::clean_outside();
            is_first_frame = false;
            #if FLAG_FPS_Counter
                float timer_now = SDL_GetTicks();
                float timer_delta = timer_now-timer_old;
                timer_old = timer_now;
                if(timer_delta <= 0) timer_delta = 1.f; // To avoid potantial divide by zero
                fps = 1000.f/timer_delta;
                total_fps += fps;
                fps_index++;
                if(timer_now-last_fps_updated>200){
                    last_fps_updated = timer_now;
                    fps = (int)((float)total_fps/fps_index);
                    sprintf(fps_text, "%d", fps);
                    fps_index = 0;
                    total_fps = 0;
                }
                if(text::font != NULL) {
                    drawer::rect(0, 0, 60, 20, 0x00CCAA00);
                    text::draw(fps_text, 10, 2, 40, 16, 0x00000000);
                }
            #endif
            SDL_RenderPresent(window::renderer);
            SDL_UpdateWindowSurface(window::window);
            #ifdef __EMSCRIPTEN__ // Unblocking event loop
                emscripten_sleep(0);
            #endif
            if(__loop_return != NULL){
                void* temp_loop_return = __loop_return;
                __loop_return = NULL;
                return temp_loop_return;
            }
            #ifdef DISCORD_CLIENT_ID
                discord::loop();
            #endif
        };
        return 0;
    }
}

#include <raw_wolf_drawer.hpp>
#include <raw_wolf_text.hpp>
#include <raw_wolf_simple_ui.hpp>
#include <raw_wolf_settings.hpp>
#include <raw_wolf_welcome.hpp>
#include <raw_wolf_audio.hpp>
#include <raw_wolf_tools.hpp>
#include <raw_wolf_networking.hpp>
