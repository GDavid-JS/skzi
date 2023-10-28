class RsaPss: public SignatureManager, public EncryptionManager {
public:
    void generate_keys() {
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        EVP_PKEY_keygen_init(ctx);
        
        EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PSS_PADDING);
        
        EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256());
        EVP_PKEY_CTX_set_rsa_pss_saltlen(ctx, -1);

        EVP_PKEY_keygen(ctx, &key);
        
        EVP_PKEY_CTX_free(ctx);
    }
};