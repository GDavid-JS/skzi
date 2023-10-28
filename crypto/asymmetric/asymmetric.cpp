class Asymmetric {
private:
    template <typename Func, typename... Args> auto key_to_pem(Func&& func, Args... args) {
        BIO* key_bio = BIO_new(BIO_s_mem());

        func(key_bio, key, args...);

        BUF_MEM* key_buffer_ptr;
        BIO_get_mem_ptr(key_bio, &key_buffer_ptr);

        const std::string key_str(key_buffer_ptr->data, key_buffer_ptr->length);

        BIO_free(key_bio);

        return key_str;
    }

    template <typename Func> void string_to_evp(Func&& func, const std::string& key) {
        BIO* key_bio = BIO_new_mem_buf(key.c_str(), -1);

        func(key_bio, &this->key, nullptr, nullptr);

        BIO_free(key_bio);
    }

protected:
    EVP_PKEY* key = nullptr;
    
public:

    ~Asymmetric() {
        EVP_PKEY_free(this->key);
    }

    virtual void set_public(const std::string& public_key) {
        string_to_evp(&PEM_read_bio_PUBKEY, public_key);
    }

    virtual void set_private(const std::string& private_key) {
        string_to_evp(&PEM_read_bio_PrivateKey, private_key);
    }

    virtual std::string get_public() {
        return key_to_pem(&PEM_write_bio_PUBKEY);
    }

    virtual std::string get_private() {
        return key_to_pem(&PEM_write_bio_PrivateKey, nullptr, nullptr, 0, nullptr, nullptr);
    }

    virtual void generate_keys() = 0;
};