#define RSA_CLASS(class_name, size) \
class class_name: public SignatureManager, public EncryptionManager { \
public: \
    void generate_keys() override { \
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr); \
        EVP_PKEY_keygen_init(ctx); \
        EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, size); \
        EVP_PKEY_keygen(ctx, &key); \
        EVP_PKEY_CTX_free(ctx); \
    } \
};

#define DSA_CLASS(class_name, size) \
class class_name: public SignatureManager { \
public: \
    void generate_keys() override { \
        EVP_PKEY_CTX *ctx_params = EVP_PKEY_CTX_new_id(EVP_PKEY_DSA, NULL); \
        EVP_PKEY_paramgen_init(ctx_params); \
        EVP_PKEY_CTX_set_dsa_paramgen_bits(ctx_params, size); \
        \
        EVP_PKEY* pkey_params = NULL; \
        EVP_PKEY_paramgen(ctx_params, &pkey_params); \
        \
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey_params, NULL); \
        EVP_PKEY_keygen_init(ctx); \
        \
        EVP_PKEY_keygen(ctx, &key); \
        \
        EVP_PKEY_free(pkey_params); \
        EVP_PKEY_CTX_free(ctx_params); \
        EVP_PKEY_CTX_free(ctx); \
    } \
};

class RsaPss: public SignatureManager, public EncryptionManager {
public:
    void generate_keys() {
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        EVP_PKEY_keygen_init(ctx);
        
        EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PSS_PADDING);
        
        EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256());
        EVP_PKEY_CTX_set_rsa_pss_saltlen(ctx, -1);

        EVP_PKEY_keygen(ctx, &key);
        
        EVP_PKEY_CTX_free(ctx);
    }
};

class Ecdsa: public SignatureManager {
public:
    void generate_keys() override {
        EVP_PKEY_CTX *ctx_params = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
        EVP_PKEY_paramgen_init(ctx_params);
        EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx_params, NID_X9_62_prime256v1);

        EVP_PKEY* pkey_params = NULL;
        EVP_PKEY_paramgen(ctx_params, &pkey_params);

        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey_params, NULL);
        EVP_PKEY_keygen_init(ctx);
        
        EVP_PKEY_keygen(ctx, &key);

        EVP_PKEY_free(pkey_params);
        EVP_PKEY_CTX_free(ctx_params);
        EVP_PKEY_CTX_free(ctx);
    }
};

RSA_CLASS(Rsa512, 512)
RSA_CLASS(Rsa1024, 1024)
RSA_CLASS(Rsa2048, 2048)
RSA_CLASS(Rsa4096, 4096)
RSA_CLASS(Rsa8192, 8192)
RSA_CLASS(Rsa16384, 16384)

DSA_CLASS(Dsa512, 512)
DSA_CLASS(Dsa1024, 1024)
DSA_CLASS(Dsa2048, 2048)
DSA_CLASS(Dsa4096, 4096)
DSA_CLASS(Dsa8192, 8192)
DSA_CLASS(Dsa16384, 16384)