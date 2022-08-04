/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

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

#ifndef TFBASICPROGRESSNOTIFIER_HXX
#define TFBASICPROGRESSNOTIFIER_HXX

#define NEEDS_TYPE_TRAITS
#define NEEDS_FUNCTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF::Foundation
{

    /**
     * BasicProgressNotifier might not need to exists.  All it does is provide a sliver of a semantic wrapper
     * around just a callback function.  It seems unnecessary for that, but for the time being, I'm going to
     * keep it in the library.
     * @tparam THING the type of the thing used for notification.
     */
    template<typename THING>
    class BasicProgressNotifier : public AllocatorInterface
    {
    public:
        using thing_type = THING;

        using callback_type = std::function<void(const thing_type & n)>;

        explicit BasicProgressNotifier(callback_type callback = nullptr) : m_callback{callback} {}

        void set_callback(callback_type callback)
        {
            m_callback = callback;
        }

        void notify(const thing_type & n)
        {
            if (m_callback)
            {
                m_callback(n);
            }
        }

    private:
        callback_type m_callback{};
    };

} // namespace TF::Foundation

#endif // TFBASICPROGRESSNOTIFIER_HXX
