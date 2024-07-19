#include <raw_wolf_engine.hpp>

namespace game {
    namespace var {
        const int size = 50;
        const int speed = 5;
        int x;
        int y;

        void reset(){
            x = (1280 - size)/2;
            y = (720 - size)/2;
        }
    };

    namespace control {
        void move_to(int x, int y){
            var::x = x - var::size/2;
            var::y = y - var::size/2;
        };

        void move_by(int x, int y){
            var::x += x;
            var::y += y;
        }

        void exit(){
            raw_wolf::__loop_return = (void*)1;
        };
    }

    void mouse_handler(raw_wolf::MouseEvent mouse) {
        if(mouse.click == 1 && mouse.button == 1){
            return control::move_to(mouse.x, mouse.y);
        };
    };

    void keyboard_handler(raw_wolf::KeyboardEvent keyboard) {
        switch(keyboard.scancode) {
            case SDL_SCANCODE_Q: {
                if(keyboard.press == 1){
                    return control::exit();
                }
            };
            case SDL_SCANCODE_RIGHT:
            case SDL_SCANCODE_D: {
                return control::move_by(var::speed, 0);
            };
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_A: {
                return control::move_by(-var::speed, 0);
            };
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_S: {
                return control::move_by(0, var::speed);
            };
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_W: {
                return control::move_by(0, -var::speed);
            };
            default: {
                return;
            }
        }
    };

    void loop(bool is_first_frame){
        if(is_first_frame){
            var::reset();
            raw_wolf::drawer::scaling::width = 1280;
            raw_wolf::drawer::scaling::height = 720;
            raw_wolf::window::mouse_handler = mouse_handler;
            raw_wolf::window::keyboard_handler = keyboard_handler;
        };
        raw_wolf::drawer::rect(0, 0, 1280, 720, 0xEFB36F00);
        raw_wolf::drawer::rect(var::x, var::y, var::size, var::size, 0xFFFFFF00);

        static raw_wolf::TextTexture* info_text_1 = raw_wolf::simple_ui::static_text((char*)"Press Q to exit");
        raw_wolf::text::draw(info_text_1, 20, 600, 0, 30);
        static raw_wolf::TextTexture* info_text_2 = raw_wolf::simple_ui::static_text((char*)"Use Arrow/WASD keys to move");
        raw_wolf::text::draw(info_text_2, 20, 635, 0, 30);
        static raw_wolf::TextTexture* info_text_3 = raw_wolf::simple_ui::static_text((char*)"Use mouse to jump");
        raw_wolf::text::draw(info_text_3, 20, 670, 0, 30);
    }
}

int main(){
    raw_wolf::details::app_name = (char*)"App Name";
    raw_wolf::details::theme_color = 0x33333300;
    raw_wolf::init();
    raw_wolf::text::load_font((char*)"./examples/assets/fonts/NotoSans-Medium.ttf");
    raw_wolf::welcome::background_image = raw_wolf::drawer::load_image((char*)"./examples/assets/backgrounds/background1.png");
    raw_wolf::welcome::title = (char*)"Welcome Title";
    raw_wolf::welcome::title_color = 0xFFFFFF00;
    raw_wolf::settings::background_color = 0xEFB36F00;
    for(;;){
        void* state = raw_wolf::loop(raw_wolf::welcome::welcome);
        if(state == (void*)raw_wolf::welcome::returning::new_game){
            state = raw_wolf::loop(game::loop);
        }
    }
    return 0;
}

#include <raw_wolf_windows_loader.hpp>