/* date = July 2nd 2020 11:52 pm */

#ifndef APP_H
#define APP_H

typedef struct APP_Variables  APP_Variables;
struct APP_Variables
{
    M_Arena arena_;
    M_Arena *arena;
    
    R_Font test_font;
    R_Font test_font_small;
    
    R_RGBATexture test_texture;
};

#endif //APP_H
