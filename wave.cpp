#include <string.h>
#include <SDL/SDL.h>
#include <iostream>
#include <cmath>

using namespace std;
    
int main(int argc, char *argv[]) {
    const unsigned int pixel_height = 400;
    const unsigned int pixel_width  = 800;

    double period = 0.05;
    double iter   = 0.0;

    int width = 10;

    string mode = "sin";

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
    
    unsigned int colour_change = 0;
    for(;;) {
        SDL_Flip(screen);
        SDL_LockSurface(screen);
        int x = 0; 
        int y = 0; 
        int test_val; 

        unsigned int pixel;

        for(x = 0; x < pixel_width; ++x) {
            
            if(mode == "sin") { 
                test_val = 200 + 100*sin(iter + static_cast<double>(x)*period);     
            } 
            else if(mode == "cos") { 
                test_val = 200 + 100*cos(iter + static_cast<double>(x)*period);     
            } 
            else if(mode == "tan") { 
                test_val = 200 + 100*tan(iter + static_cast<double>(x)*period);     
            } 
            
            for(y = 0; y < pixel_height; ++y) {
                if(test_val < y + width && test_val > y - width) { 
                    pixel = static_cast<unsigned int>(x)*1 + colour_change;
                }
                else {
                    pixel = 0;
                }
                int bpp = screen->format->BytesPerPixel;
                Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
                if((x>screen->w) || 
                   (y>screen->h) || 
                   (x<0)         ||
                   (y<0)) {
                    return -1;
                } 
                *(Uint32 *)p = pixel;
            }
        }

        iter += 0.1;
        colour_change = colour_change + width; 

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.key.keysym.sym == SDLK_c) { 
                SDL_FillRect(screen,NULL,0); 
            }
            else if(event.key.keysym.sym == SDLK_q) { 
                SDL_Quit();
                return 0; 
            }
            else if(event.key.keysym.sym == SDLK_j) { 
                SDL_FillRect(screen,NULL,0); 
                if(period > 0.001) { 
                    period -= 0.001; 
                } 
            }
            else if(event.key.keysym.sym == SDLK_k) { 
                SDL_FillRect(screen,NULL,0); 
                period += 0.001; 
            }
            //mode 
            else if(event.key.keysym.sym == SDLK_t) { 
                SDL_FillRect(screen,NULL,0); 
                mode = "tan"; 
            }
            else if(event.key.keysym.sym == SDLK_o) { 
                SDL_FillRect(screen,NULL,0); 
                mode = "cos"; 
            }
            else if(event.key.keysym.sym == SDLK_s) { 
                SDL_FillRect(screen,NULL,0); 
                mode = "sin"; 
            }
       
        
        }
    
        SDL_UnlockSurface(screen);
        usleep(50000); 
    }
    SDL_Quit();
    
    return 0;
}

