class Encryption: virtual public Asymmetric {
public:
    virtual std::string encrypt(const std::string& text) {
        // Создаем контекст ключа
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(this->key, nullptr);
        if (!ctx) {
            throw std::runtime_error("Failed to create PKEY context");
        }
        
        // Инициализация контекста для шифрования
        if (EVP_PKEY_encrypt_init(ctx) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize encryption context");
        }

        // Определение длины шифртекста
        size_t ciphertext_len;
        if (EVP_PKEY_encrypt(ctx, nullptr, &ciphertext_len, reinterpret_cast<const unsigned char*>(text.c_str()), text.size()) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to determine ciphertext length");
        }

        // Выделение памяти для шифртекста
        std::vector<unsigned char> ciphertext(ciphertext_len);
        
        // Шифрование данных
        if (EVP_PKEY_encrypt(ctx, ciphertext.data(), &ciphertext_len, reinterpret_cast<const unsigned char*>(text.c_str()), text.size()) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to encrypt the data");
        }

        // Освобождение контекста
        EVP_PKEY_CTX_free(ctx);

        // Преобразование шифртекста в строку
        std::string encrypted_text(reinterpret_cast<const char*>(ciphertext.data()), ciphertext_len);

        return encrypted_text;
    }

    virtual std::string decrypt(const std::string& ciphertext) {
        // Создаем контекст ключа
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(this->key, nullptr);
        if (!ctx) {
            throw std::runtime_error("Failed to create PKEY context");
        }

        // Инициализация контекста для дешифрования
        if (EVP_PKEY_decrypt_init(ctx) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize decryption context");
        }

        // Определение длины расшифрованных данных
        size_t text_len;
        if (EVP_PKEY_decrypt(ctx, nullptr, &text_len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size()) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to determine plaintext length");
        }

        // Выделение памяти для расшифрованных данных
        std::vector<unsigned char> plaintext(text_len);
        
        // Расшифрование данных
        if (EVP_PKEY_decrypt(ctx, plaintext.data(), &text_len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size()) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("Failed to decrypt the data");
        }

        // Освобождение контекста
        EVP_PKEY_CTX_free(ctx);

        // Преобразование расшифрованных данных в строку
        std::string decrypted_text(reinterpret_cast<const char*>(plaintext.data()), text_len);

        return decrypted_text;
    }
};
