# ArxSmartPtr

C++ smart pointer-like classes for Arduino which can't use standard smart pointers

## Note

`ArxSmartPtr` is C++ smart pointer-__like__ classes for Arduino.
All of the functions is not supported currently.


## Supported Boards

This library is currently enabled only if you use following architecture.
Please use C++ Standard Template Library for other boards.

- AVR (Uno, Nano, Mega, etc.)
- MEGAAVR (Uno WiFi, Nano Ecery, etc.)
- SAMD (Zero, MKR, M0, etc.)
- SPRESENSE


## Usage

Please see example for more information.

``` C++
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
```

## APIs

``` C++
T* get() const
long use_count() const
explicit operator bool() const
reference operator*() const
T* operator->() const

// just an alias of shared_ptr<T>(new T())
template<typename T, typename... Args>
inline shared_ptr<T> make_shared(Args&&... args)
```

## Roadmap

This library will be updated if I want to use more smart pointer interfaces on supported boards shown above.
PRs are welcome!

## License

MIT
