/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPS_WND_H
#define __VASP_OPS_WND_H

#include "opbase.h"

// Window functions

namespace VecOp {
	BL d_bevel(OpParam &p); 
	BL d_mbevel(OpParam &p); 

	BL d_window(OpParam &p);
	BL d_mwindow(OpParam &p);
	BL d_vwindow(OpParam &p);
	BL d_vmwindow(OpParam &p);
}

namespace VaspOp {
	Vasp *m_bevelup(OpParam &p,Vasp &src,Vasp *dst,BL up = true,BL mul = false);  // bevel up 
	inline Vasp *m_mbevelup(OpParam &p,Vasp &src,Vasp *dst) { return m_bevelup(p,src,dst,true,true); }   // * bevel up (fade in)
	inline Vasp *m_beveldn(OpParam &p,Vasp &src,Vasp *dst) { return m_bevelup(p,src,dst,false,false); }  // bevel down
	inline Vasp *m_mbeveldn(OpParam &p,Vasp &src,Vasp *dst) { return m_bevelup(p,src,dst,false,true); }   // * bevel down (fade out)
	Vasp *m_window(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL mul = false);  // window curve
	inline Vasp *m_mwindow(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) { return m_window(p,src,arg,dst,true); }  // * window curve
}

#endif
