/* 

flext - C++ layer for Max/MSP and pd (pure data) externals

Copyright (c) 2001,2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include <flext.h>

#ifdef NT
#include <windows.h>
#endif

t_atom *flext_base::CopyList(int argc,const t_atom *argv)
{
	int i;
	t_atom *dst = new t_atom[argc];
	for(i = 0; i < argc; ++i) CopyAtom(dst+i,argv+i);
	return dst;
}

void flext_base::Sleep(int ms)
{
#ifdef NT
	::Sleep(ms);
#else
#pragma message ("flext - Sleep not implemented!")
#endif
}