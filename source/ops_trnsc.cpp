/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_trnsc.h"
#include "opdefs.h"

// --------------------------------------------------------------

BL VecOp::d_pow(OpParam &p) { return d__rbin(f_rpow<S>,p); }
BL VecOp::d_cpowi(OpParam &p) { return d__cop(f_cpowi<S>,p); }
BL VecOp::d_rpow(OpParam &p) { return d__cbin(f_crpow<S>,p); }

BL VecOp::d_sqr(OpParam &p) { D__run(f_rsqr<S>,p); }
BL VecOp::d_ssqr(OpParam &p) { return d__run(f_rssqr<S>,p); }
BL VecOp::d_csqr(OpParam &p) { D__cun(f_csqr<S>,p); }
BL VecOp::d_sqrt(OpParam &p) { return d__run(f_rsqrt<S>,p); }
BL VecOp::d_ssqrt(OpParam &p) { return d__run(f_rssqrt<S>,p); }

BL VecOp::d_exp(OpParam &p) { return d__run(f_rexp<S>,p); }
BL VecOp::d_log(OpParam &p) { return d__run(f_rlog<S>,p); }

