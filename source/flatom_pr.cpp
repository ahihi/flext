/* 

flext - C++ layer for Max/MSP and pd (pure data) externals

Copyright (c) 2001-2005 Thomas Grill (gr@grrrr.org)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file flatom_pr.cpp
    \brief Definitions for printing and scanning the t_atom type.
*/
 
#include "flext.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

// \TODO take bufsz into account!
bool flext::PrintAtom(const t_atom &a,char *buf,size_t bufsz)
{
    bool ok = true;
    if(IsFloat(a)) {
        ok = STD::snprintf(buf,bufsz,"%g",GetFloat(a)) > 0;
    }
    else if(IsInt(a)) {
        ok = STD::snprintf(buf,bufsz,"%i",GetInt(a)) > 0;
    }
    else if(IsSymbol(a)) {
        if(!FLEXT_ASSERT(GetSymbol(a))) 
            *buf = 0;
        else {
            const char *c = GetString(a);
            size_t len = strlen(c);
            if(len < bufsz) {
                memcpy(buf,c,len); buf[len] = 0;
                ok = true;
            }
            else 
                ok = false;
        }
    }
    else if(IsPointer(a)) {
        ok = STD::snprintf(buf,bufsz,"%p",GetPointer(a)) > 0;
    }
#if FLEXT_SYS == FLEXT_SYS_PD
    else if(a.a_type == A_DOLLAR) {
        ok = STD::snprintf(buf,bufsz,"$%d",a.a_w.w_index) > 0;
    }
    else if(a.a_type == A_DOLLSYM) {
        ok = STD::snprintf(buf,bufsz,"$%s",GetString(a)) > 0;
    }
#elif FLEXT_SYS == FLEXT_SYS_MAX
    else if(a.a_type == A_DOLLAR) {
        ok = STD::snprintf(buf,bufsz,"$%d",a.a_w.w_long) > 0;
    }
#else
//#pragma message("Not implemented")
#endif
    else {
        error("flext: atom type unknown");
        ok = false;
    }
    return ok;
}

bool flext::PrintList(int argc,const t_atom *argv,char *buf,size_t bufsz)
{
    bool ok = true;
    for(int i = 0; ok && i < argc && bufsz > 0; ++i) {
        if(i) { *(buf++) = ' '; --bufsz; } // prepend space

        if(PrintAtom(argv[i],buf,bufsz)) {
            size_t len = strlen(buf);
            buf += len,bufsz -= len;
        }
        else
            ok = false;
    }
    *buf = 0;
    return ok;
}


bool flext::ScanAtom(t_atom &a,const char *buf)
{
    // skip whitespace
    while(*buf && isspace(*buf)) ++buf;
    if(!*buf) return false;

    char tmp[1024];
    strcpy(tmp,buf);
    char *c = tmp;

    // check for word type (s = 0,1,2 ... int,float,symbol)
    int s = 0;
    for(; *c && !isspace(*c); ++c) {
        if(!isdigit(*c)) 
            s = (*c != '.' || s == 1)?2:1;
    }

    switch(s) {
    case 0: // integer
#if FLEXT_SYS == FLEXT_SYS_MAX
        SetInt(a,atol(tmp));
        break;
#endif
    case 1: // float
        SetFloat(a,(float)atof(tmp));
        break;
    default: { // anything else is a symbol
        char t = *c; *c = 0;
        SetString(a,tmp);
        *c = t;
        break;
    }
    }

    return true;
}


