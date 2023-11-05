class Symmetric {
protected:
    std::string key;

    template <typename Func> std::string template_encrypt(Func&& func, const std::string& data) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

        if (!ctx) {
            throw std::runtime_error("Не удалось создать контекст шифрования");
        }

        EVP_EncryptInit_ex(ctx, func(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

        int max_ciphertext_length = data.length() + EVP_MAX_BLOCK_LENGTH;
        std::vector<unsigned char> ciphertext(max_ciphertext_length);

        int ciphertext_length = 0;
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &ciphertext_length, reinterpret_cast<const unsigned char*>(data.c_str()), data.length()) <= 0) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при обновлении шифрования");
        }

        int final_ciphertext_length = 0;
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + ciphertext_length, &final_ciphertext_length) <= 0) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при завершении шифрования");
        }

        ciphertext_length += final_ciphertext_length;
        EVP_CIPHER_CTX_free(ctx);
        ciphertext.resize(ciphertext_length);

        return std::string(reinterpret_cast<const char*>(ciphertext.data()), ciphertext_length);
    }

    template <typename Func> std::string template_decrypt(Func&& func, const std::string& ciphertext) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

        if (!ctx) {
            throw std::runtime_error("Не удалось создать контекст дешифрования");
        }

        EVP_DecryptInit_ex(ctx, func(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

        int max_plaintext_length = ciphertext.length();
        std::vector<unsigned char> plaintext(max_plaintext_length);

        int plaintext_length = 0;
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &plaintext_length, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.length()) <= 0) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при обновлении дешифрования");
        }

        int final_plaintext_length = 0;
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + plaintext_length, &final_plaintext_length) <= 0) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при завершении дешифрования");
        }

        plaintext_length += final_plaintext_length;
        EVP_CIPHER_CTX_free(ctx);

        plaintext.resize(plaintext_length);

        return std::string(reinterpret_cast<const char*>(plaintext.data()), plaintext_length);
    }

    virtual void template_generate_key(const int key_length) {
        key.resize(key_length);

        if (RAND_bytes(reinterpret_cast<unsigned char*>(&key[0]), key_length) != 1) {
            throw std::runtime_error("Ошибка при генерации ключа");
        }
    }


public:
    virtual void set_key(std::string key) {
        this->key = key;
    }

    virtual std::string get_key() {
        return key;
    }

    virtual void generate_key() = 0;
    virtual std::string encrypt(const std::string& data) = 0;
    virtual std::string decrypt(const std::string& data) = 0;
};
