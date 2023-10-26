#define SYMMETRIC_CLASS(class_name, size, type_macro) \
class class_name : public Symmetric { \
public: \
    std::string generate_key() override { \
        return Symmetric::template_generate_key(size); \
    } \
    std::string encrypt(const std::string& data) override { \
        return Symmetric::template_encrypt(type_macro, data); \
    } \
    std::string decrypt(const std::string& data) override { \
        return Symmetric::template_decrypt(type_macro, data); \
    } \
};

// Aes
// AES-128
SYMMETRIC_CLASS(Aes128Cbc, 16, EVP_aes_128_cbc)
SYMMETRIC_CLASS(Aes128Cfb, 16, EVP_aes_128_cfb)
SYMMETRIC_CLASS(Aes128Cfb1, 16, EVP_aes_128_cfb1)
SYMMETRIC_CLASS(Aes128Cfb8, 16, EVP_aes_128_cfb8)
SYMMETRIC_CLASS(Aes128Ctr, 16, EVP_aes_128_ctr)
SYMMETRIC_CLASS(Aes128Ecb, 16, EVP_aes_128_ecb)
SYMMETRIC_CLASS(Aes128Ofb, 16, EVP_aes_128_ofb)

// AES-192
SYMMETRIC_CLASS(Aes192Cbc, 24, EVP_aes_192_cbc)
SYMMETRIC_CLASS(Aes192Cfb, 24, EVP_aes_192_cfb)
SYMMETRIC_CLASS(Aes192Cfb1, 24, EVP_aes_192_cfb1)
SYMMETRIC_CLASS(Aes192Cfb8, 24, EVP_aes_192_cfb8)
SYMMETRIC_CLASS(Aes192Ctr, 24, EVP_aes_192_ctr)
SYMMETRIC_CLASS(Aes192Ecb, 24, EVP_aes_192_ecb)
SYMMETRIC_CLASS(Aes192Ofb, 24, EVP_aes_192_ofb)

// AES-256
SYMMETRIC_CLASS(Aes256Cbc, 32, EVP_aes_256_cbc)
SYMMETRIC_CLASS(Aes256Cfb, 32, EVP_aes_256_cfb)
SYMMETRIC_CLASS(Aes256Cfb1, 32, EVP_aes_256_cfb1)
SYMMETRIC_CLASS(Aes256Cfb8, 32, EVP_aes_256_cfb8)
SYMMETRIC_CLASS(Aes256Ctr, 32, EVP_aes_256_ctr)
SYMMETRIC_CLASS(Aes256Ecb, 32, EVP_aes_256_ecb)
SYMMETRIC_CLASS(Aes256Ofb, 32, EVP_aes_256_ofb)

// AES-wrap
SYMMETRIC_CLASS(Aes128Wrap, 32, EVP_aes_128_wrap)
SYMMETRIC_CLASS(Aes192Wrap, 32, EVP_aes_192_wrap)
SYMMETRIC_CLASS(Aes256Wrap, 32, EVP_aes_256_wrap)

// ARIA
// ARIA-128
SYMMETRIC_CLASS(Aria128Cbc, 16, EVP_aria_128_cbc)
SYMMETRIC_CLASS(Aria128Cfb, 16, EVP_aria_128_cfb)
SYMMETRIC_CLASS(Aria128Cfb1, 16, EVP_aria_128_cfb1)
SYMMETRIC_CLASS(Aria128Cfb8, 16, EVP_aria_128_cfb8)
SYMMETRIC_CLASS(Aria128Ctr, 16, EVP_aria_128_ctr)
SYMMETRIC_CLASS(Aria128Ecb, 16, EVP_aria_128_ecb)
SYMMETRIC_CLASS(Aria128Ofb, 16, EVP_aria_128_ofb)

// ARIA-192
SYMMETRIC_CLASS(Aria192Cbc, 24, EVP_aria_192_cbc)
SYMMETRIC_CLASS(Aria192Cfb, 24, EVP_aria_192_cfb)
SYMMETRIC_CLASS(Aria192Cfb1, 24, EVP_aria_192_cfb1)
SYMMETRIC_CLASS(Aria192Cfb8, 24, EVP_aria_192_cfb8)
SYMMETRIC_CLASS(Aria192Ctr, 24, EVP_aria_192_ctr)
SYMMETRIC_CLASS(Aria192Ecb, 24, EVP_aria_192_ecb)
SYMMETRIC_CLASS(Aria192Ofb, 24, EVP_aria_192_ofb)

// ARIA-256
SYMMETRIC_CLASS(Aria256Cbc, 32, EVP_aria_256_cbc)
SYMMETRIC_CLASS(Aria256Cfb, 32, EVP_aria_256_cfb)
SYMMETRIC_CLASS(Aria256Cfb1, 32, EVP_aria_256_cfb1)
SYMMETRIC_CLASS(Aria256Cfb8, 32, EVP_aria_256_cfb8)
SYMMETRIC_CLASS(Aria256Ctr, 32, EVP_aria_256_ctr)
SYMMETRIC_CLASS(Aria256Ecb, 32, EVP_aria_256_ecb)
SYMMETRIC_CLASS(Aria256Ofb, 32, EVP_aria_256_ofb)

// Blowfish
SYMMETRIC_CLASS(BfCbc, 56, EVP_bf_cbc)
SYMMETRIC_CLASS(BfCfb, 56, EVP_bf_cfb)
SYMMETRIC_CLASS(BfEcb, 56, EVP_bf_ecb)
SYMMETRIC_CLASS(BfOfb, 56, EVP_bf_ofb)

// Camellia
// Camellia-128
SYMMETRIC_CLASS(Camellia128Cbc, 16, EVP_camellia_128_cbc)
SYMMETRIC_CLASS(Camellia128Cfb, 16, EVP_camellia_128_cfb)
SYMMETRIC_CLASS(Camellia128Cfb1, 16, EVP_camellia_128_cfb1)
SYMMETRIC_CLASS(Camellia128Cfb8, 16, EVP_camellia_128_cfb8)
SYMMETRIC_CLASS(Camellia128Ctr, 16, EVP_camellia_128_ctr)
SYMMETRIC_CLASS(Camellia128Ecb, 16, EVP_camellia_128_ecb)
SYMMETRIC_CLASS(Camellia128Ofb, 16, EVP_camellia_128_ofb)

// Camellia-192
SYMMETRIC_CLASS(Camellia192Cbc, 24, EVP_camellia_192_cbc)
SYMMETRIC_CLASS(Camellia192Cfb, 24, EVP_camellia_192_cfb)
SYMMETRIC_CLASS(Camellia192Cfb1, 24, EVP_camellia_192_cfb1)
SYMMETRIC_CLASS(Camellia192Cfb8, 24, EVP_camellia_192_cfb8)
SYMMETRIC_CLASS(Camellia192Ctr, 24, EVP_camellia_192_ctr)
SYMMETRIC_CLASS(Camellia192Ecb, 24, EVP_camellia_192_ecb)
SYMMETRIC_CLASS(Camellia192Ofb, 24, EVP_camellia_192_ofb)

// Camellia-256
SYMMETRIC_CLASS(Camellia256Cbc, 32, EVP_camellia_256_cbc)
SYMMETRIC_CLASS(Camellia256Cfb, 32, EVP_camellia_256_cfb)
SYMMETRIC_CLASS(Camellia256Cfb1, 32, EVP_camellia_256_cfb1)
SYMMETRIC_CLASS(Camellia256Cfb8, 32, EVP_camellia_256_cfb8)
SYMMETRIC_CLASS(Camellia256Ctr, 32, EVP_camellia_256_ctr)
SYMMETRIC_CLASS(Camellia256Ecb, 32, EVP_camellia_256_ecb)
SYMMETRIC_CLASS(Camellia256Ofb, 32, EVP_camellia_256_ofb)

// CAST
SYMMETRIC_CLASS(CastCbc, 16, EVP_cast5_cbc)
SYMMETRIC_CLASS(CastCfb, 16, EVP_cast5_cfb)
SYMMETRIC_CLASS(CastEcb, 16, EVP_cast5_ecb)
SYMMETRIC_CLASS(CastOfb, 16, EVP_cast5_ofb)

// Chacha20
SYMMETRIC_CLASS(Chacha20, 32, EVP_chacha20)

// DES
SYMMETRIC_CLASS(DesCbc, 8, EVP_des_cbc)
SYMMETRIC_CLASS(DesCfb, 8, EVP_des_cfb)
SYMMETRIC_CLASS(DesCfb1, 8, EVP_des_cfb1)
SYMMETRIC_CLASS(DesCfb8, 8, EVP_des_cfb8)
SYMMETRIC_CLASS(DesEcb, 8, EVP_des_ecb)
SYMMETRIC_CLASS(DesEde, 8, EVP_des_ede)
SYMMETRIC_CLASS(DesEdeCbc, 8, EVP_des_ede_cbc)
SYMMETRIC_CLASS(DesEdeCfb, 8, EVP_des_ede_cfb)
SYMMETRIC_CLASS(DesEdeEcb, 8, EVP_des_ede_ecb)
SYMMETRIC_CLASS(DesEdeOfb, 8, EVP_des_ede_ofb)
SYMMETRIC_CLASS(DesEde3, 8, EVP_des_ede3)
SYMMETRIC_CLASS(DesEde3Cbc, 8, EVP_des_ede3_cbc)
SYMMETRIC_CLASS(DesEde3Cfb, 8, EVP_des_ede3_cfb)
SYMMETRIC_CLASS(DesEde3Cfb1, 8, EVP_des_ede3_cfb1)
SYMMETRIC_CLASS(DesEde3Cfb8, 8, EVP_des_ede3_cfb8)
SYMMETRIC_CLASS(DesEde3Ecb, 8, EVP_des_ede3_ecb)
SYMMETRIC_CLASS(DesEde3Ofb, 8, EVP_des_ede3_ofb)
SYMMETRIC_CLASS(DesOfb, 8, EVP_des_ofb)

// DESX
SYMMETRIC_CLASS(DesxCbc, 8, EVP_desx_cbc)

// RC2
SYMMETRIC_CLASS(Rc240Cbc, 16, EVP_rc2_40_cbc)
SYMMETRIC_CLASS(Rc264Cbc, 16, EVP_rc2_64_cbc)
SYMMETRIC_CLASS(Rc2Cbc, 16, EVP_rc2_cbc)
SYMMETRIC_CLASS(Rc2Cfb, 16, EVP_rc2_cfb)
SYMMETRIC_CLASS(Rc2Ecb, 16, EVP_rc2_ecb)
SYMMETRIC_CLASS(Rc2Ofb, 16, EVP_rc2_ofb)

// RC4
SYMMETRIC_CLASS(Rc440, 16, EVP_rc4_40)

// SEED
SYMMETRIC_CLASS(SeedCbc, 16, EVP_seed_cbc)
SYMMETRIC_CLASS(SeedCfb, 16, EVP_seed_cfb)
SYMMETRIC_CLASS(SeedEcb, 16, EVP_seed_ecb)
SYMMETRIC_CLASS(SeedOfb, 16, EVP_seed_ofb)

// SM4
SYMMETRIC_CLASS(Sm4Cbc, 16, EVP_sm4_cbc)
SYMMETRIC_CLASS(Sm4Cfb, 16, EVP_sm4_cfb)
SYMMETRIC_CLASS(Sm4Ctr, 16, EVP_sm4_ctr)
SYMMETRIC_CLASS(Sm4Ecb, 16, EVP_sm4_ecb)
SYMMETRIC_CLASS(Sm4Ofb, 16, EVP_sm4_ofb)

