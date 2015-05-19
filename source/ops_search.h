/*

VASP modular - vector assembling signal processor / objects for Max and Pure Data

Copyright (c)2002-2015 Thomas Grill (gr@grrrr.org)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPS_SEARCH_H
#define __VASP_OPS_SEARCH_H

#include "opbase.h"

// Sample search functions

namespace VecOp {
	BL d_search(OpParam &p); //! find values
}

namespace VaspOp {
	// search functions
	Vasp *m_search(OpParam &p,CVasp &src,const Argument &arg,CVasp *dst,BL st); //! find values
	inline Vasp *m_soffset(OpParam &p,CVasp &src,const Argument &arg,CVasp *dst = NULL) { return m_search(p,src,arg,dst,true); }
	inline Vasp *m_sframes(OpParam &p,CVasp &src,const Argument &arg,CVasp *dst = NULL) { return m_search(p,src,arg,dst,false); }
}

#endif
