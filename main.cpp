#include "crypto/crypto.cpp"
#include "mainwindow.h"
#include <QApplication>

// // void function(Symmetric* sym) {
// //     std::string key = sym->generate_key();
// //     std::cout << key << "\n";
// // }

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    // Rsa asym;
    // asym.generate_keys();
    // std::cout << asym.get_public();
    // std::cout << asym.get_private();
    // std::cout << asym.generate_self_signed_certificate_X509(1, "David Grigoryan", "example", "example@example.com");

    // asym.sign_file("test.txt", "test.txt.signature");
    // std::cout << public_key << "\n";
    // std::cout << asym.verify_signature("test.txt", "test.txt.signature") << "\n";
    // // 
    // std::string text = "text";
    // std::string encrypt_text = asym.encrypt(text);
    // std::string decrypt_text = asym.decrypt(encrypt_text);

    // std::cout << text << "\n";
    // std::cout << encrypt_text << "\n";
    // std::cout << decrypt_text << "\n";

    return a.exec();
}