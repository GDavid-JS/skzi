class EncryptionManager: virtual public Asymmetric {
public:
    virtual std::string encrypt(const std::string& text) {
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(this->key, nullptr);
        EVP_PKEY_encrypt_init(ctx);

        size_t ciphertext_len;
        EVP_PKEY_encrypt(ctx, nullptr, &ciphertext_len, reinterpret_cast<const unsigned char*>(text.c_str()), text.size());

        std::vector<unsigned char> ciphertext(ciphertext_len);
        EVP_PKEY_encrypt(ctx, ciphertext.data(), &ciphertext_len, reinterpret_cast<const unsigned char*>(text.c_str()), text.size());

        EVP_PKEY_CTX_free(ctx);

        std::string encrypted_text(reinterpret_cast<const char*>(ciphertext.data()), ciphertext_len);

        return encrypted_text;
    }

    virtual std::string decrypt(const std::string& ciphertext) {
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(this->key, nullptr);
        
        EVP_PKEY_decrypt_init(ctx);

        size_t text_len;
        EVP_PKEY_decrypt(ctx, nullptr, &text_len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());

        std::vector<unsigned char> plaintext(text_len);
        EVP_PKEY_decrypt(ctx, plaintext.data(), &text_len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
        EVP_PKEY_CTX_free(ctx);

        std::string decrypted_text(reinterpret_cast<const char*>(plaintext.data()), text_len);

        return decrypted_text;
    }
}; 