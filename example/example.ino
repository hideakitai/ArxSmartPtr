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


void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("===============================");
    {
        arx::shared_ptr<Base> t(new Base(1));
    }
    // Base::Constructor 1
    // Base::Destructor 1

    Serial.println("===============================");
    {
        arx::shared_ptr<void> t(new Base(2));
    }
    // Base::Constructor 2
    // Base::Destructor 2

    Serial.println("===============================");
    {
        arx::shared_ptr<Derived> a(new Derived(3));
        Serial.print("use_count = "); Serial.println(a.use_count());
        arx::shared_ptr<Base> b = a;
        Serial.print("use_count = "); Serial.println(a.use_count());
        arx::shared_ptr<void> c = a;
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
        arx::shared_ptr<Base> t1(new Base(4));
        arx::shared_ptr<Base> t2;
        {
            arx::shared_ptr<Base> t3(new Base(5));
            arx::shared_ptr<Base> t4(new Base(6));
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
        arx::shared_ptr<Base> t1 = arx::make_shared<Base>(7);
        arx::shared_ptr<Base> t2;
        {
            arx::shared_ptr<Base> t3 = arx::make_shared<Base>(8);
            arx::shared_ptr<Base> t4 = arx::make_shared<Base>(9);
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
}

void loop()
{
}
