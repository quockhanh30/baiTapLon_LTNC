#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "defs.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer, const int width, const int height);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, int x, int y);
bool isInside(int x, int y, SDL_Rect rect);

bool showLoginScreen(string& nhapSoTaiKhoan, string& nhapMatKhau, SDL_Renderer* renderer, TTF_Font* font);
void renderMessage(SDL_Renderer* renderer, TTF_Font* font, const string& message);
void showLockedAccountImage(SDL_Renderer* renderer, const string& imagePath);
void hienThiSoDu(SDL_Renderer* renderer, TTF_Font* font, vector<thongTinTaiKhoan>& thongTin, int indexTaiKhoan);
void chuyenTien(SDL_Renderer* renderer, TTF_Font* font, vector<thongTinTaiKhoan>& thongTin, int indexTaiKhoan);
void rutTien(SDL_Renderer* renderer, TTF_Font* font, vector<thongTinTaiKhoan>& thongTin, int indexTaiKhoan);
void dangXuat(SDL_Renderer* renderer, TTF_Font* font);

void hienThiMenu(SDL_Renderer* renderer, TTF_Font* font, vector<thongTinTaiKhoan>& thongTin, int indexTaiKhoan);

#endif // GRAPHICS_H
