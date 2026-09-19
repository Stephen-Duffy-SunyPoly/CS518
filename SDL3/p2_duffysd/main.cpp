#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


enum Direction {
    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT
};

struct Pos {
    int x, y;
    bool directionValid(SDL_Surface *s, Direction direction) const {
        if (direction == UP_LEFT || direction == UP || direction == UP_RIGHT) {
            if (y==0) {
                return false;
            }
        }
        if (direction == DOWN_LEFT || direction == DOWN || direction == DOWN_RIGHT) {
            if (y == s->h-1) {
                return false;
            }
        }
        if (direction == UP_LEFT || direction == LEFT || direction == DOWN_LEFT) {
            if (x == 0) {
                return false;
            }
        }
        if (direction == UP_RIGHT || direction == RIGHT || direction == DOWN_RIGHT) {
            if (x == s->w-1) {
                return false;
            }
        }
        return true;
    }
    Pos operator+(const Direction direction) const {
        switch (direction) {
            case UP:
                return {x,y-1};
            case UP_RIGHT:
                return {x+1,y-1};
            case RIGHT:
                return {x+1,y};
            case DOWN_RIGHT:
                return {x+1,y+1};
            case DOWN:
                return {x,y+1};
            case DOWN_LEFT:
                return {x-1,y+1};
            case LEFT:
                return {x-1,y};
            case UP_LEFT:
                return {x-1,y-1};
            default:
                return {x,y};
        }
    }
};

// basically just a 3 byte data type
struct Bit24Color {
    // ReSharper disable once CppDeclaratorNeverUsed
    Uint8 b1,b2,b3;
};

static int imap(const SDL_Surface * s, const int x, const int y) {
    return x+y*s->w;
}

static int imap(const SDL_Surface * s, Pos pos) {
    return imap(s,pos.x,pos.y);
}

static void shuffleOrder(Pos *order, int size) {
    for (int i=0;i<size;i++) {
        int a = SDL_rand(size);
        Pos tmp = order[i];
        order[i] = order[a];
        order[a] = tmp;
    }
}

template <typename T>
void pixleDust(SDL_Surface *image, int numberOfPixels, Pos *workOrder) {
    auto pixels = static_cast<T *>(image->pixels);
    //randomize the order
    for (int i=0;i<numberOfPixels;i++) {
        Pos pos = workOrder[i];
        //choose a random direction to swap in
        auto direction = static_cast<Direction>(SDL_rand(8));
        while (!pos.directionValid(image,direction)) {
            direction = static_cast<Direction>(SDL_rand(8));
        }
        Pos other = pos + direction;

        //swap them
        T tmp = pixels[imap(image,pos)];
        pixels[imap(image,pos)] = pixels[imap(image,other)];
        pixels[imap(image,other)] = tmp;
    }
}

int main(int argc, char* argv[]) {
    constexpr SDL_InitFlags INIT_FLAGS = SDL_INIT_VIDEO;

    if (argc < 2) {
        SDL_Log("Usage: %s <Image File>", argv[0]);
        return 1;
    }

    //SDL initialization things
    if (!SDL_Init(INIT_FLAGS)) {
        SDL_Log("SDL could not initialize! SDL Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface * image;
    ((image = SDL_LoadBMP(argv[1]))) || ((image = SDL_LoadPNG(argv[1])));
    if (image == nullptr) {
        SDL_Log("Error loading Image! SDL Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Prog 2: duffysd",image->w,image->h,0);

    if (window == nullptr) {
        SDL_Log("Window could not be created! SDL Error: %s", SDL_GetError());
        SDL_DestroySurface(image);
        SDL_Quit();
        return 1;
    }

    SDL_Surface * surface = SDL_GetWindowSurface(window);
    if (surface == nullptr) {
        SDL_Log("Error obtaining window surface! SDL Error: %s", SDL_GetError());
        SDL_DestroySurface(image);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // my program initialization things
    //this will hold the order pixels are swapped in
    int numberOfPixels = image->w*image->h;
    Pos * workOrder = new Pos[numberOfPixels];
    //initialize to correct positions
    for (int y=0; y<image->h; y++) {
        for (int x=0; x<image->w; x++) {
            workOrder[imap(image,x,y)] = {x,y};
        }
    }


    SDL_BlitSurface(image,nullptr,surface,nullptr);

    SDL_Event e;
    bool quit = false;
    bool snappening = false;
    int numberSaves = 0;
    char saveNameBuffer[128];
    const SDL_PixelFormatDetails *imageDetails = SDL_GetPixelFormatDetails(image->format);
    int bpp = imageDetails->bits_per_pixel;
    if (bpp != 32 && bpp != 24) {
        SDL_Log("Image Color format has unsupported nuber of bits per color");
    }

    //render loop
    while(!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    quit = true;
                    break;
                case SDL_EVENT_KEY_DOWN: {
                    auto key = e.key.key;
                        if (key == SDLK_ESCAPE) {
                            quit = true;
                        } else if (key == SDLK_SPACE) {
                            snappening = true;
                        } else if (key == SDLK_S) {
                            SDL_snprintf(saveNameBuffer,128,"image%d.png",numberSaves);
                            SDL_SavePNG(image,saveNameBuffer);
                            numberSaves++;
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        if (snappening) {
            shuffleOrder(workOrder, numberOfPixels);
            if (bpp == 32) {
                pixleDust<Uint32>(image, numberOfPixels, workOrder);
            } else if (bpp == 24) {
                pixleDust<Bit24Color>(image, numberOfPixels, workOrder);
            }
            SDL_BlitSurface(image,nullptr,surface,nullptr);
        }
        SDL_UpdateWindowSurface(window);
    }

    //clean up
    delete[] workOrder;
    SDL_DestroyWindow(window);
    SDL_DestroySurface(image);
    SDL_Quit();
    return 0;
}
