/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPS_FLT_H
#define __VASP_OPS_FLT_H

// Filtering functions

namespace VecOp {
	BL d_flp(OpParam &p);
	BL d_fhp(OpParam &p);
}

namespace VaspOp {
	Vasp *m_fhp(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL,BL hp = true); //! hi pass
	inline Vasp *m_flp(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_fhp(p,src,arg,dst,false); } //! lo pass	
}

#endif
