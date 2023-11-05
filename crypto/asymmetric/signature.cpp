class Signature: virtual public Asymmetric {
public:
    virtual void sign_file(const std::string& input_filepath, const std::string& output_filepath) {
        // Открываем входной файл
        std::ifstream input_file(input_filepath, std::ios::binary);
        if (!input_file.is_open()) {
            throw std::runtime_error("Не удалось открыть входной файл");
        }

        // Создаем контекст хеширования сообщения
        EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
        if (!md_ctx) {
            input_file.close();
            throw std::runtime_error("Не удалось создать контекст хеширования");
        }

        // Инициализируем контекст подписи
        if (EVP_DigestSignInit(md_ctx, nullptr, EVP_sha256(), nullptr, key) <= 0) {
            EVP_MD_CTX_free(md_ctx);
            input_file.close();
            throw std::runtime_error("Не удалось инициализировать контекст подписи");
        }

        // Читаем входной файл блоками и обновляем контекст подписи
        size_t buffer_size = 4096;
        char buffer[buffer_size];
        while (input_file.read(buffer, buffer_size)) {
            EVP_DigestSignUpdate(md_ctx, buffer, input_file.gcount());
        }

        // Определяем размер подписи
        size_t signature_size;
        if (EVP_DigestSignFinal(md_ctx, nullptr, &signature_size) <= 0) {
            EVP_MD_CTX_free(md_ctx);
            input_file.close();
            throw std::runtime_error("Не удалось определить размер подписи");
        }

        // Выделяем память под подпись
        std::vector<unsigned char> signature(signature_size);

        // Сбрасываем позицию входного файла и снова обновляем контекст подписи
        input_file.clear();
        input_file.seekg(0);
        while (input_file.read(buffer, buffer_size)) {
            EVP_DigestSignUpdate(md_ctx, buffer, input_file.gcount());
        }

        // Генерируем окончательную подпись
        if (EVP_DigestSignFinal(md_ctx, signature.data(), &signature_size) <= 0) {
            EVP_MD_CTX_free(md_ctx);
            input_file.close();
            throw std::runtime_error("Не удалось сгенерировать подпись");
        }

        // Закрываем входной файл
        input_file.close();

        // Освобождаем контекст хеширования
        EVP_MD_CTX_free(md_ctx);

        // Открываем выходной файл для записи
        std::ofstream output_file(output_filepath, std::ios::binary);
        if (!output_file.is_open()) {
            throw std::runtime_error("Не удалось открыть выходной файл");
        }

        // Записываем подпись в выходной файл
        output_file.write(reinterpret_cast<const char*>(signature.data()), signature_size);

        // Закрываем выходной файл
        output_file.close();
    }

    virtual bool verify_signature(const std::string& input_filepath, const std::string& signature_filepath) {
        // Открываем входной файл и файл с подписью
        std::ifstream input_file(input_filepath, std::ios::binary);
        std::ifstream signature_file(signature_filepath, std::ios::binary);

        if (!input_file.is_open() || !signature_file.is_open()) {
            return false;  // Возвращаем false в случае ошибки открытия файлов
        }

        // Создаем контекст хеширования сообщения
        EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
        if (!md_ctx) {
            input_file.close();
            signature_file.close();
            throw std::runtime_error("Не удалось создать контекст хеширования");
        }

        // Инициализируем контекст проверки подписи
        if (EVP_DigestVerifyInit(md_ctx, nullptr, EVP_sha256(), nullptr, key) <= 0) {
            EVP_MD_CTX_free(md_ctx);
            input_file.close();
            signature_file.close();
            throw std::runtime_error("Не удалось инициализировать контекст проверки подписи");
        }

        // Читаем входной файл блоками и обновляем контекст проверки подписи
        size_t buffer_size = 4096;
        char buffer[buffer_size];
        while (input_file.read(buffer, buffer_size)) {
            EVP_DigestVerifyUpdate(md_ctx, buffer, input_file.gcount());
        }

        // Читаем файл с подписью и проверяем подпись
        size_t signature_size = 0;
        signature_file.seekg(0, std::ios::end);
        signature_size = static_cast<size_t>(signature_file.tellg());
        signature_file.seekg(0, std::ios::beg);

        std::vector<unsigned char> signature(signature_size);
        signature_file.read(reinterpret_cast<char*>(signature.data()), signature_size);

        int verify_result = EVP_DigestVerifyFinal(md_ctx, signature.data(), signature_size);

        // Закрываем входной файл, файл с подписью и освобождаем контекст хеширования
        input_file.close();
        signature_file.close();
        EVP_MD_CTX_free(md_ctx);

        return verify_result;
    }
};
