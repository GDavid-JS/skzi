#include <openssl/rand.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/ec.h>
#include <openssl/dsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/aes.h>

//base
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <functional>
#include <initializer_list>
#include <thread>

// assymetric
#include "asymmetric/asymmetric.cpp"
#include "asymmetric/EncryptionManager.cpp"
#include "asymmetric/SignatureManager.cpp"
#include "asymmetric/rsa.cpp"
#include "asymmetric/dsa.cpp"

// symetric
#include "symmetric/symmetric.cpp"
#include "symmetric/aes/128/EVP_aes_128_cbc.cpp"
#include "symmetric/aes/128/EVP_aes_128_cfb.cpp"
#include "symmetric/aes/128/EVP_aes_128_cfb1.cpp"
#include "symmetric/aes/128/EVP_aes_128_cfb8.cpp"
#include "symmetric/aes/128/EVP_aes_128_ctr.cpp"
#include "symmetric/aes/128/EVP_aes_128_ecb.cpp"
#include "symmetric/aes/128/EVP_aes_128_ofb.cpp"
#include "symmetric/aria128.cpp"


// Макросы для AES-128 в разных режимах
const EVP_CIPHER* aes_128_cbc = EVP_aes_128_cbc();
const EVP_CIPHER* aes_128_cfb = EVP_aes_128_cfb();
const EVP_CIPHER* aes_128_cfb1 = EVP_aes_128_cfb1();
const EVP_CIPHER* aes_128_cfb8 = EVP_aes_128_cfb8();
const EVP_CIPHER* aes_128_ctr = EVP_aes_128_ctr();
const EVP_CIPHER* aes_128_ecb = EVP_aes_128_ecb();
const EVP_CIPHER* aes_128_ofb = EVP_aes_128_ofb();

// Макросы для AES-192 в разных режимах
const EVP_CIPHER* aes_192_cbc = EVP_aes_192_cbc();
const EVP_CIPHER* aes_192_cfb = EVP_aes_192_cfb();
const EVP_CIPHER* aes_192_cfb1 = EVP_aes_192_cfb1();
const EVP_CIPHER* aes_192_cfb8 = EVP_aes_192_cfb8();
const EVP_CIPHER* aes_192_ctr = EVP_aes_192_ctr();
const EVP_CIPHER* aes_192_ecb = EVP_aes_192_ecb();
const EVP_CIPHER* aes_192_ofb = EVP_aes_192_ofb();

// Макросы для AES-256 в разных режимах
const EVP_CIPHER* aes_256_cbc = EVP_aes_256_cbc();
const EVP_CIPHER* aes_256_cfb = EVP_aes_256_cfb();
const EVP_CIPHER* aes_256_cfb1 = EVP_aes_256_cfb1();
const EVP_CIPHER* aes_256_cfb8 = EVP_aes_256_cfb8();
const EVP_CIPHER* aes_256_ctr = EVP_aes_256_ctr();
const EVP_CIPHER* aes_256_ecb = EVP_aes_256_ecb();
const EVP_CIPHER* aes_256_ofb = EVP_aes_256_ofb();

// Макросы для AES с разными ключами и режимами обертки
const EVP_CIPHER* aes_128_wrap = EVP_aes_128_wrap();
const EVP_CIPHER* aes_192_wrap = EVP_aes_192_wrap();
const EVP_CIPHER* aes_256_wrap = EVP_aes_256_wrap();