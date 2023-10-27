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
    
    template <typename Func> void template_generate_keys(Func&& func, int keyType, const unsigned short size) {
        // E</typename>VP_PKEY_CTX* ctx_params = EVP_PKEY_CTX_new_id(keyType, nullptr);
        // EVP_PKEY_keygen_init(ctx_params);
        // func(ctx_params, size);
        // EVP_PKEY_keygen(ctx, &key);
        // EVP_PKEY_CTX_free(ctx);


        // EVP_PKEY* pkey_params = NULL;
        // EVP_PKEY_paramgen(ctx_params, &pkey_params);

        // // using parameters, build DSA keypair
        // EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey_params, NULL);
        // EVP_PKEY_keygen_init(ctx);
        
        // EVP_PKEY* pkey = NULL;
        // EVP_PKEY_keygen(ctx, &pkey);

        // // cleanup everything but the final key
        // EVP_PKEY_free(pkey_params);
        // EVP_PKEY_CTX_free(ctx_params);
        // EVP_PKEY_CTX_free(ctx);
    
        // build parameters first
        EVP_PKEY_CTX *ctx_params = EVP_PKEY_CTX_new_id(keyType, NULL);
        EVP_PKEY_paramgen_init(ctx_params);
        func(ctx_params, size);

        EVP_PKEY* pkey_params = NULL;
        EVP_PKEY_paramgen(ctx_params, &pkey_params);

        // using parameters, build DSA keypair
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey_params, NULL);
        EVP_PKEY_keygen_init(ctx);
        
        // EVP_PKEY* pkey = NULL;
        EVP_PKEY_keygen(ctx, &key);

        // cleanup everything but the final key
        EVP_PKEY_free(pkey_params);
        EVP_PKEY_CTX_free(ctx_params);
        EVP_PKEY_CTX_free(ctx);

        // DSA* dsa = EVP_PKEY_get1_DSA(pkey);
        // BIO* bio = BIO_new_fp(stdout, BIO_NOCLOSE);
        // PEM_write_bio_DSAPrivateKey(bio, dsa, NULL, NULL, 0, NULL, NULL);
        // BIO_flush(bio);
        // DSA_free(dsa);
        // EVP_PKEY_free(pkey);
    }

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