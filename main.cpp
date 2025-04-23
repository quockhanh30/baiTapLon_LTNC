#include "graphics.h"
#include "trangThaiTaiKhoan.h"
#include <iostream>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

int main() {
    string nhapSoTaiKhoan, nhapMatKhau;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);

    int lanSai = 0;
    bool daDangNhap = false;

    vector<thongTinTaiKhoan> thongTin = docTrangThaiTaiKhoan();
    int index = -1;

    while (!daDangNhap && lanSai < 5) {
        if (!showLoginScreen(nhapSoTaiKhoan, nhapMatKhau, renderer, font)) {
            quitSDL(window, renderer);
            return 0;
        }

        int soTK = stoi(nhapSoTaiKhoan);
        index = -1;
        for (int i = 0; i < thongTin.size(); i++) {
            if (thongTin[i].soTaiKhoan == soTK) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            renderMessage(renderer, font, "TAI KHOAN CHUA DUOC MO!!!");
            continue;
        }

        if (thongTin[index].khoaTaiKhoan == 0) {
            renderMessage(renderer, font, "TAI KHOAN CUA BAN DA BI KHOA TAM THOI!!!");
            showLockedAccountImage(renderer, "khoataikhoan.png");
            quitSDL(window, renderer);
            return 0;
        }

        if (nhapMatKhau == thongTin[index].matKhau) {
            daDangNhap = true;
            break;
        } else {
            lanSai++;
            renderMessage(renderer, font, "BAN DA NHAP SAI MAT KHAU!!!");
        }
    }

    if (lanSai >= 5 && index != -1) {
        thongTin[index].khoaTaiKhoan = 0;
        luuTrangThaiTaiKhoan(thongTin);
        renderMessage(renderer, font, "TAI KHOAN CUA BAN DA BI KHOA TAM THOI!!!");
        showLockedAccountImage(renderer, "khoataikhoan.png");
        quitSDL(window, renderer);
        return 0;
    }

    hienThiMenu(renderer, font, thongTin, index);
    quitSDL(window, renderer);

    return 0;
}
