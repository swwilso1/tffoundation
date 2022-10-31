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

#include "tfplatform.hpp"
#include "tfallocator.hpp"
#include "tfclassformatter.hpp"
#include "tfxmlclassformatter.hpp"
#include "tfformatterfactory.hpp"
#include "tftypes.hpp"
#include "tfarray.hxx"
#include "tfbase64.hpp"
#include "tflist.hxx"
#include "tfqueue.hxx"
#include "tfmutex.hpp"
#include "tfpair.hxx"
#include "tfmap.hxx"
#include "tfunordered_map.hxx"
#include "tfthread.hpp"
#include "tfthreadpool.hpp"
#include "tfconditionvariable.hpp"
#include "tfdata.hpp"
#include "tfthreadsafe.hxx"
#include "tfthreadsafequeue.hxx"
#include "tfthreadsafeunorderedmap.hxx"
#include "tfthreadcontroller.hpp"
#include "tflog.hpp"
#include "tflogrecord.hpp"
#include "tfmultivalue.hpp"
#include "tfnotification.hpp"
#include "tfnotificationcenter.hpp"
#include "tfcomparison.hpp"
#include "tfendian.hpp"
#include "tfstring.hpp"
#include "tfdatetypes.hpp"
#include "tfdate.hxx"
#include "tfdatecomponent.hxx"
#include "tfdateformatter.hxx"
#include "tfdurationcomponent.hpp"
#include "tfdurationformatter.hpp"
#include "tfdateclocks.hpp"
#include "tfdefer.hpp"
#include "tfenvironmentsettings.hpp"
#include "tffilemanager.hpp"
#include "tfalarmcenter.hpp"
#include "tffilepermissions.hpp"
#include "tffileproperties.hpp"
#include "tffilehandle.hpp"
#include "tfuuid.hpp"
#include "tfsymboltable.hxx"
#include "tfargument.hpp"
#include "tfargumentparser.hpp"
#include "tfconversion.hpp"
#if defined(TFUNIX)
#    if ! (defined(USE_POLL_STRATEGY) || defined(USE_SELECT_STRATEGY))
#        define USE_POLL_STRATEGY
#    endif
#endif
#include "tfmultiqueue.hxx"
#include "tfpoll.hpp"
#include "tfbasicprogressnotifier.hxx"
#include "tfprogressbar.hxx"
#include "tfprogressmeter.hxx"
#include "tfcrtp.hxx"
#include "tfpipe.hpp"
#include "tfcommandline.hpp"
#include "tfprocess.hpp"
#include "tfsleep.hpp"
#if defined(TFUNIX)
#    include "tfsignalhandler.hpp"
#    include "tfsignalset.hpp"
#    include "tfsignalmanager.hpp"
#endif
#include "tfipaddress.hpp"
#include "tfnetworkinterface.hpp"
#include "tfnetworkaddress.hpp"
#include "tfinternetaddress.hpp"
#include "tfsocket.hpp"
#include "tfnetworkinterfaces.hpp"
#include "tfversion.hpp"
#include "tfcompilerid.hpp"
#include "tfplatformid.hpp"
#include "tfrandomstringgenerator.hpp"
