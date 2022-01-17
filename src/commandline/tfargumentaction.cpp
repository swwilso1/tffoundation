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

#include "tfargumentaction.hpp"

namespace TF
{

    namespace Foundation
    {

        std::ostream &operator<<(std::ostream &o, const ArgumentAction &action)
        {
            switch(action)
            {
                case ArgumentAction::Store:
                    o << "Store";
                    break;
                case ArgumentAction::StoreConst:
                    o << "StoreConst";
                    break;
                case ArgumentAction::StoreFalse:
                    o << "StoreFalse";
                    break;
                case ArgumentAction::StoreTrue:
                    o << "StoreTrue";
                    break;
                case ArgumentAction::Append:
                    o << "Append";
                    break;
                case ArgumentAction::AppendConst:
                    o << "AppendConst";
                    break;
                case ArgumentAction::Count:
                    o << "Count";
                    break;
            }

            return o;
        }

    }    // namespace Foundation

}    // namespace TF
