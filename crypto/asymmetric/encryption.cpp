class Encryption: virtual public Asymmetric {
public:
    virtual void encrypt(const std::string& input_file_path, const std::string& output_file_path) {
        // Открываем входной файл для чтения
        std::ifstream input_file(input_file_path, std::ios::binary);
        
        if (!input_file.is_open()) {
            throw std::runtime_error("Failed to open the input file for reading");
        }
        
        // Открываем выходной файл для записи
        std::ofstream output_file(output_file_path, std::ios::binary);
        
        if (!output_file.is_open()) {
            throw std::runtime_error("Failed to open the output file for writing");
        }

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

        // Чтение данных из входного файла и шифрование
        std::vector<unsigned char> input_buffer(1024);
        std::vector<unsigned char> output_buffer(1024);
        size_t ciphertext_len; // Изменение типа на size_t
        
        while (input_file.good()) {
            input_file.read(reinterpret_cast<char*>(input_buffer.data()), input_buffer.size());
            int input_length = static_cast<int>(input_file.gcount());

            if (EVP_PKEY_encrypt(ctx, output_buffer.data(), &ciphertext_len, input_buffer.data(), input_length) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                throw std::runtime_error("Failed to encrypt the data");
            }

            // Запись шифртекста в выходной файл
            output_file.write(reinterpret_cast<const char*>(output_buffer.data()), ciphertext_len);
        }

        // Освобождение контекста
        EVP_PKEY_CTX_free(ctx);

        // Закрываем файлы
        input_file.close();
        output_file.close();
    }

    virtual void decrypt(const std::string& input_file_path, const std::string& output_file_path) {
        // Открываем входной файл для чтения
        std::ifstream input_file(input_file_path, std::ios::binary);

        if (!input_file.is_open()) {
            throw std::runtime_error("Failed to open the input file for reading");
        }

        // Открываем выходной файл для записи
        std::ofstream output_file(output_file_path, std::ios::binary);

        if (!output_file.is_open()) {
            throw std::runtime_error("Failed to open the output file for writing");
        }

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

        // Чтение данных из входного файла и дешифрование
        std::vector<unsigned char> input_buffer(1024);
        std::vector<unsigned char> output_buffer(1024);
        size_t plaintext_len; // Изменение типа на size_t

        while (input_file.good()) {
            input_file.read(reinterpret_cast<char*>(input_buffer.data()), input_buffer.size());
            int input_length = static_cast<int>(input_file.gcount());

            if (EVP_PKEY_decrypt(ctx, output_buffer.data(), &plaintext_len, input_buffer.data(), input_length) <= 0) {
                EVP_PKEY_CTX_free(ctx);
                throw std::runtime_error("Failed to decrypt the data");
            }

            // Запись расшифрованных данных в выходной файл
            output_file.write(reinterpret_cast<const char*>(output_buffer.data()), plaintext_len);
        }

        // Освобождение контекста
        EVP_PKEY_CTX_free(ctx);

        // Закрываем файлы
        input_file.close();
        output_file.close();
}

};
