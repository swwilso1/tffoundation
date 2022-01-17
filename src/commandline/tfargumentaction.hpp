/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 to 2022 Tectiform Inc.

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
SOFTWARE.


******************************************************************************/

#ifndef TFARGUMENTACTION_HPP
#define TFARGUMENTACTION_HPP

#define NEEDS_OSTREAM
#include "tfheaders.hpp"

namespace TF
{

    namespace Foundation
    {
        /**
         * @brief Enum type used to indicate the action of a command-line argument
         * in the @e ArgumentParser class.
         */
        enum class ArgumentAction
        {
            /**
             * Basic command line action of storing a value from an argument.
             */
            Store,

            /**
             * Command line action of storing a constant value from a command-line argument.
             */
            StoreConst,

            /**
             * Command line action of storing a boolean true value from a command-line argument.
             */
            StoreTrue,

            /**
             * Command line action of storing a boolean false value from a command-line argument.
             */
            StoreFalse,

            /**
             * Command line action of appending a command-line argument value to a list of values.
             */
            Append,

            /**
             * Command line action for appending a constant value to a list of values.
             */
            AppendConst,

            /**
             * Command line action that counts the number of command-line arguments
             */
            Count
        };

        /**
         * @brief overloaded << operator for writing an ArgumentAction to a stream
         * @param o the ostream object
         * @param action the action
         * @return the ostream object @e o.
         */
        std::ostream &operator<<(std::ostream &o, const ArgumentAction &action);

    }    // namespace Foundation

}    // namespace TF

#endif    // TFARGUMENTACTION_HPP
