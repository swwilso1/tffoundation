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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#ifndef TFDATECLOCKS_HPP
#define TFDATECLOCKS_HPP

#define NEEDS_CHRONO
#include "tfheaders.hpp"
#include "tfdate.hxx"
#include "tfdatecomponent.hxx"
#include "tfdateformatter.hxx"

namespace TF
{

    namespace Foundation
    {

        using SystemDate = Date<std::chrono::system_clock>;

        using HighResolutionDate = Date<std::chrono::high_resolution_clock>;

        using SteadyDate = Date<std::chrono::steady_clock>;


        using SystemDateComponents = DateComponents<std::chrono::system_clock>;

        using HighResolutionDateComponents = DateComponents<std::chrono::high_resolution_clock>;

        using SteadyDateComponents = DateComponents<std::chrono::steady_clock>;


        using SystemDateFormatter = DateFormatter<std::chrono::system_clock>;

        using HighResolutionDateFormatter = DateFormatter<std::chrono::high_resolution_clock>;

        using SteadyDateFormatter = DateFormatter<std::chrono::steady_clock>;

    }    // namespace Foundation

}    // namespace TF

#endif    // TFDATECLOCKS_HPP
