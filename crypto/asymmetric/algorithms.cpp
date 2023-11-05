#define RSA_CLASS(class_name, size) \
class class_name: public Signature, public Encryption { \
public: \
    void generate_keys() override { \
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr); \
        if (!ctx) { \
            throw std::runtime_error("Не удалось создать контекст генерации ключей RSA"); \
        } \
        \
        if (EVP_PKEY_keygen_init(ctx) <= 0) { \
            EVP_PKEY_CTX_free(ctx); \
            throw std::runtime_error("Не удалось инициализировать контекст генерации ключей RSA"); \
        } \
        \
        if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, size) <= 0) { \
            EVP_PKEY_CTX_free(ctx); \
            throw std::runtime_error("Не удалось установить размер ключей RSA"); \
        } \
        \
        if (EVP_PKEY_keygen(ctx, &key) <= 0) { \
            EVP_PKEY_CTX_free(ctx); \
            throw std::runtime_error("Не удалось сгенерировать ключи RSA"); \
        } \
        \
        EVP_PKEY_CTX_free(ctx); \
    } \
};

#define DSA_CLASS(class_name, size) \
class class_name: public Signature { \
public: \
    void generate_keys() override { \
        EVP_PKEY_CTX *ctx_params = EVP_PKEY_CTX_new_id(EVP_PKEY_DSA, NULL); \
        if (!ctx_params) { \
            throw std::runtime_error("Не удалось создать контекст параметров ключей DSA"); \
        } \
        \
        if (EVP_PKEY_paramgen_init(ctx_params) <= 0) { \
            EVP_PKEY_CTX_free(ctx_params); \
            throw std::runtime_error("Не удалось инициализировать контекст параметров ключей DSA"); \
        } \
        \
        if (EVP_PKEY_CTX_set_dsa_paramgen_bits(ctx_params, size) <= 0) { \
            EVP_PKEY_CTX_free(ctx_params); \
            throw std::runtime_error("Не удалось установить размер ключей DSA"); \
        } \
        \
        EVP_PKEY* pkey_params = NULL; \
        if (EVP_PKEY_paramgen(ctx_params, &pkey_params) <= 0) { \
            EVP_PKEY_CTX_free(ctx_params); \
            throw std::runtime_error("Не удалось сгенерировать параметры ключей DSA"); \
        } \
        \
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey_params, NULL); \
        if (!ctx) { \
            EVP_PKEY_free(pkey_params); \
            EVP_PKEY_CTX_free(ctx_params); \
            throw std::runtime_error("Не удалось создать контекст генерации ключей DSA"); \
        } \
        \
        if (EVP_PKEY_keygen_init(ctx) <= 0) { \
            EVP_PKEY_free(pkey_params); \
            EVP_PKEY_CTX_free(ctx); \
            EVP_PKEY_CTX_free(ctx_params); \
            throw std::runtime_error("Не удалось инициализировать контекст генерации ключей DSA"); \
        } \
        \
        if (EVP_PKEY_keygen(ctx, &key) <= 0) { \
            EVP_PKEY_free(pkey_params); \
            EVP_PKEY_CTX_free(ctx); \
            EVP_PKEY_CTX_free(ctx_params); \
            throw std::runtime_error("Не удалось сгенерировать ключи DSA"); \
        } \
        \
        EVP_PKEY_free(pkey_params); \
        \
        EVP_PKEY_CTX_free(ctx_params); \
        \
        EVP_PKEY_CTX_free(ctx); \
    } \
};


class RsaPss: public Signature, public Encryption {
public:
    void generate_keys() override {
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        if (!ctx) {
            throw std::runtime_error("Не удалось создать контекст генерации ключей RSA");
        }

        if (EVP_PKEY_keygen_init(ctx) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Не удалось инициализировать контекст генерации ключей RSA");
        }

        if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PSS_PADDING) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Не удалось установить отступ для RSA-PSS");
        }

        if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Не удалось установить алгоритм хеширования для RSA-PSS");
        }

        if (EVP_PKEY_CTX_set_rsa_pss_saltlen(ctx, -1) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Не удалось установить длину соли для RSA-PSS");
        }

        if (EVP_PKEY_keygen(ctx, &key) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Не удалось сгенерировать ключи RSA-PSS");
        }

        EVP_PKEY_CTX_free(ctx);
    }
};

class Ecdsa: public Signature {
public:
    void generate_keys() override {
        EVP_PKEY_CTX *ctx_params = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
        if (!ctx_params) {
            throw std::runtime_error("Не удалось создать контекст параметров ключей ECDSA");
        }

        if (EVP_PKEY_paramgen_init(ctx_params) <= 0) {
            EVP_PKEY_CTX_free(ctx_params);
            throw std::runtime_error("Не удалось инициализировать контекст параметров ключей ECDSA");
        }

        if (EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx_params, NID_X9_62_prime256v1) <= 0) {
            EVP_PKEY_CTX_free(ctx_params);
            throw std::runtime_error("Не удалось установить кривую для ECDSA");
        }

        EVP_PKEY* pkey_params = NULL;
        if (EVP_PKEY_paramgen(ctx_params, &pkey_params) <= 0) {
            EVP_PKEY_CTX_free(ctx_params);
            throw std::runtime_error("Не удалось сгенерировать параметры ключей ECDSA");
        }

        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey_params, NULL);
        if (!ctx) {
            EVP_PKEY_free(pkey_params);
            EVP_PKEY_CTX_free(ctx_params);
            throw std::runtime_error("Не удалось создать контекст генерации ключей ECDSA");
        }

        if (EVP_PKEY_keygen_init(ctx) <= 0) {
            EVP_PKEY_free(pkey_params);
            EVP_PKEY_CTX_free(ctx);
            EVP_PKEY_CTX_free(ctx_params);
            throw std::runtime_error("Не удалось инициализировать контекст генерации ключей ECDSA");
        }

        if (EVP_PKEY_keygen(ctx, &key) <= 0) {
            EVP_PKEY_free(pkey_params);
            EVP_PKEY_CTX_free(ctx);
            EVP_PKEY_CTX_free(ctx_params);
            throw std::runtime_error("Не удалось сгенерировать ключи ECDSA");
        }

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

DSA_CLASS(Dsa1024, 1024)
DSA_CLASS(Dsa2048, 2048)
DSA_CLASS(Dsa3072, 3072)
DSA_CLASS(Dsa4096, 4096)
DSA_CLASS(Dsa5120, 5120)
DSA_CLASS(Dsa6144, 6144)
DSA_CLASS(Dsa7168, 7168)
DSA_CLASS(Dsa8192, 8192)