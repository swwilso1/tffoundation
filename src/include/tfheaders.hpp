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

#include "tfplatformheaders.h"


// Standard C headers

#if defined(NEEDS_ASSERT_H)
#    if defined(HAS_ASSERT_H)
#        include <assert.h>
#    else
#        error "System C header <assert.h> required, but not available"
#    endif
#    undef NEEDS_ASSERT_H
#endif


#if defined(NEEDS_CTYPE_H)
#    if defined(HAS_CTYPE_H)
#        include <ctype.h>
#    else
#        error "System C header <ctype.h> required, but not available"
#    endif
#    undef NEEDS_CTYPE_H
#endif


#if defined(NEEDS_ERRNO_H)
#    if defined(HAS_ERRNO_H)
#        include <errno.h>
#    else
#        error "System C header <errno.h> required, but not available"
#    endif
#    undef NEEDS_ERRNO_H
#endif


#if defined(NEEDS_FENV_H)
#    if defined(HAS_FENV_H)
#        include <fenv.h>
#    else
#        error "System C header <fenv.h> required, but not available"
#    endif
#    undef NEEDS_FENV_H
#endif


#if defined(NEEDS_FLOAT_H)
#    if defined(HAS_FLOAT_H)
#        include <float.h>
#    else
#        error "System C header <float.h> required, but not available"
#    endif
#    undef NEEDS_FLOAT_H
#endif


#if defined(NEEDS_INTTYPES_H)
#    if defined(HAS_INTTYPES_H)
#        include <inttypes.h>
#    else
#        error "System C header <inttypes.h> required, but not available"
#    endif
#    undef NEEDS_INTTYPES_H
#endif


#if defined(NEEDS_ISO646_H)
#    if defined(HAS_ISO646_H)
#        include <iso646.h>
#    else
#        error "System C header <iso646.h> required, but not available"
#    endif
#    undef NEEDS_ISO646_H
#endif


#if defined(NEEDS_LIMITS_H)
#    if defined(HAS_LIMITS_H)
#        include <limits.h>
#    else
#        error "System C header <limits.h> required, but not available"
#    endif
#    undef NEEDS_LIMITS_H
#endif


#if defined(NEEDS_LOCALE_H)
#    if defined(HAS_LOCALE_H)
#        include <locale.h>
#    else
#        error "System C header <locale.h> required, but not available"
#    endif
#    undef NEEDS_LOCALE_H
#endif


#if defined(NEEDS_MATH_H)
#    if defined(HAS_MATH_H)
#        include <math.h>
#    else
#        error "System C header <math.h> required, but not available"
#    endif
#    undef NEEDS_MATH_H
#endif


#if defined(NEEDS_SETJMP_H)
#    if defined(HAS_SETJMP_H)
#        include <setjmp.h>
#    else
#        error "System C header <setjmp.h> required, but not available"
#    endif
#    undef NEEDS_SETJMP_H
#endif


#if defined(NEEDS_SIGNAL_H)
#    if defined(HAS_SIGNAL_H)
#        include <signal.h>
#    else
#        error "System C header <signal.h> required, but not available"
#    endif
#    undef NEEDS_SIGNAL_H
#endif


#if defined(NEEDS_STDALIGN_H)
#    if defined(HAS_STDALIGN_H)
#        include <stdalign.h>
#    else
#        error "System C header <stdalign.h> required, but not available"
#    endif
#    undef NEEDS_STDALIGN_H
#endif


#if defined(NEEDS_STDARG_H)
#    if defined(HAS_STDARG_H)
#        include <stdarg.h>
#    else
#        error "System C header <stdarg.h> required, but not available"
#    endif
#    undef NEEDS_STDARG_H
#endif


#if defined(NEEDS_STDATOMIC_H)
#    if defined(HAS_STDATOMIC_H)
#        include <stdatomic.h>
#    else
#        error "System C header <stdatomic.h> required, but not available"
#    endif
#    undef NEEDS_STDATOMIC_H
#endif


#if defined(NEEDS_STDBOOL_H)
#    if defined(HAS_STDBOOL_H)
#        include <stdbool.h>
#    else
#        error "System C header <stdbool.h> required, but not available"
#    endif
#    undef NEEDS_STDBOOL_H
#endif


#if defined(NEEDS_STDDEF_H)
#    if defined(HAS_STDDEF_H)
#        include <stddef.h>
#    else
#        error "System C header <stddef.h> required, but not available"
#    endif
#    undef NEEDS_STDDEF_H
#endif


#if defined(NEEDS_STDINT_H)
#    if defined(HAS_STDINT_H)
#        include <stdint.h>
#    else
#        error "System C header <stdint.h> required, but not available"
#    endif
#    undef NEEDS_STDINT_H
#endif


#if defined(NEEDS_STDIO_H)
#    if defined(HAS_STDIO_H)
#        include <stdio.h>
#    else
#        error "System C header <stdio.h> required, but not available"
#    endif
#    undef NEEDS_STDIO_H
#endif


#if defined(NEEDS_STDLIB_H)
#    if defined(HAS_STDLIB_H)
#        include <stdlib.h>
#    else
#        error "System C header <stdlib.h> required, but not available"
#    endif
#    undef NEEDS_STDLIB_H
#endif


#if defined(NEEDS_STDNORETURN_H)
#    if defined(HAS_STDNORETURN_H)
#        include <stdnoreturn.h>
#    else
#        error "System C header <stdnoreturn.h> required, but not available"
#    endif
#    undef NEEDS_STDNORETURN_H
#endif


#if defined(NEEDS_STRING_H)
#    if defined(HAS_STRING_H)
#        include <string.h>
#    else
#        error "System C header <string.h> required, but not available"
#    endif
#    undef NEEDS_STRING_H
#endif


#if defined(NEEDS_TGMATH_H)
#    if defined(HAS_TGMATH_H)
#        include <tgmath.h>
#    else
#        error "System C header <tgmath.h> required, but not available"
#    endif
#    undef NEEDS_TGMATH_H
#endif


#if defined(NEEDS_THREADS_H)
#    if defined(HAS_THREADS_H)
#        include <threads.h>
#    else
#        error "System C header <threads.h> required, but not available"
#    endif
#    undef NEEDS_THREADS_H
#endif


#if defined(NEEDS_TIME_H)
#    if defined(HAS_TIME_H)
#        include <time.h>
#    else
#        error "System C header <time.h> required, but not available"
#    endif
#    undef NEEDS_TIME_H
#endif


#if defined(NEEDS_UCHAR_H)
#    if defined(HAS_UCHAR_H)
#        include <uchar.h>
#    else
#        error "System C header <uchar.h> required, but not available"
#    endif
#    undef NEEDS_UCHAR_H
#endif


#if defined(NEEDS_WCHAR_H)
#    if defined(HAS_WCHAR_H)
#        include <wchar.h>
#    else
#        error "System C header <wchar.h> required, but not available"
#    endif
#    undef NEEDS_WCHAR_H
#endif


#if defined(NEEDS_WCTYPE_H)
#    if defined(HAS_WCTYPE_H)
#        include <wctype.h>
#    else
#        error "System C header <wctype.h> required, but not available"
#    endif
#    undef NEEDS_WCTYPE_H
#endif


// Standard C++ headers

#if defined(NEEDS_ANY)
#    if defined(HAS_ANY)
#        include <any>
#    else
#        error "System C header <any> required, but not available"
#    endif
#    undef NEEDS_ANY
#endif


#if defined(NEEDS_ALGORITHM)
#    if defined(HAS_ALGORITHM)
#        include <algorithm>
#    else
#        error "System C header <algorithm> required, but not available"
#    endif
#    undef NEEDS_ALGORITHM
#endif


#if defined(NEEDS_ARRAY)
#    if defined(HAS_ARRAY)
#        include <array>
#    else
#        error "System C header <array> required, but not available"
#    endif
#    undef NEEDS_ARRAY
#endif


#if defined(NEEDS_ATOMIC)
#    if defined(HAS_ATOMIC)
#        include <atomic>
#    else
#        error "System C header <atomic> required, but not available"
#    endif
#    undef NEEDS_ATOMIC
#endif


#if defined(NEEDS_BITSET)
#    if defined(HAS_BITSET)
#        include <bitset>
#    else
#        error "System C header <bitset> required, but not available"
#    endif
#    undef NEEDS_BITSET
#endif


#if defined(NEEDS_CASSERT)
#    if defined(HAS_CASSERT)
#        include <cassert>
#    else
#        error "System C header <cassert> required, but not available"
#    endif
#    undef NEEDS_CASSERT
#endif


#if defined(NEEDS_CCTYPE)
#    if defined(HAS_CCTYPE)
#        include <cctype>
#    else
#        error "System C header <cctype> required, but not available"
#    endif
#    undef NEEDS_CCTYPE
#endif


#if defined(NEEDS_CERRNO)
#    if defined(HAS_CERRNO)
#        include <cerrno>
#    else
#        error "System C header <cerrno> required, but not available"
#    endif
#    undef NEEDS_CERRNO
#endif


#if defined(NEEDS_CFENV)
#    if defined(HAS_CFENV)
#        include <cfenv>
#    else
#        error "System C header <cfenv> required, but not available"
#    endif
#    undef NEEDS_CFENV
#endif


#if defined(NEEDS_CFLOAT)
#    if defined(HAS_CFLOAT)
#        include <cfloat>
#    else
#        error "System C header <cfloat> required, but not available"
#    endif
#    undef NEEDS_CFLOAT
#endif


#if defined(NEEDS_CHRONO)
#    if defined(HAS_CHRONO)
#        include <chrono>
#    else
#        error "System C header <chrono> required, but not available"
#    endif
#    undef NEEDS_CHRONO
#endif


#if defined(NEEDS_CINTTYPES)
#    if defined(HAS_CINTTYPES)
#        include <cinttypes>
#    else
#        error "System C header <cinttypes> required, but not available"
#    endif
#    undef NEEDS_CINTTYPES
#endif


#if defined(NEEDS_CLIMITS)
#    if defined(HAS_CLIMITS)
#        include <climits>
#    else
#        error "System C header <climits> required, but not available"
#    endif
#    undef NEEDS_CLIMITS
#endif


#if defined(NEEDS_CLOCALE)
#    if defined(HAS_CLOCALE)
#        include <clocale>
#    else
#        error "System C header <clocale> required, but not available"
#    endif
#    undef NEEDS_CLOCALE
#endif


#if defined(NEEDS_CMATH)
#    if defined(HAS_CMATH)
#        include <cmath>
#    else
#        error "System C header <cmath> required, but not available"
#    endif
#    undef NEEDS_CMATH
#endif


#if defined(NEEDS_CODECVT)
#    if defined(HAS_CODECVT)
#        include <codecvt>
#    else
#        error "System C header <codecvt> required, but not available"
#    endif
#    undef NEEDS_CODECVT
#endif


#if defined(NEEDS_COMPLEX)
#    if defined(HAS_COMPLEX)
#        include <complex>
#    else
#        error "System C header <complex> required, but not available"
#    endif
#    undef NEEDS_COMPLEX
#endif


#if defined(NEEDS_CONDITION_VARIABLE)
#    if defined(HAS_CONDITION_VARIABLE)
#        include <condition_variable>
#    else
#        error "System C header <condition_variable> required, but not available"
#    endif
#    undef NEEDS_CONDITION_VARIABLE
#endif


#if defined(NEEDS_CSTDIO)
#    if defined(HAS_CSTDIO)
#        include <cstdio>
#    else
#        error "System C header <cstdio> required, but not available"
#    endif
#    undef NEEDS_CSTDIO
#endif


#if defined(NEEDS_CSTDLIB)
#    if defined(HAS_CSTDLIB)
#        include <cstdlib>
#    else
#        error "System C header <cstdlib> required, but not available"
#    endif
#    undef NEEDS_CSTDLIB
#endif


#if defined(NEEDS_CSIGNAL)
#    if defined(HAS_CSIGNAL)
#        include <csignal>
#    else
#        error "System header <csignal> required, but not available"
#    endif
#    undef NEEDS_CSIGNAL
#endif


#if defined(NEEDS_CSETJMP)
#    if defined(HAS_CSETJMP)
#        include <csetjmp>
#    else
#        error "System header <csetjmp> required, but not available"
#    endif
#    undef NEEDS_CSETJMP
#endif


#if defined(NEEDS_CSTDARG)
#    if defined(HAS_CSTDARG)
#        include <cstdarg>
#    else
#        error "System header <cstdarg> required, but not available"
#    endif
#    undef NEEDS_CSTDARG
#endif


#if defined(NEEDS_CSTDDEF)
#    if defined(HAS_CSTDDEF)
#        include <cstddef>
#    else
#        error "System C++ header <cstddef> required, but not available"
#    endif
#    undef NEEDS_CSTDDEF
#endif


#if defined(NEEDS_CSTDINT)
#    if defined(HAS_CSTDINT)
#        include <cstdint>
#    else
#        error "System header <cstdint> required, but not available"
#    endif
#    undef NEEDS_CSTDINT
#endif


#if defined(NEEDS_CSTRING)
#    if defined(HAS_CSTRING)
#        include <cstring>
#    else
#        error "System header <cstring> required, but not available"
#    endif
#    undef NEEDS_CSTRING
#endif


#if defined(NEEDS_CTIME)
#    if defined(HAS_CTIME)
#        include <ctime>
#    else
#        error "System header <ctime> required, but not available"
#    endif
#    undef NEEDS_CTIME
#endif


#if defined(NEEDS_CUCHAR)
#    if defined(HAS_CUCHAR)
#        include <cuchar>
#    else
#        error "System header <cuchar> required, but not available"
#    endif
#    undef NEEDS_CUCHAR
#endif


#if defined(NEEDS_CWCHAR)
#    if defined(HAS_CWCHAR)
#        include <cwchar>
#    else
#        error "System header <cwchar> required, but not available"
#    endif
#    undef NEEDS_CWCHAR
#endif


#if defined(NEEDS_CWCTYPE)
#    if defined(HAS_CWCTYPE)
#        include <cwctype>
#    else
#        error "System header <cwctype> required, but not available"
#    endif
#    undef NEEDS_CWCTYPE
#endif


#if defined(NEEDS_DEQUE)
#    if defined(HAS_DEQUE)
#        include <deque>
#    else
#        error "System header <deque> required, but not available"
#    endif
#    undef NEEDS_DEQUE
#endif


#if defined(NEEDS_EXCEPTION)
#    if defined(HAS_EXCEPTION)
#        include <exception>
#    else
#        error "System header <exception> required, but not available"
#    endif
#    undef NEEDS_EXCEPTION
#endif


#if defined(NEEDS_EXECUTION)
#    if defined(HAS_EXECUTION)
#        include <execution>
#    else
#        error "System header <execution> required, but not available"
#    endif
#    undef NEEDS_EXECUTION
#endif


#if defined(NEEDS_FILESYSTEM)
#    if defined(HAS_FILESYSTEM)
#        include <filesystem>
#    else
#        error "System header <filesystem> required, but not available"
#    endif
#    undef NEEDS_FILESYSTEM
#endif


#if defined(NEEDS_FORWARD_LIST)
#    if defined(HAS_FORWARD_LIST)
#        include <forward_list>
#    else
#        error "System header <forward_list> required, but not available"
#    endif
#    undef NEEDS_FORWARD_LIST
#endif


#if defined(NEEDS_FSTREAM)
#    if defined(HAS_FSTREAM)
#        include <fstream>
#    else
#        error "System header <fstream> required, but not available"
#    endif
#    undef NEEDS_FSTREAM
#endif


#if defined(NEEDS_FUNCTIONAL)
#    if defined(HAS_FUNCTIONAL)
#        include <functional>
#    else
#        error "System header <functional> required, but not available"
#    endif
#    undef NEEDS_FUNCTIONAL
#endif


#if defined(NEEDS_FUTURE)
#    if defined(HAS_FUTURE)
#        include <future>
#    else
#        error "System header <future> required, but not available"
#    endif
#    undef NEEDS_FUTURE
#endif


#if defined(NEEDS_INITIALIZER_LIST)
#    if defined(HAS_INITIALIZER_LIST)
#        include <initializer_list>
#    else
#        error "System header <initializer_list> required, but not available"
#    endif
#    undef NEEDS_INITIALIZER_LIST
#endif


#if defined(NEEDS_IOMANIP)
#    if defined(HAS_IOMANIP)
#        include <iomanip>
#    else
#        error "System header <iomanip> required, but not available"
#    endif
#    undef NEEDS_IOMANIP
#endif


#if defined(NEEDS_IOS)
#    if defined(HAS_IOS)
#        include <ios>
#    else
#        error "System header <ios> required, but not available"
#    endif
#    undef NEEDS_IOS
#endif


#if defined(NEEDS_IOSFWD)
#    if defined(HAS_IOSFWD)
#        include <iosfwd>
#    else
#        error "System header <iosfwd> required, but not available"
#    endif
#    undef NEEDS_IOSFWD
#endif


#if defined(NEEDS_IOSTREAM)
#    if defined(HAS_IOSTREAM)
#        include <iostream>
#    else
#        error "System header <iostream> required, but not available"
#    endif
#    undef NEEDS_IOSTREAM
#endif


#if defined(NEEDS_ISTREAM)
#    if defined(HAS_ISTREAM)
#        include <istream>
#    else
#        error "System header <istream> required, but not available"
#    endif
#    undef NEEDS_ISTREAM
#endif


#if defined(NEEDS_ITERATOR)
#    if defined(HAS_ITERATOR)
#        include <iterator>
#    else
#        error "System header <iterator> required, but not available"
#    endif
#    undef NEEDS_ITERATOR
#endif


#if defined(NEEDS_LIMITS)
#    if defined(HAS_LIMITS)
#        include <limits>
#    else
#        error "System header <limits> required, but not available"
#    endif
#    undef NEEDS_LIMITS
#endif


#if defined(NEEDS_LIST)
#    if defined(HAS_LIST)
#        include <list>
#    else
#        error "System C++ header <list> required, but not available."
#    endif
#    undef NEEDS_LIST
#endif


#if defined(NEEDS_LOCALE)
#    if defined(HAS_LOCALE)
#        include <locale>
#    else
#        error "System header <locale> required, but not available"
#    endif
#    undef NEEDS_LOCALE
#endif


#if defined(NEEDS_MAP)
#    if defined(HAS_MAP)
#        include <map>
#    else
#        error "System C++ header <map> required. but not available."
#    endif
#    undef NEEDS_MAP
#endif


#if defined(NEEDS_MEMORY)
#    if defined(HAS_MEMORY)
#        include <memory>
#    else
#        error "System header <memory> required, but not available"
#    endif
#    undef NEEDS_MEMORY
#endif


#if defined(NEEDS_MUTEX)
#    if defined(HAS_MUTEX)
#        include <mutex>
#    else
#        error "System C++ header <mutex> required, but not available"
#    endif
#    undef NEEDS_MUTEX
#endif


#if defined(NEEDS_NEW)
#    if defined(HAS_NEW)
#        include <new>
#    else
#        error "System C++ header <new> required, but not available"
#    endif
#    undef NEEDS_NEW
#endif


#if defined(NEEDS_NUMERIC)
#    if defined(HAS_NUMERIC)
#        include <numeric>
#    else
#        error "System header <numeric> required, but not available"
#    endif
#    undef NEEDS_NUMERIC
#endif


#if defined(NEEDS_OPTIONAL)
#    if defined(HAS_OPTIONAL)
#        include <optional>
#    else
#        error "System header <optional> required, but not available"
#    endif
#    undef NEEDS_OPTIONAL
#endif


#if defined(NEEDS_OSTREAM)
#    if defined(HAS_OSTREAM)
#        include <ostream>
#    else
#        error "System C++ header <ostream> required, but not available"
#    endif
#    undef NEEDS_OSTREAM
#endif


#if defined(NEEDS_QUEUE)
#    if defined(HAS_QUEUE)
#        include <queue>
#    else
#        error "System header <queue> required, but not available"
#    endif
#    undef NEEDS_QUEUE
#endif


#if defined(NEEDS_RANDOM)
#    if defined(HAS_RANDOM)
#        include <random>
#    else
#        error "System header <random> required, but not available"
#    endif
#    undef NEEDS_RANDOM
#endif


#if defined(NEEDS_RATIO)
#    if defined(HAS_RATIO)
#        include <ratio>
#    else
#        error "System header <ratio> required, but not available"
#    endif
#    undef NEEDS_RATIO
#endif


#if defined(NEEDS_REGEX)
#    if defined(HAS_REGEX)
#        include <regex>
#    else
#        error "System header <regex> required, but not available"
#    endif
#    undef NEEDS_REGEX
#endif


#if defined(NEEDS_SCOPED_ALLOCATOR)
#    if defined(HAS_SCOPED_ALLOCATOR)
#        include <scoped_allocator>
#    else
#        error "System header <scoped_allocator> required, but not available"
#    endif
#    undef NEEDS_SCOPED_ALLOCATOR
#endif


#if defined(NEEDS_SET)
#    if defined(HAS_SET)
#        include <set>
#    else
#        error "System header <set> required, but not available"
#    endif
#    undef NEEDS_SET
#endif


#if defined(NEEDS_SHARED_MUTEX)
#    if defined(HAS_SHARED_MUTEX)
#        include <shared_mutex>
#    else
#        error "System header <shared_mutex> required, but not available"
#    endif
#    undef NEEDS_SHARED_MUTEX
#endif


#if defined(NEEDS_SSTREAM)
#    if defined(HAS_SSTREAM)
#        include <sstream>
#    else
#        error "System C++ header <sstream> required, but not available"
#    endif
#    undef NEEDS_SSTREAM
#endif


#if defined(NEEDS_STACK)
#    if defined(HAS_STACK)
#        include <stack>
#    else
#        error "System header <stack> required, but not available"
#    endif
#    undef NEEDS_STACK
#endif


#if defined(NEEDS_STDEXCEPT)
#    if defined(HAS_STDEXCEPT)
#        include <stdexcept>
#    else
#        error "System header <stdexcept> required, but not available"
#    endif
#    undef NEEDS_STDEXCEPT
#endif


#if defined(NEEDS_STREAMBUF)
#    if defined(HAS_STREAMBUF)
#        include <streambuf>
#    else
#        error "System header <streambuf> required, but not available"
#    endif
#    undef NEEDS_STREAMBUF
#endif


#if defined(NEEDS_STRING)
#    if defined(HAS_STRING)
#        include <string>
#    else
#        error "System C++ header <string> required, but not available"
#    endif
#    undef NEEDS_STRING
#endif


#if defined(NEEDS_STRING_VIEW)
#    if defined(HAS_STRING_VIEW)
#        include <string_view>
#    else
#        error "System header <string_view> required, but not available"
#    endif
#    undef NEEDS_STRING_VIEW
#endif


#if defined(NEEDS_STRSTREAM)
#    if defined(HAS_STRSTREAM)
#        include <strstream>
#    else
#        error "System header <strstream> required, but not available"
#    endif
#    undef NEEDS_STRSTREAM
#endif


#if defined(NEEDS_SYSTEM_ERROR)
#    if defined(HAS_SYSTEM_ERROR)
#        include <system_error>
#    else
#        error "System header <system_error> required, but not available"
#    endif
#    undef NEEDS_SYSTEM_ERROR
#endif


#if defined(NEEDS_THREAD)
#    if defined(HAS_THREAD)
#        include <thread>
#    else
#        error "System header <thread> required, but not available"
#    endif
#    undef NEEDS_THREAD
#endif


#if defined(NEEDS_TUPLE)
#    if defined(HAS_TUPLE)
#        include <tuple>
#    else
#        error "System header <tuple> required, but not available"
#    endif
#    undef NEEDS_TUPLE
#endif


#if defined(NEEDS_TYPEINFO)
#    if defined(HAS_TYPEINFO)
#        include <typeinfo>
#    else
#        error "System C++ header <typeinfo> required, but not avavilable"
#    endif
#    undef NEEDS_TYPEINFO
#endif


#if defined(NEEDS_TYPEINDEX)
#    if defined(HAS_TYPEINDEX)
#        include <typeindex>
#    else
#        error "System header <typeindex> required, but not available"
#    endif
#    undef NEEDS_TYPEINDEX
#endif


#if defined(NEEDS_TYPE_TRAITS)
#    if defined(HAS_TYPE_TRAITS)
#        include <type_traits>
#    else
#        error "System header <type_traits> required, but not available"
#    endif
#    undef NEEDS_TYPE_TRAITS
#endif


#if defined(NEEDS_UNORDERED_MAP)
#    if defined(HAS_UNORDERED_MAP)
#        include <unordered_map>
#    else
#        error "System header <unordered_map> required, but not available"
#    endif
#    undef NEEDS_UNORDERED_MAP
#endif


#if defined(NEEDS_UNORDERED_SET)
#    if defined(HAS_UNORDERED_SET)
#        include <unordered_set>
#    else
#        error "System header <unordered_set> required, but not available"
#    endif
#    undef NEEDS_UNORDERED_SET
#endif


#if defined(NEEDS_UTILITY)
#    if defined(HAS_UTILITY)
#        include <utility>
#    else
#        error "System C++ header <utility> required, but not available"
#    endif
#    undef NEEDS_UTILITY
#endif


#if defined(NEEDS_VALARRAY)
#    if defined(HAS_VALARRAY)
#        include <valarray>
#    else
#        error "System header <valarray> required, but not available"
#    endif
#    undef NEEDS_VALARRAY
#endif


#if defined(NEEDS_VARIANT)
#    if defined(HAS_VARIANT)
#        include <variant>
#    else
#        error "System header <variant> required, but not available"
#    endif
#    undef NEEDS_VARIANT
#endif


#if defined(NEEDS_VECTOR)
#    if defined(HAS_VECTOR)
#        include <vector>
#    else
#        error "System C++ header <vector> required, but not available"
#    endif
#    undef NEEDS_VECTOR
#endif


#if defined(NEEDS_POLL_H)
#    if defined(HAS_POLL_H)
#        include <poll.h>
#    else
#        error "Platform specific header <poll.h> required, but not available"
#    endif
#    undef NEEDS_POLL_H
#endif


#if defined(NEEDS_SYS_SELECT_H)
#    if defined(HAS_SYS_SELECT_H)
#        include <sys/select.h>
#    else
#        error "Platform specific header <sys/select.h> required, but not available"
#    endif
#    undef NEEDS_SYS_SELECT_H
#endif
