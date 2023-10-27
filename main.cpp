#include "crypto/crypto.cpp"
#include "mainwindow.h"
#include <QApplication>

// // void function(Symmetric* sym) {
// //     std::string key = sym->generate_key();
// //     std::cout << key << "\n";
// // }

int main(int argc, char *argv[])
{
    // QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
        // 
    Elliptic asym;
    // Dsa asym;
    // Dsa asym;
    // DiffieHellman asym;
// 
    asym.generate_keys();
    std::string public_key = asym.get_public();
    asym.sign_file("test.txt", "test.txt.signature");
    std::cout << public_key << "\n";
    std::cout << asym.verify_signature("test.txt", "test.txt.signature");
// 
    // 
    std::string text = "text";
    std::string encrypt_text = asym.encrypt(text);
    std::string decrypt_text = asym.decrypt(encrypt_text);

    std::cout << text << "\n";
    std::cout << encrypt_text << "\n";
    std::cout << decrypt_text << "\n";


    // Aria256Cbc sym;

    // std::cout << sym.generate_key();


    // return a.exec();
}

// int main()
// {
//     // OPENSSL_init();
//     // OpenSSL_add_all_algorithms();

//     // build parameters first
//     EVP_PKEY_CTX *ctx_params = EVP_PKEY_CTX_new_id(EVP_PKEY_DSA, NULL);
//     EVP_PKEY_paramgen_init(ctx_params);
//     EVP_PKEY_CTX_set_dsa_paramgen_bits(ctx_params, 1024);

//     EVP_PKEY* pkey_params = NULL;
//     EVP_PKEY_paramgen(ctx_params, &pkey_params);

//     // using parameters, build DSA keypair
//     EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey_params, NULL);
//     EVP_PKEY_keygen_init(ctx);
    
//     EVP_PKEY* pkey = NULL;
//     EVP_PKEY_keygen(ctx, &pkey);

//     // cleanup everything but the final key
//     EVP_PKEY_free(pkey_params);
//     EVP_PKEY_CTX_free(ctx_params);
//     EVP_PKEY_CTX_free(ctx);

//     // TODO: whatever you want with the generator pkey. in this
//     //  example we're just dumping the full unencrypted key to 
//     //  stdout. 
//     // DSA* dsa = EVP_PKEY_get1_DSA(pkey);
//     // BIO* bio = BIO_new_fp(stdout, BIO_NOCLOSE);
//     // PEM_write_bio_DSAPrivateKey(bio, dsa, NULL, NULL, 0, NULL, NULL);
//     // BIO_flush(bio);
//     // DSA_free(dsa);
//     BIO* key_bio = BIO_new(BIO_s_mem());

//     PEM_write_bio_PUBKEY(key_bio, pkey);

//     BUF_MEM* key_buffer_ptr;
//     BIO_get_mem_ptr(key_bio, &key_buffer_ptr);

//     const std::string key_str(key_buffer_ptr->data, key_buffer_ptr->length);

//     BIO_free(key_bio);
//     EVP_PKEY_free(pkey);

//     std::cout << key_str;

//     return 0;
// }