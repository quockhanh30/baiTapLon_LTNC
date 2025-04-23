#ifndef DEFS_H
#define DEFS_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

struct thongTinTaiKhoan {
    int soTaiKhoan;
    string matKhau;
    int maOTP;
    int soDu;
    int khoaTaiKhoan;

    thongTinTaiKhoan(int _soTaiKhoan, string _matKhau, int _maOTP, int _soDu, int _khoaTaiKhoan) {
        soTaiKhoan = _soTaiKhoan;
        matKhau = _matKhau;
        maOTP = _maOTP;
        soDu = _soDu;
        khoaTaiKhoan = _khoaTaiKhoan;
    }

    thongTinTaiKhoan& operator+=(int soTienCanChuyen) {
        this->soDu += soTienCanChuyen;
        return *this;
    }

    thongTinTaiKhoan& operator-=(int soTienCanChuyen) {
        this->soDu -= soTienCanChuyen;
        return *this;
    }
};

#endif //DEFS_H

