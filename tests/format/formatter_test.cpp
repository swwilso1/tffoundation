/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2017 Tectiform Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#include <ostream>
#include <iostream>
#include <sstream>
#include "TFFoundation/TFFoundation.hpp"

#define kArrayLength 50

struct Foo
{
    char a;
    int b;
    float d;
    unsigned long array[kArrayLength];
};

std::ostream &operator<<(std::ostream &o, const Foo &f)
{
    TF::Foundation::FormatterFactory factory;
    TF::Foundation::FormatterFactory::default_formatter_type *formatter = factory.getFormatter();
    std::stringstream converter;
    converter << f.a;
    formatter->setClassName("Foo");
    formatter->addClassMember("char", "a", converter.str());
    formatter->addClassMember<int>("int", "b", f.b);
    formatter->addClassMember<float>("float", "d", f.d);
    formatter->addClassMember<unsigned long>("unsigned long", "array", f.array, kArrayLength);
    o << *formatter;
    delete formatter;
    return o;
}


int main()
{
    TF::Foundation::FormatterFactory factory;
    factory.setFormatterType(TF::Foundation::FormatterType::XMLFormatterType);
    TF::Foundation::FormatterFactory::default_formatter_type *formatter = factory.getFormatter();
    formatter->setClassName("TestClass");
    formatter->addClassTemplateType("char");
    formatter->addClassTemplateType("unsigned char");
    formatter->addClassMember<Foo>("theFoo", Foo());
    formatter->addClassMember<Foo>("Foo", "theOtherFoo", Foo());

    std::cout << *formatter << std::endl;

    delete formatter;

    return 0;
}
