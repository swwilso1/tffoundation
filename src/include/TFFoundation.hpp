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

#include "tfallocator.hpp"
#include "tfclassformatter.hpp"
#include "tfxmlclassformatter.hpp"
#include "tfformatterfactory.hpp"
#include "tftypes.hpp"
#include "tfarray.hxx"
#include "tflist.hxx"
#include "tfqueue.hxx"
#include "tfmutex.hpp"
#include "tfpair.hxx"
#include "tfmap.hxx"
#include "tfthread.hpp"
#include "tfconditionvariable.hpp"
#include "tfdata.hpp"
#include "tfthreadsafequeue.hxx"
#include "tfthreadcontroller.hpp"
#include "tflog.hpp"
#include "tfnotification.hpp"
#include "tfnotificationcenter.hpp"
#include "tfcomparison.hpp"
#include "tfendian.hpp"
#include "tfstring.hpp"
#include "tfdatetypes.hpp"
#include "tfdate.hxx"
#include "tfdatecomponent.hxx"
#include "tfdateformatter.hxx"
#include "tfdateclocks.hpp"
#include "tfenvironmentsettings.hpp"
#include "tffilemanager.hpp"
#include "tfalarmcenter.hpp"
#include "tffilepermissions.hpp"



