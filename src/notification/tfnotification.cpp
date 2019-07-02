/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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

#include "tfnotification.hpp"

namespace TF
{

    namespace Foundation
    {

        Notification::Notification()
        {
            theSender = nullptr;
        }


        Notification::Notification(const notification_label &l, const notification_data &d, object_type s)
        {
            theLabel = l;
            theData = d;
            theSender = s;
        }


        Notification::Notification(const notification_label &l, object_type s)
        {
            theLabel = l;
            theSender = s;
        }


        Notification::Notification(const notification_label &l, const notification_data &d)
        {
            theLabel = l;
            theData = d;
            theSender = nullptr;
        }


        Notification::Notification(const notification_label &l)
        {
            theLabel = l;
            theSender = nullptr;
        }


        Notification::Notification(const Notification &n)
        {
            theLabel = n.theLabel;
            theData = n.theData;
            theSender = n.theSender;
        }


        Notification::Notification(Notification &&n)
        {
            theLabel = n.theLabel;
            theData = n.theData;
            theSender = n.theSender;
        }


        Notification::~Notification()
        {
        }


        Notification &Notification::operator=(const Notification &n)
        {
            if(this != &n)
            {
                theLabel = n.theLabel;
                theData = n.theData;
                theSender = n.theSender;
            }

            return *this;
        }


        Notification &Notification::operator=(Notification &&n)
        {
            theLabel = n.theLabel;
            theData = n.theData;
            theSender = n.theSender;
            return *this;
        }


        bool Notification::operator==(const Notification &n)
        {
            if(this != &n)
            {
                if(theLabel != n.theLabel)
                    return false;
                if(theData != n.theData)
                    return false;
                if(theSender != n.theSender)
                    return false;
            }

            return true;
        }


        bool Notification::operator!=(const Notification &n)
        {
            if(*this == n)
                return false;
            return true;
        }


        std::ostream &Notification::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("Notification");
                formatter->addClassMember<notification_label>("theLabel", theLabel);
                formatter->addClassMember<notification_data>("theData", theData);
                formatter->addClassMember("theSender", theSender);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream &operator<<(std::ostream &o, const Notification &n)
        {
            return n.description(o);
        }


    }    // namespace Foundation

}    // namespace TF
