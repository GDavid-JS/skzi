class Ecdsa: public SignatureManager {
public:
    void generate_keys() override {
        EVP_PKEY_CTX *ctx_params = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
        EVP_PKEY_paramgen_init(ctx_params);
        EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx_params, NID_X9_62_prime256v1);

        EVP_PKEY* pkey_params = NULL;
        EVP_PKEY_paramgen(ctx_params, &pkey_params);

        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey_params, NULL);
        EVP_PKEY_keygen_init(ctx);
        
        EVP_PKEY_keygen(ctx, &key);

        EVP_PKEY_free(pkey_params);
        EVP_PKEY_CTX_free(ctx_params);
        EVP_PKEY_CTX_free(ctx);
    }
};