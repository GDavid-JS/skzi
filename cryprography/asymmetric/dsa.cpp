class Dsa : public SignatureManager {
private:
public:
    Dsa() {}

    void generate_keys(const unsigned short size = 2048) override {
        SignatureManager::template_generate_keys(&EVP_PKEY_CTX_set_dsa_paramgen_bits, EVP_PKEY_DSA, size);
    }
};