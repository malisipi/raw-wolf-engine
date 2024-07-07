namespace raw_wolf::collision {
    // Will only check X-axis
    // Returns true even Y-axis is not collided
    inline bool is_collided_x_axis(SDL_Rect shape1, SDL_Rect shape2){
        return (shape1.x>shape2.x && shape1.x<shape2.x+shape2.w) || (shape2.x>shape1.x && shape2.x<shape1.x+shape1.w);
    }
    // Will only check Y-axis
    // Returns true even X-axis is not collided
    inline bool is_collided_y_axis(SDL_Rect shape1, SDL_Rect shape2){
        return (shape1.y>shape2.y && shape1.y<shape2.y+shape2.h) || (shape2.y>shape1.y && shape2.y<shape1.y+shape1.h);
    }

    bool is_collided(SDL_Rect shape1, SDL_Rect shape2){
        return is_collided_x_axis(shape1, shape2) && is_collided_y_axis(shape1, shape2);
    }

    // Please sure the objects is collided before calling this function
    // You can use is_collided() for this purpose.
    SDL_Rect get_collided_area(SDL_Rect shape1, SDL_Rect shape2){
        if(shape2.x >= shape1.x){
            // |--------|  <== Shape 1
            // |        |
            // |    |xxx|----| <== Shape 2
            // |    |xxx|    |
            // |----|xxx|    |
            //      |        |
            //      |--------|
            // Cartesian Product: (x2,y2) x (x1+w1,y1+h1)
            if(shape2.y >= shape1.y){
                return SDL_Rect({
                    .x = shape2.x,
                    .y = shape2.y,
                    .w = shape1.x + shape1.w - shape2.x,
                    .h = shape1.y + shape1.h - shape2.y
                });
            }
            /////////////////////////////////
            //
            //      |--------| <== Shape 2
            //      |        |
            // |----|xxx|    |
            // |    |xxx|    |
            // |    |xxx|----|
            // |        |
            // |--------| <== Shape 1
            // Cartesian Product: (x2,y1) x (x1+w1,y2+h2)
            else { // shape2.y < shape1.y
                return SDL_Rect({
                    .x = shape2.x,
                    .y = shape1.y,
                    .w = shape1.x + shape1.w - shape2.x,
                    .h = shape2.y + shape2.h - shape1.y
                });
            }
        }
        // else {
        return get_collided_area(shape2, shape1);
        // }
    }

    std::tuple<int, int> get_mid_point_of_area(SDL_Rect shape){
        return std::make_tuple((shape.x + shape.w/2), (shape.y + shape.h/2));
    } 
}