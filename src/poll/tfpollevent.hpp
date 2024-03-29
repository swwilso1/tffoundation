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

#ifndef TFPOLLEVENT_HPP
#define TFPOLLEVENT_HPP

namespace TF
{

    namespace Foundation
    {

        enum class PollEvent
        {
            Read = 0x1,
            Write = 0x2,
            Except = 0x4
        };

        /**
         * Method to check whether a bitmask event type contains an event.
         * @tparam INTEGER type bitmask type
         * @param event the event mask
         * @param pevent the event
         * @return true if the @e event mask contains @e pevent and false otherwise.
         */
        template<typename INTEGER, typename = std::enable_if_t<std::is_integral<INTEGER>::value>>
        bool event_set_for(INTEGER event, PollEvent pevent)
        {
            if ((event & static_cast<INTEGER>(pevent)) == static_cast<INTEGER>(pevent))
            {
                return true;
            }
            return false;
        }

        /**
         * @brief method to set poll events in a bitmask of another type.
         * @tparam INTEGER the type
         * @param event the event var
         * @param pevent the poll event value
         * @return event with the bitmask value of @e pevent added.
         */
        template<typename INTEGER, typename = std::enable_if_t<std::is_integral<INTEGER>::value>>
        auto set_event_for(INTEGER event, PollEvent pevent) -> INTEGER
        {
            event |= static_cast<INTEGER>(pevent);
            return event;
        }

    } // namespace Foundation

} // namespace TF

#endif // TFPOLLEVENT_HPP
