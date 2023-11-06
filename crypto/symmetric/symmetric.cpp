class Symmetric {
protected:
    std::string key;

    template <typename Func> void template_encrypt(Func&& func, const std::string& input_file_path, const std::string& output_file_path) {
        std::ifstream input_file(input_file_path, std::ios::binary);

        if (!input_file.is_open()) {
            throw std::runtime_error("Не удалось открыть входной файл для чтения");
        }

        std::ofstream output_file(output_file_path, std::ios::binary);

        if (!output_file.is_open()) {
            throw std::runtime_error("Не удалось открыть выходной файл для записи");
        }

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

        if (!ctx) {
            throw std::runtime_error("Не удалось создать контекст шифрования");
        }

        EVP_EncryptInit_ex(ctx, func(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

        int max_block_length = EVP_CIPHER_CTX_block_size(ctx);
        std::vector<unsigned char> input_buffer(max_block_length);
        std::vector<unsigned char> output_buffer(max_block_length);

        while (input_file.good()) {
            input_file.read(reinterpret_cast<char*>(input_buffer.data()), max_block_length);
            int input_length = static_cast<int>(input_file.gcount());

            int output_length = 0;
            if (EVP_EncryptUpdate(ctx, output_buffer.data(), &output_length, input_buffer.data(), input_length) <= 0) {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("Ошибка при обновлении шифрования");
            }

            output_file.write(reinterpret_cast<const char*>(output_buffer.data()), output_length);
        }

        int final_output_length = 0;
        if (EVP_EncryptFinal_ex(ctx, output_buffer.data(), &final_output_length) <= 0) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при завершении шифрования");
        }

        output_file.write(reinterpret_cast<const char*>(output_buffer.data()), final_output_length);

        EVP_CIPHER_CTX_free(ctx);

        input_file.close();
        output_file.close();
    }

    template <typename Func> void template_decrypt(Func&& func, const std::string& input_file_path, const std::string& output_file_path) {
        std::ifstream input_file(input_file_path, std::ios::binary);

        if (!input_file.is_open()) {
            throw std::runtime_error("Не удалось открыть входной файл для чтения");
        }

        std::ofstream output_file(output_file_path, std::ios::binary);

        if (!output_file.is_open()) {
            throw std::runtime_error("Не удалось открыть выходной файл для записи");
        }

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

        if (!ctx) {
            throw std::runtime_error("Не удалось создать контекст дешифрования");
        }

        EVP_DecryptInit_ex(ctx, func(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

        int max_block_length = EVP_CIPHER_CTX_block_size(ctx);
        std::vector<unsigned char> input_buffer(max_block_length);
        std::vector<unsigned char> output_buffer(max_block_length);

        while (input_file.good()) {
            input_file.read(reinterpret_cast<char*>(input_buffer.data()), max_block_length);
            int input_length = static_cast<int>(input_file.gcount());

            int output_length = 0;
            if (EVP_DecryptUpdate(ctx, output_buffer.data(), &output_length, input_buffer.data(), input_length) <= 0) {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("Ошибка при обновлении дешифрования");
            }

            output_file.write(reinterpret_cast<const char*>(output_buffer.data()), output_length);
        }

        int final_output_length = 0;
        if (EVP_DecryptFinal_ex(ctx, output_buffer.data(), &final_output_length) <= 0) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при завершении дешифрования");
        }

        output_file.write(reinterpret_cast<const char*>(output_buffer.data()), final_output_length);

        EVP_CIPHER_CTX_free(ctx);

        input_file.close();
        output_file.close();
    }


    virtual void template_generate_key(const int key_length) {
        // key.resize(key_length);

        // if (RAND_bytes(reinterpret_cast<unsigned char*>(&key[0]), key_length) != 1) {
        //     throw std::runtime_error("Ошибка при генерации ключа");
        // }
        unsigned char key_data[key_length];

        RAND_bytes(key_data, key_length);

        std::string input(reinterpret_cast<const char*>(key_data), key_length);

        BIO* bmem = BIO_new(BIO_s_mem());
        BIO* b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        BIO_push(b64, bmem);
        BIO_write(b64, input.c_str(), input.size());
        BIO_flush(b64);

        BUF_MEM* bptr;
        BIO_get_mem_ptr(b64, &bptr);

        std::string output(bptr->data, bptr->length - 1);

        BIO_free_all(b64);

        key = output;
    }


public:
    virtual void set_key(std::string key) {
        this->key = key;
    }

    virtual std::string get_key() {
        return key;
    }

    virtual void generate_key() = 0;
    virtual void encrypt(const std::string& input_file_path, const std::string& output_file_path) = 0;
    virtual void decrypt(const std::string& input_file_path, const std::string& output_file_path) = 0;
};
