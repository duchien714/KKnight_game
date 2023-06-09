#ifndef RENDERWINDOW_H_INCLUDED
#define RENDERWINDOW_H_INCLUDED

#include "Header.h"

class RenderWindow
{
public:
    bool initWindow();
    void quitSDL();
    void clearScreen();
    void display();

    void logSDLError(ostream& os, const string &msg, bool fatal);

    SDL_Texture* loadIMG(const string &filePath);
    void render(SDL_Texture *mTexture = nullptr, const int &x = 0, const int &y = 0, const int &realWidth = 0, const int &realHeight = 0, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE );
    void renderB(SDL_Texture *mTexture = nullptr, SDL_Rect* box = nullptr, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE );
    SDL_Renderer* getRenderer() const
    {
        return m_renderer;
    };
    void renderBox(SDL_Rect* box);
    void renderText(const char *text, TTF_Font *font, int x, int y, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255);

protected:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
};

#endif // RENDERWINDOW_H_INCLUDED
