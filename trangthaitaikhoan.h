#ifndef TRANGTHAITAIKHOAN_H
#define TRANGTHAITAIKHOAN_H

#include "defs.h"

vector<thongTinTaiKhoan> docTrangThaiTaiKhoan();
void luuTrangThaiTaiKhoan(const vector<thongTinTaiKhoan>& thongTin);


vector<int> docSoToTienTrongMay();
void luuSoToTienTrongMay(const vector<int>& viTien);

#endif // TRANGTHAITAIKHOAN_H
