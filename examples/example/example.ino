#include <ArxSmartPtr.h>

struct Base
{
    int i;

    Base(int i) : i(i)
    {
        Serial.print("Base::Constructor ");
        Serial.println(i);
    }
    ~Base() // "virtual" is not required for shared_ptr
    {
        Serial.print("Base::Destructor ");
        Serial.println(i);
    }
};

struct Derived : public Base
{
    Derived(int i) : Base(i)
    {
        Serial.print("Derived::Constructor ");
        Serial.println(Base::i);
    }
    ~Derived()
    {
        Serial.print("Derived::Destructor ");
        Serial.println(Base::i);
    }
};

class Test
{
    std::shared_ptr<Derived> p;

public:
    Test(int i) : p(std::make_shared<Derived>(i))
    {
        Serial.println("Test Constructor");
        // Serial.println(*(p.get()));
    }
    ~Test()
    {
        Serial.println("Test Destructor");
    }

    std::shared_ptr<Derived> getPtr() const { return p; }
    long getCount() const { return p.use_count(); }

};

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("===============================");
    {
        std::shared_ptr<Base> t(new Base(1));
    }
    // Base::Constructor 1
    // Base::Destructor 1

    Serial.println("===============================");
    {
        std::shared_ptr<void> t(new Base(2));
    }
    // Base::Constructor 2
    // Base::Destructor 2

    Serial.println("===============================");
    {
        std::shared_ptr<Derived> a(new Derived(3));
        Serial.print("use_count = "); Serial.println(a.use_count());
        std::shared_ptr<Base> b = a;
        Serial.print("use_count = "); Serial.println(a.use_count());
        std::shared_ptr<void> c = a;
        Serial.print("use_count = "); Serial.println(a.use_count());
    }
    // Base::Constructor 3
    // Derived::Constructor 3
    // use_count = 1
    // use_count = 2
    // use_count = 3
    // Derived::Destructor 3
    // Base::Destructor 3

    Serial.println("===============================");
    {
        Serial.println("start");
        std::shared_ptr<Base> t1(new Base(4));
        std::shared_ptr<Base> t2;
        {
            std::shared_ptr<Base> t3(new Base(5));
            std::shared_ptr<Base> t4(new Base(6));
            t2 = t3;
        }
        Serial.println("end");
    }
    // start
    // Base::Constructor 4
    // Base::Constructor 5
    // Base::Constructor 6
    // Base::Destructor 6
    // end
    // Base::Destructor 5
    // Base::Destructor 4

    Serial.println("===============================");
    {
        Serial.println("start");
        std::shared_ptr<Base> t1 = std::make_shared<Base>(7);
        std::shared_ptr<Base> t2;
        {
            std::shared_ptr<Base> t3 = std::make_shared<Base>(8);
            std::shared_ptr<Base> t4 = std::make_shared<Base>(9);
			t2 = t3;
        }
        Serial.println("end");
    }
    // start
    // Base::Constructor 7
    // Base::Constructor 8
    // Base::Constructor 9
    // Base::Destructor 9
    // end
    // Base::Destructor 8
    // Base::Destructor 7

    Serial.println("===============================");
    {
        Serial.println("start");
        Test t(10);
        {
            auto p = t.getPtr();
            Serial.print("p count = ");
            Serial.println(p.use_count());
            Serial.print("t count = ");
            Serial.println(t.getCount());
            Serial.println("p deleted");
        }
        Serial.print("t count = ");
        Serial.println(t.getCount());

        Serial.println("end");
    }
    // start
    // Base::Constructor 10
    // Derived::Constructor 10
    // Test Constructor
    // p count = 2
    // t count = 2
    // p deleted
    // t count = 1
    // end
    // Test Destructor
    // Derived::Destructor 10
    // Base::Destructor 10
}

void loop()
{
}
