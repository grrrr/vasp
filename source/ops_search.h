/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPS_SEARCH_H
#define __VASP_OPS_SEARCH_H

#include "opbase.h"

// Sample search functions

namespace VecOp {
}

namespace VaspOp {
	// search functions
	Vasp *m_soffset(OpParam &p,Vasp &src,Vasp *dst = NULL); //! find values
	Vasp *m_sframes(OpParam &p,Vasp &src,Vasp *dst = NULL); //! find values
}

#endif
