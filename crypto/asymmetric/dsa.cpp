class Dsa: public SignatureManager {
public:
    void generate_keys() override {
        EVP_PKEY_CTX *ctx_params = EVP_PKEY_CTX_new_id(EVP_PKEY_DSA, NULL);
        EVP_PKEY_paramgen_init(ctx_params);
        EVP_PKEY_CTX_set_dsa_paramgen_bits(ctx_params, 2048);

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
    }
};