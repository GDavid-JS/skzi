class Rsa : public EncryptionManager, public SignatureManager {
private:
public:
    Rsa() {}

    void generate_keys(const unsigned short size = 2048) override {
        EncryptionManager::template_generate_keys(&EVP_PKEY_CTX_set_rsa_keygen_bits, EVP_PKEY_RSA, size);
    }
};
