#ifndef _PRECOMPILED_H_INCLUDED_
#define _PRECOMPILED_H_INCLUDED_

#ifdef __WINDOWS__
	#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in 
#include <nmsdk.h>
#endif

#ifdef __GNUG__
	#pragma implementation
	#pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/file.h"
#include <wx/filename.h>
#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#ifdef __WINDOWS__
	#include <yvals.h>              // warning numbers get enabled in yvals.h 
	// BUG: C4786 Warning Is Not Disabled with #pragma Warning
	// STATUS: Microsoft has confirmed this to be a bug in the Microsoft product. 
	// This warning can be ignored. This occured only in the <map> container.
	#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters in 
	#pragma warning(disable: 4018)  // signed/unsigned mismatch
	#pragma warning(disable: 4100)  // unreferenced formal parameter
	#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', 
											  // signed/unsigned mismatch
	#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
	#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize 
											  // class template 'vector'
	#pragma warning(disable: 4710)  // 'function' : function not inlined
#endif
#include <map>
#include <vector>


#endif // _PRECOMPILED_H_INCLUDED_
