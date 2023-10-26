class Symmetric {
protected:
    std::string key;

    template <typename Func> std::string template_encrypt(Func&& func, const std::string& data) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

        EVP_EncryptInit_ex(ctx, func(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

        int max_ciphertext_length = data.length() + EVP_MAX_BLOCK_LENGTH;
        std::vector<unsigned char> ciphertext(max_ciphertext_length);

        int ciphertext_length = 0;
        EVP_EncryptUpdate(ctx, ciphertext.data(), &ciphertext_length, reinterpret_cast<const unsigned char*>(data.c_str()), data.length());

        int final_ciphertext_length = 0;
        EVP_EncryptFinal_ex(ctx, ciphertext.data() + ciphertext_length, &final_ciphertext_length);

        ciphertext_length += final_ciphertext_length;
        EVP_CIPHER_CTX_free(ctx);
        ciphertext.resize(ciphertext_length);

        return std::string(reinterpret_cast<const char*>(ciphertext.data()), ciphertext_length);
    }

    template <typename Func> std::string template_decrypt(Func&& func, const std::string& ciphertext) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

        EVP_DecryptInit_ex(ctx, func(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

        int max_plaintext_length = ciphertext.length();
        std::vector<unsigned char> plaintext(max_plaintext_length);

        int plaintext_length = 0;
        EVP_DecryptUpdate(ctx, plaintext.data(), &plaintext_length, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.length());

        int final_plaintext_length = 0;
        EVP_DecryptFinal_ex(ctx, plaintext.data() + plaintext_length, &final_plaintext_length);

        plaintext_length += final_plaintext_length;

        EVP_CIPHER_CTX_free(ctx);

        plaintext.resize(plaintext_length);

        return std::string(reinterpret_cast<const char*>(plaintext.data()), plaintext_length);
    }

    virtual std::string template_generate_key(const int key_length) {
        // unsigned char key_data[key_length];

        // RAND_bytes(key_data, key_length);

        // std::string input(reinterpret_cast<const char*>(key_data), key_length);

        // BIO* bmem = BIO_new(BIO_s_mem());
        // BIO* b64 = BIO_new(BIO_f_base64());
        // BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        // BIO_push(b64, bmem);
        // BIO_write(b64, input.c_str(), input.size());
        // BIO_flush(b64);

        // BUF_MEM* bptr;
        // BIO_get_mem_ptr(b64, &bptr);

        // std::string output(bptr->data, bptr->length - 1);

        // BIO_free_all(b64);

        // key = output;
        // return key;


        key.resize(key_length);

        RAND_bytes(reinterpret_cast<unsigned char*>(&key[0]), key_length);

        return key;
    }

public:
    Symmetric() {}
    
    Symmetric(const std::string& key) : key(key) {}

    ~Symmetric() {}

    virtual void set_key(std::string key) {
        this->key = key;
    }

    virtual std::string get_key() {
        return key;
    }

    virtual std::string generate_key() = 0;
    virtual std::string encrypt(const std::string& data) = 0;
    virtual std::string decrypt(const std::string& data) = 0;
};