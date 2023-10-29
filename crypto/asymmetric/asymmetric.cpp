class Asymmetric {
private:
    template <typename Func, typename... Args> auto from_evp(Func&& func, Args... args) {
        BIO* key_bio = BIO_new(BIO_s_mem());

        func(key_bio, key, args...);

        BUF_MEM* key_buffer_ptr;
        BIO_get_mem_ptr(key_bio, &key_buffer_ptr);

        const std::string key_str(key_buffer_ptr->data, key_buffer_ptr->length);

        BIO_free(key_bio);

        return key_str;
    }

protected:
    EVP_PKEY* key = nullptr;
    
public:

    ~Asymmetric() {
        EVP_PKEY_free(this->key);
    }

    virtual void set_public(const std::string& public_key) {
        BIO* key_bio = BIO_new_mem_buf(public_key.c_str(), -1);
        PEM_read_bio_PUBKEY(key_bio, &this->key, nullptr, nullptr);
        BIO_free(key_bio);
    }

    virtual void set_private(const std::string& private_key) {
        BIO* key_bio = BIO_new_mem_buf(private_key.c_str(), -1);
        PEM_read_bio_PrivateKey(key_bio, &this->key, nullptr, nullptr);
        BIO_free(key_bio);
    }

    virtual std::string get_public() {
        return from_evp(&PEM_write_bio_PUBKEY);
    }

    virtual std::string get_private() {
        return from_evp(&PEM_write_bio_PrivateKey, nullptr, nullptr, 0, nullptr, nullptr);
    }

    virtual void fromX509(std::string x509_filepath) {
        std::ifstream x509_file(x509_filepath, std::ios::binary);
        
        fromX509(x509_file);
    }

    virtual void fromX509(std::ifstream& x509_file) {
        std::string x509_content((std::istreambuf_iterator<char>(x509_file)), std::istreambuf_iterator<char>());

        BIO* x509_bio = BIO_new_mem_buf(x509_content.c_str(), -1);
        X509* x509 = PEM_read_bio_X509(x509_bio, nullptr, nullptr, nullptr);

        this->key = X509_get_pubkey(x509);
        X509_free(x509);
        BIO_free(x509_bio);
    }

    virtual void generateSelfSignedCertificateX509(const std::string& commonName, const std::string& organization, const std::string& emailAddress) {
        X509* x509 = X509_new();

        // Создаем сертификат с открытым ключом
        X509_set_version(x509, 2); // Версия X.509 сертификата
        ASN1_INTEGER_set(X509_get_serialNumber(x509), 1); // Уникальный серийный номер
        X509_gmtime_adj(X509_get_notBefore(x509), 0); // Дата начала действия
        X509_gmtime_adj(X509_get_notAfter(x509), 31536000L); // Дата окончания действия (1 год)
        X509_set_pubkey(x509, key);

        // Устанавливаем субъект и издателя как "self-signed"
        X509_NAME* name = X509_get_subject_name(x509);
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (const unsigned char*)commonName.c_str(), -1, -1, 0);
        X509_set_issuer_name(x509, name);

        // Добавляем дополнительные атрибуты
        X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (const unsigned char*)organization.c_str(), -1, -1, 0);
        X509_NAME_add_entry_by_txt(name, "emailAddress", MBSTRING_ASC, (const unsigned char*)emailAddress.c_str(), -1, -1, 0);

        // Подписываем сертификат с использованием закрытого ключа
        X509_sign(x509, key, EVP_sha256());
            

        FILE* certFile = fopen("self_signed_certificate.pem", "wb");
        PEM_write_X509(certFile, x509);

        X509_free(x509);
    }



    virtual void generate_keys() = 0;
};