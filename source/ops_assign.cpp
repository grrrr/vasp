/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_assign.h"
#include "opdefs.h"


template<class T> inline V f_rcopy(T &v,T a) { v = a; }
template<class T> inline V f_ccopy(T &rv,T &iv,T ra,T ia) { rv = ra,iv = ia; }

template<class T> inline V f_rset(T &v,T,T b) { v = b; }
template<class T> inline V f_cset(T &rv,T &iv,T,T,T rb,T ib) { rv = rb,iv = ib; }


BL VecOp::d_copy(OpParam &p) { D__run(f_rcopy<S>,p); }
BL VecOp::d_ccopy(OpParam &p) { D__cun(f_ccopy<S>,p); }
BL VecOp::d_set(OpParam &p) { D__rbin(f_rset<S>,p); }
BL VecOp::d_cset(OpParam &p) { D__cbin(f_cset<S>,p); }


VASP_BINARY("vasp.=",set,false,"assigns a value or vasp")
VASP_BINARY("vasp.c=",cset,false,"assigns a complex value or vasp")
