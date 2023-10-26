#include "cryprography/cryprography.cpp"
#include "mainwindow.h"
#include <QApplication>

void function(Symmetric* sym) {
    std::string key = sym->generate_key();
    std::cout << key << "\n";
}

int main(int argc, char *argv[])
{
    // QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    // return a.exec();
    
    
    // Rsa rsa;
    // rsa.generate_keys();
    // std::string public_key = rsa.get_public();
    // // rsa.sign_file("test.txt", "test.txt.signature");
    // std::cout << public_key << "\n";
    // std::cout << rsa.verify_signature("test.txt", "test.txt.signature");

    BfCbc sym;
    
    function(&sym);

    std::cout << sym.get_key() << "\n";
    // std::string text = "text";
    // std::string encrypt_text = sym.encrypt(text);
    // std::string decrypt_text = sym.decrypt(encrypt_text);


    // std::cout << text << "\n";
    // std::cout << encrypt_text << "\n";
    // std::cout << decrypt_text << "\n";


    return 0;
}