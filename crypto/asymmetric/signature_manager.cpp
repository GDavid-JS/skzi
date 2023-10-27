class SignatureManager: virtual public Asymmetric {
public:
    virtual void sign_file(const std::string& input_filepath, const std::string& output_filepath) {
        std::ifstream input_file(input_filepath, std::ios::binary);

        EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
        EVP_DigestSignInit(md_ctx, nullptr, EVP_sha256(), nullptr, key);

        // Чтение файла по блокам и обновление контекста подписи с данными
        size_t buffer_size = 4096;
        char buffer[buffer_size];
        while (input_file.read(buffer, buffer_size)) {
            EVP_DigestSignUpdate(md_ctx, buffer, input_file.gcount());
        }

        // Завершение подписи и получение размера подписи
        size_t signature_size;
        EVP_DigestSignFinal(md_ctx, nullptr, &signature_size);

        std::vector<unsigned char> signature(signature_size);

        input_file.clear();
        input_file.seekg(0);
        while (input_file.read(buffer, buffer_size)) {
            EVP_DigestSignUpdate(md_ctx, buffer, input_file.gcount());
        }
        EVP_DigestSignFinal(md_ctx, signature.data(), &signature_size);

        input_file.close();

        EVP_MD_CTX_free(md_ctx);

        std::ofstream output_file(output_filepath, std::ios::binary);

        // Запись подписи в выходной файл
        output_file.write(reinterpret_cast<const char*>(signature.data()), signature_size);

        // Закрытие выходного файла
        output_file.close();
    }

    virtual bool verify_signature(const std::string& input_filepath, const std::string& signature_filepath) {
        std::ifstream input_file(input_filepath, std::ios::binary);
        std::ifstream signature_file(signature_filepath, std::ios::binary);

        if (!input_file.is_open() || !signature_file.is_open()) {
            // Обработка ошибки при открытии файлов
            return false;
        }

        EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
        EVP_DigestVerifyInit(md_ctx, nullptr, EVP_sha256(), nullptr, key);

        // Чтение файла с данными по блокам и обновление контекста подписи
        size_t buffer_size = 4096;
        char buffer[buffer_size];
        while (input_file.read(buffer, buffer_size)) {
            EVP_DigestVerifyUpdate(md_ctx, buffer, input_file.gcount());
        }

        // Чтение файла с подписью и проверка подписи
        size_t signature_size = 0;
        signature_file.seekg(0, std::ios::end);
        signature_size = static_cast<size_t>(signature_file.tellg());
        signature_file.seekg(0, std::ios::beg);

        std::vector<unsigned char> signature(signature_size);
        signature_file.read(reinterpret_cast<char*>(signature.data()), signature_size);

        int verify_result = EVP_DigestVerifyFinal(md_ctx, signature.data(), signature_size);

        input_file.close();
        signature_file.close();
        EVP_MD_CTX_free(md_ctx);

        if (verify_result == 1) {
            // Подпись верна
            return true;
        } else if (verify_result == 0) {
            // Подпись неверна
            return false;
        } else {
            // Обработка ошибки при проверке подписи
            return false;
        }
    }
};