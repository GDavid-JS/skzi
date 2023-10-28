class Rsa: public SignatureManager, public EncryptionManager {
public:
    void generate_keys() override {
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        EVP_PKEY_keygen_init(ctx);
        EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
        EVP_PKEY_keygen(ctx, &key);
        EVP_PKEY_CTX_free(ctx);
    }
};