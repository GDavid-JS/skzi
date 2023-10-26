class Aes128 : public Symmetric {
private:
public:
    std::string generate_key() override {
        return Symmetric::template_generate_key(24);
    }

    std::string encrypt(const std::string& data) override {
        return Symmetric::template_encrypt(EVP_aes_192_cbc, data);
    }

    std::string decrypt(const std::string& data) override {
        return Symmetric::template_decrypt(EVP_aes_192_cbc, data);
    }
};