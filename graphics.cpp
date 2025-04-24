#include "graphics.h"
#include <SDL_image.h>
#include <iostream>
#include <sstream>
#include "trangThaiTaiKhoan.h"
#include <SDL_mixer.h>

using namespace std;

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer, const int width, const int height) {
    SDL_Init(SDL_INIT_VIDEO);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "Khong the khoi tao SDL_mixer! Loi: " << Mix_GetError() << endl;
    }

    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("cay ATM Nam Dinh", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, int x, int y) {
    SDL_Color color = {255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

bool isInside(int x, int y, SDL_Rect rect) {
    return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
}

void renderMessage(SDL_Renderer* renderer, TTF_Font* font, const string& message) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);
    SDL_RenderClear(renderer);

    vector<string> lines;
    stringstream ss(message);
    string line;
    while (getline(ss, line, '\n')) {
        lines.push_back(line);
    }

    int y = 250;
    for (const string& l : lines) {
        renderText(renderer, font, l, 200, y);
        y += 40;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
}


bool showLoginScreen(string& nhapSoTaiKhoan, string& nhapMatKhau, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Rect loginButton = {200, 450, 400, 60};
    SDL_Surface* buttonSurface = IMG_Load("dangnhap.png");
    SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);

    SDL_Rect inputTaiKhoan = {100, 100, 600, 40};
    SDL_Rect inputMatKhau = {100, 200, 600, 40};

    SDL_StartTextInput();
    bool quit = false;
    SDL_Event e;
    enum InputField { NONE, TAIKHOAN, MATKHAU } currentField = TAIKHOAN;
    nhapSoTaiKhoan = "";
    nhapMatKhau = "";

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return false;
            } else if (e.type == SDL_TEXTINPUT) {
                if (currentField == TAIKHOAN) {
                    nhapSoTaiKhoan += e.text.text;
                } else if (currentField == MATKHAU) {
                    nhapMatKhau += e.text.text;
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    if (currentField == TAIKHOAN) {
                        if (!nhapSoTaiKhoan.empty()) {
                            nhapSoTaiKhoan.pop_back();
                        }
                    } else if (currentField == MATKHAU) {
                        if (!nhapMatKhau.empty()) {
                            nhapMatKhau.pop_back();
                        }
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (isInside(x, y, loginButton)) {
                    return true;
                } else if (isInside(x, y, inputTaiKhoan)) {
                    currentField = TAIKHOAN;
                } else if (isInside(x, y, inputMatKhau)) {
                    currentField = MATKHAU;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &inputTaiKhoan);
        SDL_RenderDrawRect(renderer, &inputMatKhau);

        renderText(renderer, font, "NHAP SO TAI KHOAN CUA BAN: " + nhapSoTaiKhoan, inputTaiKhoan.x + 5, inputTaiKhoan.y + 5);
        renderText(renderer, font, "NHAP MAT KHAU CUA BAN: " + string(nhapMatKhau.length(), '*'), inputMatKhau.x + 5, inputMatKhau.y + 5);

        SDL_RenderCopy(renderer, buttonTexture, NULL, &loginButton);

        SDL_RenderPresent(renderer);
    }

    return false;
}

void showLockedAccountImage(SDL_Renderer* renderer, const string& imagePath) {
    SDL_Surface* imgSurface = IMG_Load(imagePath.c_str());
    if (!imgSurface) {
        cout << "Khong the load anh khoa tai khoan\n";
        return;
    }

    SDL_Texture* imgTexture = SDL_CreateTextureFromSurface(renderer, imgSurface);
    SDL_FreeSurface(imgSurface);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    SDL_DestroyTexture(imgTexture);
}

void hienThiSoDu(SDL_Renderer* renderer, TTF_Font* font, vector<thongTinTaiKhoan>& thongTin, int indexTaiKhoan) {
    SDL_Event e;
    bool quayLai = false;

    SDL_Rect nutQuayLaiRect = { 50, 450, 400, 100 };
    SDL_Texture* btnQuayLai = IMG_LoadTexture(renderer, "quaylai.png");

    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "hinhnenTienSoDu.jpg");
    if (!backgroundTexture) {
        cerr << "khong tai duoc hinh nen!" << endl;
        exit(1);
    }

    while (!quayLai) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        string soDuStr = "SO DU HIEN TAI CUA BAN LA: " + to_string(thongTin[indexTaiKhoan].soDu) + "$";
        renderText(renderer, font, soDuStr, 200, 200);

        SDL_RenderCopy(renderer, btnQuayLai, NULL, &nutQuayLaiRect);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                if (x >= nutQuayLaiRect.x && x <= nutQuayLaiRect.x + nutQuayLaiRect.w &&
                    y >= nutQuayLaiRect.y && y <= nutQuayLaiRect.y + nutQuayLaiRect.h) {
                    quayLai = true;
                }
            }
        }
    }

    SDL_DestroyTexture(btnQuayLai);
    SDL_DestroyTexture(backgroundTexture);
}

void chuyenTien(SDL_Renderer* renderer, TTF_Font* font, vector<thongTinTaiKhoan>& thongTin, int indexTaiKhoan) {
    string soTaiKhoanNhan = "", soTienStr = "", otpNhap = "";
    int indexNhan = -1;
    SDL_Event e;
    enum TrangThaiNhap { NHAP_TAIKHOAN, NHAP_SOTIEN, NHAP_OTP, HOANTAT } trangThai = NHAP_TAIKHOAN;

    SDL_Rect nutQuayLaiRect = { 10, 10, 300, 80 };
    SDL_Texture* nenTexture = IMG_LoadTexture(renderer, "hinhnentienChuyenTien.png");

    SDL_StartTextInput();
    int soLanSaiOTP = 0;

    while (trangThai != HOANTAT) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (x >= nutQuayLaiRect.x && x <= nutQuayLaiRect.x + nutQuayLaiRect.w &&
                    y >= nutQuayLaiRect.y && y <= nutQuayLaiRect.y + nutQuayLaiRect.h) {
                    SDL_DestroyTexture(nenTexture);
                    return;
                }
            } else if (e.type == SDL_TEXTINPUT) {
                if (trangThai == NHAP_TAIKHOAN) {
                    soTaiKhoanNhan += e.text.text;
                } else if (trangThai == NHAP_SOTIEN) {
                    soTienStr += e.text.text;
                } else if (trangThai == NHAP_OTP) {
                    otpNhap += e.text.text;
                }
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE) {
                if (trangThai == NHAP_TAIKHOAN) {
                    if (!soTaiKhoanNhan.empty()) {
                        soTaiKhoanNhan.pop_back();
                    }
                } else if (trangThai == NHAP_SOTIEN) {
                    if (!soTienStr.empty()) {
                        soTienStr.pop_back();
                    }
                } else if (trangThai == NHAP_OTP) {
                    if (!otpNhap.empty()) {
                        otpNhap.pop_back();
                    }
                }
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                if (trangThai == NHAP_TAIKHOAN) {
                    for (int i = 0; i < thongTin.size(); i++) {
                        if (to_string(thongTin[i].soTaiKhoan) == soTaiKhoanNhan) {
                            indexNhan = i;
                            trangThai = NHAP_SOTIEN;
                            break;
                        }
                    }
                    if (indexNhan == -1) {
                        renderMessage(renderer, font, "TAI KHOAN KHONG TON TAI!!!");
                    }
                } else if (trangThai == NHAP_SOTIEN) {
                    int soTien = stoi(soTienStr);
                    if (soTien > thongTin[indexTaiKhoan].soDu) {
                        renderMessage(renderer, font, "TAI KHOAN CUA BAN KHONG DU TIEN!!!");
                        SDL_DestroyTexture(nenTexture);
                        return;
                    } else {
                        trangThai = NHAP_OTP;
                    }
                } else if (trangThai == NHAP_OTP) {
                    if (otpNhap == to_string(thongTin[indexTaiKhoan].maOTP)) {
                        int soTien = stoi(soTienStr);
                        thongTin[indexTaiKhoan].soDu -= soTien;
                        thongTin[indexNhan].soDu += soTien;
                        luuTrangThaiTaiKhoan(thongTin);

                        Mix_Chunk* tingting = Mix_LoadWAV("tingtingchuyentien.wav");
                        if (tingting) {
                            Mix_PlayChannel(-1, tingting, 0);
                        } else {
                            cerr << "Khong tai duoc tingtingchuyentien.wav: " << Mix_GetError() << endl;
                        }
                        renderMessage(renderer, font, "CHUYEN TIEN THANH CONG");
                        Mix_FreeChunk(tingting);
                        SDL_Delay(1500);
                        trangThai = HOANTAT;
                    } else {
                        soLanSaiOTP++;
                        otpNhap = "";

                        if (soLanSaiOTP == 3) {
                            thongTin[indexTaiKhoan].khoaTaiKhoan = 0;
                            luuTrangThaiTaiKhoan(thongTin);
                            renderMessage(renderer, font, "TAI KHOAN CUA BAN DA BI KHOA TAM THOI!!!");
                            showLockedAccountImage(renderer, "khoataikhoan.png");
                            SDL_DestroyTexture(nenTexture);
                            exit(0);
                        } else {
                            renderMessage(renderer, font, "BAN DA NHAP SAI MA OTP, VUI LONG NHAP LAI!!!");
                        }
                    }
                }
            }
        }

        if (nenTexture) {
            SDL_RenderCopy(renderer, nenTexture, NULL, NULL);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);
            SDL_RenderClear(renderer);
        }

        if (trangThai == NHAP_TAIKHOAN) {
            renderText(renderer, font, "NHAP SO TAI KHOAN NHAN: " + soTaiKhoanNhan, 50, 100);
        } else if (trangThai == NHAP_SOTIEN) {
            renderText(renderer, font, "NHAP SO TIEN MUON CHUYEN: " + soTienStr, 50, 100);
        } else if (trangThai == NHAP_OTP) {
            renderText(renderer, font, "NHAP MA OTP: " + string(otpNhap.length(), '*'), 50, 100);
        }

        SDL_Texture* btnQuayLai = IMG_LoadTexture(renderer, "quaylai.png");
        if (btnQuayLai) {
            SDL_RenderCopy(renderer, btnQuayLai, NULL, &nutQuayLaiRect);
            SDL_DestroyTexture(btnQuayLai);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
    SDL_DestroyTexture(nenTexture);
}

void rutTien(SDL_Renderer* renderer, TTF_Font* font, vector<thongTinTaiKhoan>& thongTin, int indexTaiKhoan) {
    string soTienStr = "", otpStr = "";
    int trangThai = 0;
    int soLanSaiOTP = 0;
    int soTienCanRut = 0;
    SDL_Event e;
    SDL_StartTextInput();

    SDL_Rect nutQuayLaiRect = { 50, 450, 400, 100 };

    while (trangThai != 2) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x, y = e.button.y;
                if (x >= nutQuayLaiRect.x && x <= nutQuayLaiRect.x + nutQuayLaiRect.w &&
                    y >= nutQuayLaiRect.y && y <= nutQuayLaiRect.y + nutQuayLaiRect.h) {
                    return;
                }
            }

            if (e.type == SDL_TEXTINPUT) {
                if (trangThai == 0) soTienStr += e.text.text;
                else if (trangThai == 1) otpStr += e.text.text;
            }

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    if (trangThai == 0 && !soTienStr.empty()) soTienStr.pop_back();
                    else if (trangThai == 1 && !otpStr.empty()) otpStr.pop_back();
                }

                if (e.key.keysym.sym == SDLK_RETURN) {
                    if (trangThai == 0) {
                        soTienCanRut = stoi(soTienStr);
                        if (soTienCanRut > thongTin[indexTaiKhoan].soDu) {
                            renderMessage(renderer, font, "TAI KHOAN CUA BAN KHONG DU TIEN!!!");
                            return;
                        }
                        trangThai = 1;
                    } else if (trangThai == 1) {
                        if (otpStr == to_string(thongTin[indexTaiKhoan].maOTP)) {
                            vector<int> tienTrongMay = docSoToTienTrongMay();
                            vector<int> menhGiaTien = {1000, 500, 200, 100, 50, 20, 10, 5, 2, 1};
                            string ketQua = "";

                            int soTienConLai = soTienCanRut;
                            for (int i = 0; i < menhGiaTien.size(); ++i) {
                                int soTo = soTienConLai / menhGiaTien[i];
                                if (soTo > 0 && tienTrongMay[i] >= soTo) {
                                    ketQua += "BAN DA NHAN DUOC " + to_string(soTo) + " TO " + to_string(menhGiaTien[i]) + "$\n";
                                    tienTrongMay[i] -= soTo;
                                    soTienConLai %= menhGiaTien[i];
                                }
                            }

                            thongTin[indexTaiKhoan].soDu -= soTienCanRut;
                            luuTrangThaiTaiKhoan(thongTin);
                            luuSoToTienTrongMay(tienTrongMay);

                            Mix_Chunk* tingting = Mix_LoadWAV("tingtingchuyentien.wav");
                            if (tingting) {
                                Mix_PlayChannel(-1, tingting, 0);
                            } else {
                                cerr << "Khong tai duoc tingtingchuyentien.wav: " << Mix_GetError() << endl;
                            }
                            renderMessage(renderer, font, "RUT TIEN THANH CONG!!!");
                            Mix_FreeChunk(tingting);
                            SDL_Delay(1500);
                            renderMessage(renderer, font, ketQua);
                            trangThai = 2;
                        } else {
                            soLanSaiOTP++;
                            otpStr = "";
                            if (soLanSaiOTP == 3) {
                                thongTin[indexTaiKhoan].khoaTaiKhoan = 0;
                                luuTrangThaiTaiKhoan(thongTin);
                                renderMessage(renderer, font, "TAI KHOAN CUA BAN DA BI KHOA TAM THOI!!!");
                                showLockedAccountImage(renderer, "khoataikhoan.png");
                                exit(0);
                            } else {
                                renderMessage(renderer, font, "BAN DA NHAP SAI MA OTP, VUI LONG NHAP LAI!!");
                            }
                        }
                    }
                }
            }
        }

        SDL_Surface* nenSurface = IMG_Load("hinhnentienRutTien.jpeg");
        SDL_Texture* nenTexture = nenSurface ? SDL_CreateTextureFromSurface(renderer, nenSurface) : nullptr;
        if (nenTexture) {
            SDL_RenderCopy(renderer, nenTexture, NULL, NULL);
            SDL_DestroyTexture(nenTexture);
        }
        if (nenSurface) SDL_FreeSurface(nenSurface);

        if (trangThai == 0)
            renderText(renderer, font, "NHAP SO TIEN MUON RUT: " + soTienStr, 50, 100);
        else if (trangThai == 1)
            renderText(renderer, font, "NHAP MA OTP: " + string(otpStr.length(), '*'), 50, 100);

        SDL_Texture* btnQuayLai = IMG_LoadTexture(renderer, "quaylai.png");
        if (btnQuayLai) {
            SDL_RenderCopy(renderer, btnQuayLai, NULL, &nutQuayLaiRect);
            SDL_DestroyTexture(btnQuayLai);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
}

void dangXuat(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "hengaplai.jpg");
    if (!backgroundTexture) {
        cerr << "Khong tai duoc hengaplai.jpg: " << IMG_GetError() << endl;
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_RenderPresent(renderer);


    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_Delay(10);
    }

    SDL_DestroyTexture(backgroundTexture);
    exit(0);
}


void hienThiMenu(SDL_Renderer* renderer, TTF_Font* font, vector<thongTinTaiKhoan>& thongTin, int indexTaiKhoan) {
    SDL_Event e;
    bool quit = false;

    SDL_Rect xemSoDuRect = { 100, 100, 400, 100 };
    SDL_Rect chuyenTienRect = { 100, 220, 400, 100 };
    SDL_Rect rutTienRect = { 100, 340, 400, 100 };
    SDL_Rect dangXuatRect = { 100, 460, 400, 100 };

    SDL_Texture* background = IMG_LoadTexture(renderer, "hinhnentien.jpg");

    while (!quit) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (background) SDL_RenderCopy(renderer, background, NULL, NULL);

        SDL_Texture* xemSoDuImg = IMG_LoadTexture(renderer, "kiemtrasodu.png");
        SDL_Texture* chuyenTienImg = IMG_LoadTexture(renderer, "chuyentien.png");
        SDL_Texture* rutTienImg = IMG_LoadTexture(renderer, "ruttien.png");
        SDL_Texture* dangXuatImg = IMG_LoadTexture(renderer, "dangxuat.png");

        SDL_RenderCopy(renderer, xemSoDuImg, NULL, &xemSoDuRect);
        SDL_RenderCopy(renderer, chuyenTienImg, NULL, &chuyenTienRect);
        SDL_RenderCopy(renderer, rutTienImg, NULL, &rutTienRect);
        SDL_RenderCopy(renderer, dangXuatImg, NULL, &dangXuatRect);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                if (isInside(x, y, xemSoDuRect)) {
                    hienThiSoDu(renderer, font, thongTin, indexTaiKhoan);
                } else if (isInside(x, y, chuyenTienRect)) {
                    chuyenTien(renderer, font, thongTin, indexTaiKhoan);
                } else if (isInside(x, y, rutTienRect)) {
                    rutTien(renderer, font, thongTin, indexTaiKhoan);
                } else if (isInside(x, y, dangXuatRect)) {
                    dangXuat(renderer, font);
                }
            }
        }

        SDL_DestroyTexture(xemSoDuImg);
        SDL_DestroyTexture(chuyenTienImg);
        SDL_DestroyTexture(rutTienImg);
        SDL_DestroyTexture(dangXuatImg);
    }

    SDL_DestroyTexture(background);
}

