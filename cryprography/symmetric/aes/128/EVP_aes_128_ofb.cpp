class Aes128ofb : public Symmetric {
private:
public:
    std::string generate_key() override {
        return Symmetric::template_generate_key(16);
    }

    std::string encrypt(const std::string& data) override {
        return Symmetric::template_encrypt(EVP_aes_128_ofb, data);
    }

    std::string decrypt(const std::string& data) override {
        return Symmetric::template_decrypt(EVP_aes_128_ofb, data);
    }
};