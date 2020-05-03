# ArxSmartPtr

C++ smart pointer-like classes for Arduino which can't use standard smart pointers

## Note

jArxSmartPtr`j is C++ smart pointer-*like* classes for Arduino.
All of the functions is not supported currently.


## Supported Boards

This library is enabled only if you use following architecture.
Please use C++ Standard Template Library for other boards.

- AVR
- MEGAAVR
- SAMD
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


## Roadmap

This library will be updated if I want to use more smart pointer interfaces on supported boards shown above.
PRs are welcome!

## License

MIT
