/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 Tectiform Inc.

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

#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tfformatter.hpp"


namespace TF
{

    namespace Foundation
    {

        template<class T>
        ArrayIterator<T>::ArrayIterator()
        {
            theLocation = 0;
            theKind = iterator_kind::End;
        }


        template<class T>
        ArrayIterator<T>::ArrayIterator(Array <value_type> &a, size_type i)
        {
            theArray = a;
            theLocation = i;
            theKind = iterator_kind::Normal;
        }


        template<class T>
        ArrayIterator<T>::ArrayIterator(const ArrayIterator &i)
        {
            theArray = i.theArray;
            theLocation = i.theLocation;
            theKind = i.theKind;
        }


        template<class T>
        ArrayIterator<T>::ArrayIterator(ArrayIterator &&i)
        {
            theArray = i.theArray;
            theLocation = i.theLocation;
            theKind = i.theKind;
        }


        template<class T>
        ArrayIterator<T>::~ArrayIterator()
        {
        }


        template<class T>
        ArrayIterator <T> &ArrayIterator<T>::operator=(const ArrayIterator &i)
        {
            if (this != &i)
            {
                theArray = i.theArray;
                theLocation = i.theLocation;
                theKind = i.theKind;
            }

            return *this;
        }


        template<class T>
        ArrayIterator <T> &ArrayIterator<T>::operator=(ArrayIterator &&i)
        {
            if (this != &i)
            {
                theArray = i.theArray;
                theLocation = i.theLocation;
                theKind = i.theKind;
            }

            return *this;
        }


        template<class T>
        bool ArrayIterator<T>::operator==(const ArrayIterator &i) const
        {
            if (this != &i)
            {
                if (theArray != i.theArray)
                    return false;
                if (theKind != i.theKind)
                    return false;
                if (theKind == iterator_kind::Normal)
                {
                    if (theLocation != i.theLocation)
                        return false;
                }
            }

            return true;
        }


        template<class T>
        bool ArrayIterator<T>::operator!=(const ArrayIterator &i) const
        {
            if (*this == i)
                return false;
            return true;
        }


        template<class T>
        bool ArrayIterator<T>::operator<(const ArrayIterator &i) const
        {
            if (this != &i)
            {
                if (theArray != i.theArray)
                    return false;

                if (theKind != i.theKind)
                {
                    if (theKind == iterator_kind::Normal && i.theKind == iterator_kind::End)
                        return true;
                    else
                        return false;
                }

                return theLocation < i.theLocation;
            }

            return false;
        }


        template<class T>
        bool ArrayIterator<T>::operator<=(const ArrayIterator &i) const
        {
            if (this != &i)
            {
                if (theArray != i.theArray)
                    return false;

                if (theKind != i.theKind)
                {
                    if (theKind == iterator_kind::Normal && i.theKind == iterator_kind::End)
                        return true;
                    else
                        return false;
                }

                return theLocation <= i.theLocation;
            }

            return true;
        }


        template<class T>
        bool ArrayIterator<T>::operator>=(const ArrayIterator &i) const
        {
            if (this != &i)
            {
                if (theArray != i.theArray)
                    return false;

                if (theKind != i.theKind)
                {
                    if (theKind == iterator_kind::Normal && i.theKind == iterator_kind::End)
                        return true;
                    else
                        return false;
                }

                return theLocation >= i.theLocation;
            }

            return true;
        }


        template<class T>
        bool ArrayIterator<T>::operator>(const ArrayIterator &i) const
        {
            if (this != &i)
            {
                if (theArray != i.theArray)
                    return false;

                if (theKind != i.theKind)
                {
                    if (theKind == iterator_kind::Normal && i.theKind == iterator_kind::End)
                        return true;
                    else
                        return false;
                }

                return theLocation > i.theLocation;
            }

            return false;
        }


        template<class T>
        ArrayIterator <T> &ArrayIterator<T>::operator++()
        {
            if (theKind == iterator_kind::Normal)
            {
                if (theLocation == (theArray.size() - 1))
                    theKind = iterator_kind::End;
                else
                    theLocation++;
            }

            return *this;
        }


        template<class T>
        ArrayIterator <T> ArrayIterator<T>::operator++(int)
        {
            ArrayIterator copyIterator(*this);

            if (theKind == iterator_kind::Normal)
            {
                if (theLocation == (theArray.size() - 1))
                    theKind = iterator_kind::End;
                else
                    theLocation++;
            }

            return copyIterator;
        }


        template<class T>
        ArrayIterator <T> &ArrayIterator<T>::operator--()
        {
            if (theKind == iterator_kind::Normal)
            {
                if (theLocation != 0)
                    theLocation--;
            } else if (theKind == iterator_kind::End && theArray.size() > 0)
                theLocation = theArray.size() - 1;

            return *this;
        }


        template<class T>
        ArrayIterator <T> ArrayIterator<T>::operator--(int)
        {
            ArrayIterator copyIterator(*this);

            if (theKind == iterator_kind::Normal)
            {
                if (theLocation != 0)
                    theLocation--;
            } else if (theKind == iterator_kind::End && theArray.size() > 0)
                theLocation = theArray.size() - 1;

            return copyIterator;
        }


        template<class T>
        ArrayIterator <T> ArrayIterator<T>::operator+(int i)
        {
            ArrayIterator copyIterator(*this);

            if (copyIterator.theKind == iterator_kind::Normal)
            {
                if ((copyIterator.theLocation + i) > (copyIterator.theArray.size() - 1))
                    copyIterator.theKind = iterator_kind::End;
                else
                    copyIterator.theLocation += i;
            }

            return copyIterator;
        }


        template<class T>
        ArrayIterator <T> ArrayIterator<T>::operator-(int i)
        {
            ArrayIterator copyIterator(*this);

            if (copyIterator.theKind == iterator_kind::Normal)
            {
                if (copyIterator.theLocation <= i)
                    copyIterator.theLocation = 0;
                else
                    copyIterator.theLocation -= i;
            } else if (copyIterator.theKind == iterator_kind::End && theArray.size() > 0)
            {
                copyIterator.theLocation = theArray.size() - 1;
                i--;

                if (copyIterator.theLocation <= i)
                    copyIterator.theLocation = 0;
                else
                    copyIterator.theLocation -= i;
            }

            return copyIterator;
        }


        template<class T>
        ArrayIterator <T> &ArrayIterator<T>::operator+=(int i)
        {
            if (theKind == iterator_kind::Normal)
            {
                if ((theLocation + i) > (theArray.size() - 1))
                    theKind = iterator_kind::End;
                else
                    theLocation += i;
            }

            return *this;
        }


        template<class T>
        ArrayIterator <T> &ArrayIterator<T>::operator-=(int i)
        {
            if (theKind == iterator_kind::Normal)
            {
                if (theLocation <= i)
                    theLocation = 0;
                else
                    theLocation -= i;
            } else if (theKind == iterator_kind::End && theArray.size() > 0)
            {
                theLocation = theArray.size() - 1;
                i--;

                if (theLocation <= i)
                    theLocation = 0;
                else
                    theLocation -= i;
            }

            return *this;
        }


        template<class T>
        typename ArrayIterator<T>::value_type ArrayIterator<T>::operator*()
        {
            return theArray[theLocation];
        }


        template<class T>
        typename ArrayIterator<T>::value_type *ArrayIterator<T>::operator->()
        {
            return *theArray.theArray + theLocation;
        }


        template<class T>
        std::ostream& ArrayIterator<T>::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("ArrayIterator");
                formatter->addClassTemplateType<value_type>();
                formatter->addClassMember<Array<T>>("theArray",theArray);
                formatter->addClassMember<size_type>("theLocation",theLocation);
                if(theKind == iterator_kind::Normal)
                    formatter->addClassMember("ArrayIteratorKind", "theKind", std::string("Normal"));
                else if(theKind == iterator_kind::End)
                    formatter->addClassMember("ArrayIteratorKind", "theKind", std::string("End"));
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        template<class T>
        std::ostream& operator<<(std::ostream &o, const ArrayIterator<T> &i)
        {
            return i.description(o);
        }


        template<class T>
        Array<T>::Array()
        {
            theArray = nullptr;
            theLength = 0;
        }


        template<class T>
        Array<T>::Array(const value_type *a, size_type l)
        {
            theArray = std::shared_ptr<value_type>(new value_type[l]);
            theLength = l;
            value_type *tmp = theArray.get();


            for(size_type i = 0; i < l; i++)
                *(tmp + i) = a[i];
        }


        template<class T>
        Array<T>::Array(const size_type &l)
        {
            theArray = nullptr;
            theLength = 0;

            if(l > 0)
            {
                theArray = std::shared_ptr<value_type>(new value_type[l]);
                theLength = l;
            }
        }


        template<class T>
        Array<T>::Array(const Array &a)
        {
            theArray = nullptr;
            theLength = 0;

            if(a.theLength > 0)
            {
                theArray = std::shared_ptr<value_type>(new value_type[a.theLength]);

                for(size_type i = 0; i < a.theLength; i++)
                    *(theArray.get() + i) = *(a.theArray.get() + i);

                theLength = a.theLength;
            }
        }


        template<class T>
        Array<T>::Array(Array &&a)
        {
            theArray = a.theArray;
            theLength = a.theLength;
            a.theArray = nullptr;
            a.theLength = 0;
        }


        template<class T>
        Array<T>::~Array()
        {
            theLength = 0;
        }


        template<class T>
        Array<T>& Array<T>::operator=(const Array &a)
        {
            if(this != &a)
            {
                if(theLength > 0)
                {
                    theLength = 0;
                }

                if(a.theLength > 0)
                {
                    theArray = std::shared_ptr<value_type>(new value_type[a.theLength]);
                    for (size_type i = 0; i < a.theLength; i++)
                        *(theArray.get() + i) = *(a.theArray.get() + i);
                    theLength = a.theLength;
                }
            }

            return *this;
        }


        template<class T>
        Array<T>& Array<T>::operator=(Array &&a)
        {
            if(this != &a)
            {
                if(theLength != 0)
                {
                    theLength = 0;
                }

                theArray = a.theArray;
                theLength = a.theLength;
                a.theArray = nullptr;
                a.theLength = 0;
            }

            return *this;
        }


        template<class T>
        bool Array<T>::operator==(const Array &a) const
        {
            if(this != &a)
            {
                if(theLength != a.theLength)
                    return false;

                for(size_type i = 0; i < theLength; i++)
                {
                    if (*(theArray.get() + i) != *(a.theArray.get() + i))
                        return false;
                }
            }

            return true;
        }


        template<class T>
        bool Array<T>::operator!=(const Array &a) const
        {
            if(*this == a)
                return false;
            return true;
        }


        template<class T>
        typename Array<T>::iterator Array<T>::begin()
        {
            if(theLength == 0)
                return end();

            return ArrayIterator<T>(*this, 0);
        }


        template<class T>
        typename Array<T>::const_iterator Array<T>::cbegin()
        {
            if(theLength == 0)
                return cend();

            const ArrayIterator<T> citer(*this, 0);
            return citer;
        }


        template<class T>
        typename Array<T>::iterator Array<T>::end()
        {
            ArrayIterator<T> iter(*this, 0);
            iter.setEnd();
            return iter;
        }


        template<class T>
        typename Array<T>::const_iterator Array<T>::cend()
        {
            ArrayIterator<T> citer(*this, 0);
            citer.setEnd();
            return citer;
        }


        template<class T>
        typename Array<T>::reference Array<T>::at(size_type i)
        {
            if(i >= theLength)
                throw std::out_of_range("Array index out of bounds");

            return *(theArray.get() + i);
        }


        template<class T>
        typename Array<T>::const_reference Array<T>::at(size_type i) const
        {
            if(i >= theLength)
                throw std::out_of_range("Array index out of bounds");

            return *(theArray.get() + i);
        }


        template<class T>
        typename Array<T>::reference Array<T>::operator[](size_type i)
        {
            return *(theArray.get() + i);
        }


        template<class T>
        typename Array<T>::const_reference Array<T>::operator[](size_type i) const
        {
            return *(theArray.get() + i);
        }


        template<class T>
        typename Array<T>::reference Array<T>::front()
        {
            return *(theArray.get());
        }


        template<class T>
        typename Array<T>::const_reference Array<T>::front() const
        {
            return *(theArray.get());
        }


        template<class T>
        typename Array<T>::reference Array<T>::back()
        {
            return *(theArray.get() + theLength - 1);
        }


        template<class T>
        typename Array<T>::const_reference Array<T>::back() const
        {
            return *(theArray.get() + theLength - 1);
        }


        template<class T>
        typename Array<T>::pointer Array<T>::data()
        {
            return theArray.get();
        }


        template<class T>
        typename Array<T>::const_pointer Array<T>::data() const
        {
            return theArray.get();
        }


        template<class T>
        constexpr bool Array<T>::empty() const
        {
            if(theLength > 0)
                return false;
            return true;
        }


        template<class T>
        constexpr typename Array<T>::size_type Array<T>::size() const
        {
            return theLength;
        }


        template<class T>
        void Array<T>::fill(const_reference value)
        {
            if(theLength > 0)
            {
                for(size_type i = 0; i < theLength; i++)
                    *(theArray.get() + i) = value;
            }
        }


        template<class T>
        void Array<T>::swap(Array &other)
        {
            std::shared_ptr<value_type> theTmpArray = other.theArray;
            size_type theTmpLength = other.theLength;
            other.theArray = theArray;
            other.theLength = theLength;
            theArray = theTmpArray;
            theLength = theTmpLength;
        }


        template<class T>
        std::ostream& Array<T>::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("Array");
                formatter->addClassTemplateType<value_type>();
                formatter->addClassMember<value_type>("theArray", theArray.get(), theLength);
                formatter->addClassMember<size_type>("theLength",theLength);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        template<class T>
        std::ostream& operator<<(std::ostream &o, const Array<T> &a)
        {
            return a.description(o);
        }

    } // Foundation

} // TF


