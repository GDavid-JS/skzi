template <class TypeClass> class Register {
private:
    std::map<std::string, std::function<TypeClass*()>> registry;

public:
    template <typename Algorithm> void register_algorithm(const std::string& name) {
        if (registry.find(name) != registry.end()) {
            throw std::runtime_error("Алгоритм с таким именем уже зарегистрирован.");
        }
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

        register_algorithm<Dsa1024>("Dsa1024");
        register_algorithm<Dsa2048>("Dsa2048");
        register_algorithm<Dsa3072>("Dsa3072");
        register_algorithm<Dsa4096>("Dsa4096");
        register_algorithm<Dsa5120>("Dsa5120");
        register_algorithm<Dsa6144>("Dsa6144");
        register_algorithm<Dsa7168>("Dsa7168");
        register_algorithm<Dsa8192>("Dsa8192");
        register_algorithm<Ecdsa>("Ecdsa");
    }
};

class SymmetricRegister: public Register<Symmetric> {
public:
    SymmetricRegister() {
        register_classes();
    }

    void register_classes() override {
        register_algorithm<Aes128Cbc>("Aes128Cbc");
        register_algorithm<Aes128Cfb>("Aes128Cfb");
        register_algorithm<Aes128Cfb1>("Aes128Cfb1");
        register_algorithm<Aes128Cfb8>("Aes128Cfb8");
        register_algorithm<Aes128Ctr>("Aes128Ctr");
        register_algorithm<Aes128Ecb>("Aes128Ecb");
        register_algorithm<Aes128Ofb>("Aes128Ofb");

        register_algorithm<Aes192Cbc>("Aes192Cbc");
        register_algorithm<Aes192Cfb>("Aes192Cfb");
        register_algorithm<Aes192Cfb1>("Aes192Cfb1");
        register_algorithm<Aes192Cfb8>("Aes192Cfb8");
        register_algorithm<Aes192Ctr>("Aes192Ctr");
        register_algorithm<Aes192Ecb>("Aes192Ecb");
        register_algorithm<Aes192Ofb>("Aes192Ofb");

        register_algorithm<Aes256Cbc>("Aes256Cbc");
        register_algorithm<Aes256Cfb>("Aes256Cfb");
        register_algorithm<Aes256Cfb1>("Aes256Cfb1");
        register_algorithm<Aes256Cfb8>("Aes256Cfb8");
        register_algorithm<Aes256Ctr>("Aes256Ctr");
        register_algorithm<Aes256Ecb>("Aes256Ecb");
        register_algorithm<Aes256Ofb>("Aes256Ofb");
        
        register_algorithm<Aes128Wrap>("Aes128Wrap");
        register_algorithm<Aes192Wrap>("Aes192Wrap");
        register_algorithm<Aes256Wrap>("Aes256Wrap");

        register_algorithm<Aria128Cbc>("Aria128Cbc");
        register_algorithm<Aria128Cfb>("Aria128Cfb");
        register_algorithm<Aria128Cfb1>("Aria128Cfb1");
        register_algorithm<Aria128Cfb8>("Aria128Cfb8");
        register_algorithm<Aria128Ctr>("Aria128Ctr");
        register_algorithm<Aria128Ecb>("Aria128Ecb");
        register_algorithm<Aria128Ofb>("Aria128Ofb");

        register_algorithm<Aria192Cbc>("Aria192Cbc");
        register_algorithm<Aria192Cfb>("Aria192Cfb");
        register_algorithm<Aria192Cfb1>("Aria192Cfb1");
        register_algorithm<Aria192Cfb8>("Aria192Cfb8");
        register_algorithm<Aria192Ctr>("Aria192Ctr");
        register_algorithm<Aria192Ecb>("Aria192Ecb");
        register_algorithm<Aria192Ofb>("Aria192Ofb");

        register_algorithm<Aria256Cbc>("Aria256Cbc");
        register_algorithm<Aria256Cfb>("Aria256Cfb");
        register_algorithm<Aria256Cfb1>("Aria256Cfb1");
        register_algorithm<Aria256Cfb8>("Aria256Cfb8");
        register_algorithm<Aria256Ctr>("Aria256Ctr");
        register_algorithm<Aria256Ecb>("Aria256Ecb");
        register_algorithm<Aria256Ofb>("Aria256Ofb");

        register_algorithm<BfCbc>("BfCbc");
        register_algorithm<BfCfb>("BfCfb");
        register_algorithm<BfEcb>("BfEcb");
        register_algorithm<BfOfb>("BfOfb");

        register_algorithm<Chacha20>("Chacha20");

        register_algorithm<DesCbc>("DesCbc");
        register_algorithm<DesCfb>("DesCfb");
        register_algorithm<DesCfb1>("DesCfb1");
        register_algorithm<DesCfb8>("DesCfb8");
        register_algorithm<DesEcb>("DesEcb");
        register_algorithm<DesEde>("DesEde");
        register_algorithm<DesEdeCbc>("DesEdeCbc");
        register_algorithm<DesEdeCfb>("DesEdeCfb");
        register_algorithm<DesEdeEcb>("DesEdeEcb");
        register_algorithm<DesEdeOfb>("DesEdeOfb");
        register_algorithm<DesEde3>("DesEde3");
        register_algorithm<DesEde3Cbc>("DesEde3Cbc");
        register_algorithm<DesEde3Cfb>("DesEde3Cfb");
        register_algorithm<DesEde3Cfb1>("DesEde3Cfb1");
        register_algorithm<DesEde3Cfb8>("DesEde3Cfb8");
        register_algorithm<DesEde3Ecb>("DesEde3Ecb");
        register_algorithm<DesEde3Ofb>("DesEde3Ofb");
        register_algorithm<DesOfb>("DesOfb");

        register_algorithm<DesxCbc>("DesxCbc");

        register_algorithm<Rc240Cbc>("Rc240Cbc");
        register_algorithm<Rc264Cbc>("Rc264Cbc");
        register_algorithm<Rc2Cbc>("Rc2Cbc");
        register_algorithm<Rc2Cfb>("Rc2Cfb");
        register_algorithm<Rc2Ecb>("Rc2Ecb");
        register_algorithm<Rc2Ofb>("Rc2Ofb");

        register_algorithm<Rc440>("Rc440");

        register_algorithm<SeedCbc>("SeedCbc");
        register_algorithm<SeedCfb>("SeedCfb");
        register_algorithm<SeedEcb>("SeedEcb");
        register_algorithm<SeedOfb>("SeedOfb");

        register_algorithm<Sm4Cbc>("Sm4Cbc");
        register_algorithm<Sm4Cfb>("Sm4Cfb");
        register_algorithm<Sm4Ctr>("Sm4Ctr");
        register_algorithm<Sm4Ecb>("Sm4Ecb");
        register_algorithm<Sm4Ofb>("Sm4Ofb");
    }
};
