/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPS_TRNSC_H
#define __VASP_OPS_TRNSC_H

#include "opbase.h"

// Transcendent math functions

namespace VecOp {
	BL d_pow(OpParam &p); 

	BL d_rpow(OpParam &p); 

	BL d_sqrt(OpParam &p); 
	BL d_ssqrt(OpParam &p); 


	BL d_exp(OpParam &p); 
	BL d_log(OpParam &p); 
}

namespace VaspOp {
	inline Vasp *m_pow(OpParam &p,CVasp &src,const Argument &arg,CVasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_pow); } // power

	Vasp *m_rpow(OpParam &p,CVasp &src,const Argument &arg,CVasp *dst = NULL); // radius power (with each two channels)

	inline Vasp *m_sqrt(OpParam &p,CVasp &src,CVasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_sqrt); }  // square root (from abs value)
	inline Vasp *m_ssqrt(OpParam &p,CVasp &src,CVasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_ssqrt); }  // square root (from abs value)

	inline Vasp *m_exp(OpParam &p,CVasp &src,CVasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_exp); }  // exponential function
	inline Vasp *m_log(OpParam &p,CVasp &src,CVasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_log); } // natural logarithm

//	inline Vasp *m_cexp(OpParam &p,CVasp &src,CVasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_cexp); }  // complex exponential function
//	inline Vasp *m_clog(OpParam &p,CVasp &src,CVasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_clog); } // complex logarithm (how about branches?)

}

#endif
