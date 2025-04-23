#include "trangThaiTaiKhoan.h"

vector<thongTinTaiKhoan> docTrangThaiTaiKhoan() {
    vector<thongTinTaiKhoan> thongTin;
    ifstream myfile("trangThaiTaikhoan.txt");
    int soTaiKhoan;
    string matKhau;
    int maOTP;
    int soDu;
    int khoaTaiKhoan;

    while (myfile >> soTaiKhoan >> matKhau >> maOTP >> soDu >> khoaTaiKhoan) {
        thongTin.push_back(thongTinTaiKhoan(soTaiKhoan, matKhau, maOTP, soDu, khoaTaiKhoan));
    }
    myfile.close();
    return thongTin;
}

void luuTrangThaiTaiKhoan(const vector<thongTinTaiKhoan>& thongTin) {
    ofstream myfile("trangThaiTaikhoan.txt");
    for (const auto& tk : thongTin) {
        myfile << tk.soTaiKhoan << " " << tk.matKhau << " " << tk.maOTP << " " << tk.soDu << " " << tk.khoaTaiKhoan << endl;
    }
    myfile.close();
}

vector<int> docSoToTienTrongMay() {
    vector<int> tienTrongMay;
    ifstream myfile("menhGiaTien.txt");
    int tien1000, tien500, tien200, tien100, tien50, tien20, tien10, tien5, tien2, tien1;

    while (myfile >> tien1000 >> tien500 >> tien200 >> tien100 >> tien50 >> tien20 >> tien10 >> tien5 >> tien2 >> tien1) {
        tienTrongMay.push_back(tien1000);
        tienTrongMay.push_back(tien500);
        tienTrongMay.push_back(tien200);
        tienTrongMay.push_back(tien100);
        tienTrongMay.push_back(tien50);
        tienTrongMay.push_back(tien20);
        tienTrongMay.push_back(tien10);
        tienTrongMay.push_back(tien5);
        tienTrongMay.push_back(tien2);
        tienTrongMay.push_back(tien1);
    }
    myfile.close();
    return tienTrongMay;
}

void luuSoToTienTrongMay(const vector<int>& tienTrongMay) {
    ofstream myfile("menhGiaTien.txt");
    myfile << tienTrongMay[0] << " " << tienTrongMay[1] << " " << tienTrongMay[2]
    << " " << tienTrongMay[3] << " " << tienTrongMay[4] << " " << tienTrongMay[5]
    << " " << tienTrongMay[6] << " " << tienTrongMay[7] << " " << tienTrongMay[8] << " " << tienTrongMay[9];
    myfile.close();
}
