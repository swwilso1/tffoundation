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

#ifndef TFFORMATTERFACTORY_HPP
#define TFFORMATTERFACTORY_HPP 1

#include "tfallocator.hpp"
#include "tftypes.hpp"
#include "tfclassformatter.hpp"
#include "tfxmlclassformatter.hpp"
#include "tflogxmlclassformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        enum class FormatterType
        {
            DefaultFormatterType,
            XMLFormatterType,
            LogXMLFormatterType
        };


        class FormatterFactory : public AllocatorInterface
        {
        public:
            /** @brief default formatter type */
            using default_formatter_type = ClassFormatter;

            /** @brief xml formatter type */
            using xml_formatter_type = XMLClassFormatter;

            /** @brief log xml formatter type */
            using log_xml_formatter_type = LogXMLClassFormatter;

            /** @brief type for indicating which formatter to select. */
            using formatter_type = FormatterType;

            /**
             *  @brief function to set the factory formatter type.
             *  @param t the type of formatter to enable.
             */
            static void setFormatterType(formatter_type t)
            {
                formatterType = t;
            }

            /**
             *  @brief function to get the type of formatter enabled in the factor.
             *  @return the type of the formatter.
             */
            static formatter_type getFormatterType(void)
            {
                return formatterType;
            }

            /**
             *  @brief function to get a object of the current formatter type.
             *  @return a pointer to an object of the current formatter type.
             */
            static default_formatter_type *getFormatter()
            {
                default_formatter_type *theFormatter = nullptr;
                switch(formatterType)
                {
                    case FormatterType::XMLFormatterType:
                        theFormatter = dynamic_cast<default_formatter_type *>(new xml_formatter_type());
                        break;
                    case FormatterType::LogXMLFormatterType:
                        theFormatter = dynamic_cast<default_formatter_type *>(new log_xml_formatter_type());
                        break;
                    default:
                        theFormatter = new default_formatter_type();
                }

                return theFormatter;
            }

        private:
            /** @brief the current formatterType used by the formatter */
            static formatter_type formatterType;
        };

    }    // namespace Foundation

}    // namespace TF

#endif    // TFFORMATTERFACTORY_HPP
