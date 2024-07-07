namespace raw_wolf {
    typedef struct {
        int x;
        int y;
        int button; // 1: Left Click / 2: Right Click / 3: Middle Click / 4: (Browser) Backward Click / 5: (Browser) Forward Click / -1: None (Motion)
        int click; // 1: Mouse Down / 0: Mouse Up / -1: Motion
    } MouseEvent;

    typedef struct {
        SDL_Scancode scancode;
        SDL_Keycode keycode;
        uint16_t modifier;
        int press; // 1: Press Down / 0: Press Up / -1: Repeat
    } KeyboardEvent;

    typedef struct {
        float font_ratio;
        SDL_Texture* texture;
    } TextTexture;
}
