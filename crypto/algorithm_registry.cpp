template <class TypeClass> class Register {
private:
    std::map<std::string, std::function<TypeClass*()>> registry;

public:
    template <typename Algorithm> void register_algorithm(const std::string& name) {
        registry[name] = []() { return new Algorithm; };
    }

    TypeClass* operator[](const std::string& name) {
        if (registry.find(name) != registry.end()) {
            return registry[name]();
        }
        return nullptr;
    }

    virtual void register_classes() = 0;
};

class AsymmetricRegister: public Register<Asymmetric> {
public:
    AsymmetricRegister() {
        register_classes();
    }

    void register_classes() override {
        register_algorithm<Rsa512>("Rsa512");
        register_algorithm<Rsa1024>("Rsa1024");
        register_algorithm<Rsa2048>("Rsa2048");
        register_algorithm<Rsa4096>("Rsa4096");
        register_algorithm<Rsa8192>("Rsa8192");
        register_algorithm<Rsa16384>("Rsa16384");

        register_algorithm<Dsa512>("Dsa512");
        register_algorithm<Dsa1024>("Dsa1024");
        register_algorithm<Dsa2048>("Dsa2048");
        register_algorithm<Dsa4096>("Dsa4096");
        register_algorithm<Dsa8192>("Dsa8192");
        register_algorithm<Dsa16384>("Dsa16384");
    }
};

class EncryptionRegister: public Register<Asymmetric> {
public:
    EncryptionRegister() {
        register_classes();
    }

    void register_classes() override {
        register_algorithm<Rsa512>("Rsa512");
        register_algorithm<Rsa1024>("Rsa1024");
        register_algorithm<Rsa2048>("Rsa2048");
        register_algorithm<Rsa4096>("Rsa4096");
        register_algorithm<Rsa8192>("Rsa8192");
        register_algorithm<Rsa16384>("Rsa16384");
        register_algorithm<RsaPss>("RsaPss");
    }
};

class SignatureRegister: public Register<Asymmetric> {
public:
    SignatureRegister() {
        register_classes();
    }

    void register_classes() override {
        register_algorithm<Dsa512>("Dsa512");
        register_algorithm<Dsa1024>("Dsa1024");
        register_algorithm<Dsa2048>("Dsa2048");
        register_algorithm<Dsa4096>("Dsa4096");
        register_algorithm<Dsa8192>("Dsa8192");
        register_algorithm<Dsa16384>("Dsa16384");
        register_algorithm<Ecdsa>("Ecdsa");
    }
};