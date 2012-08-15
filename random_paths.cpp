#include <string.h>
#include <SDL/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

unsigned int random_colour(void);

int main(int argc, char *argv[]) {

    const unsigned int pixel_height = 800;
    const unsigned int pixel_width  = 1280;

    unsigned int pixel_map[pixel_height * pixel_width];
    memset(pixel_map, 0, sizeof(pixel_map) );

    string mode = "blue";
    bool mode_flipper = true; 

    SDL_Surface *screen;
    
    if(SDL_Init(SDL_INIT_VIDEO)<0) {
        cout << "Failed SDL_Init " << SDL_GetError() << endl;
        return 1;
    }
    
    screen=SDL_SetVideoMode(pixel_width,pixel_height,32,SDL_SWSURFACE);
    
    if(screen==NULL) {
        cout << "Failed SDL_SetVideoMode: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    unsigned int start_x = 0;
    unsigned int start_y = pixel_height / 2;

    vector<unsigned int> new_xs;
    new_xs.push_back(start_x);
    
    vector<unsigned int> new_ys;
    new_ys.push_back(start_y);
    
    pixel_map[start_x + pixel_width * start_y] = random_colour();

    time_t time = time_t();
    srand(time);

    for(;;) {
        SDL_Flip(screen);
        SDL_LockSurface(screen);

        if( rand() % 100 > 50) {
            unsigned int branch_index = rand() % new_ys.size(); 
            new_ys.push_back( new_ys[ branch_index ] );
            new_xs.push_back( new_xs[ branch_index ] );
        }
        
        for(size_t i = 0; i < new_ys.size(); i++) {
            
            new_xs[i]+= rand() % 4;
            if(new_xs[i] == pixel_width) {
                //return 0; 
                finish_up(); 
            }
            
            new_ys[i] += rand() % 11 - 5;
    
            if(new_ys[i] > pixel_height - 10) {
                new_ys[i]-=10;
            }
            if(new_ys[i] < 10) {
                new_ys[i]+=10;
            }
      
            if(mode_flipper) {
                pixel_map[new_xs[i] + pixel_width * new_ys[i] ] = random_colour();
            }
            else {
                pixel_map[new_xs[i] + pixel_width * new_ys[i] ] = 0x000000FF;
            } 
        }
        
        if(mode == "random_colour") {
            mode_flipper = true;
        }
        else if (mode == "blue") {
            mode_flipper = false; 
        }
        else {
            if(mode_flipper) {
                mode_flipper = false;
            }
            else {
                mode_flipper = true;
            }
        }

        for(int x = 0; x < pixel_width; x++) {
            for(int y = 0; y < pixel_height; y++) {
                
                int bpp = screen->format->BytesPerPixel;
                Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
                if((x>screen->w) || 
                    (y>screen->h) || 
                    (x<0)         ||
                    (y<0)) {
                        return -1;
                    } 
                        
                *(Uint32 *)p = pixel_map[x + pixel_width*y];
            } 
        } 

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.key.keysym.sym == SDLK_c) { 
                SDL_FillRect(screen,NULL,0); 
            }
            else if(event.key.keysym.sym == SDLK_q) { 
                SDL_Quit();
                return 0; 
            }
            //mode 
            else if(event.key.keysym.sym == SDLK_r) { 
                mode = "random_colour"; 
            }
            else if(event.key.keysym.sym == SDLK_b) { 
                mode = "blue"; 
            }
            else if(event.key.keysym.sym == SDLK_a) { 
                mode = "alternating"; 
            }
        }
    
        SDL_UnlockSurface(screen);
    }
    SDL_Quit();
    
    return 0;
}

unsigned int random_colour(void) {
    unsigned int pixel = 0x0;
    Uint8 alpha = rand() % 128;
    Uint8 red = rand() % 128;
    Uint8 green = rand() % 128;
    Uint8 blue = rand() % 128;
    
    pixel += 0x00 << 24;
    pixel += red << 16;
    pixel += green << 8;
    pixel += blue;
    return pixel;
}

