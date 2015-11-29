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

#include "tfplatformheaders.h"


// Standard C headers

#if defined(NEEDS_ASSERT_H)
#	if defined(HAS_ASSERT_H)
#		include <assert.h>
#	else
#		error "System C header <assert.h> required, but not available"
#	endif
#	undef NEEDS_ASSERT_H
#endif


#if defined(NEEDS_LIMITS_H)
#   if defined(HAS_LIMITS_H)
#       include <limits.h>
#   else
#       error "System C header <limits.h> required, but not available"
#   endif
#   undef NEEDS_LIMITS_H
#endif


#if defined(NEEDS_STDLIB_H)
#   if defined(HAS_STDLIB_H)
#       include <stdlib.h>
#   else
#       error "System C header <stdlib.h> required, but not available"
#   endif
#   undef NEEDS_STDLIB_H
#endif


// Standard C++ headers

#if defined(NEEDS_CSTDDEF)
#	if defined(HAS_CSTDDEF)
#		include <cstddef>
#	else
#		error "System C++ header <cstddef> required, but not available"
#	endif
#	undef NEEDS_CSTDDEF
#endif

#if defined(NEEDS_LIST)
#	if defined(HAS_LIST)
#		include <list>
#	else
#		error "System C++ header <list> required, but not available."
#	endif
#	undef NEEDS_LIST
#endif

#if defined(NEEDS_MAP)
#	if defined(HAS_MAP)
#		include <map>
#	else
#		error "System C++ header <map> required. but not available."
#	endif
#	undef NEEDS_MAP
#endif

#if defined(NEEDS_MUTEX)
#   if defined(HAS_MUTEX)
#       include <mutex>
#   else
#       error "System C++ header <mutex> required, but not available"
#   endif
#   undef NEEDS_MUTEX
#endif


#if defined(NEEDS_NEW)
#   if defined(HAS_NEW)
#       include <new>
#   else
#       error "System C++ header <new> required, but not available"
#   endif
#   undef NEEDS_NEW
#endif

#if defined(NEEDS_OSTREAM)
#	if defined(HAS_OSTREAM)
#		include <ostream>
#	else
#		error "System C++ header <ostream> required, but not available"
#	endif
#	undef NEEDS_OSTREAM
#endif

#if defined(NEEDS_SSTREAM)
#	if defined(HAS_SSTREAM)
#		include <sstream>
#	else
#		error "System C++ header <sstream> required, but not available"
#	endif
#	undef NEEDS_SSTREAM
#endif

#if defined(NEEDS_STRING)
#   if defined(HAS_STRING)
#       include <string>
#   else
#       error "System C++ header <string> required, but not available"
#   endif
#   undef NEEDS_STRING
#endif

#if defined(NEEDS_TYPEINFO)
#	if defined(HAS_TYPEINFO)
#		include <typeinfo>
#	else
#		error "System C++ header <typeinfo> required, but not avavilable"
#	endif
#	undef NEEDS_TYPEINFO
#endif

#if defined(NEEDS_UTILITY)
#   if defined(HAS_UTILITY)
#       include <utility>
#   else
#       error "System C++ header <utility> required, but not available"
#   endif
#   undef NEEDS_UTILITY
#endif

#if defined(NEEDS_VECTOR)
#	if defined(HAS_VECTOR)
#		include <vector>
#	else
#		error "System C++ header <vector> required, but not available"
#	endif
#	undef NEEDS_VECTOR
#endif


