/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include "vasp__op.h"

// --------------------------------------------------------------

BL VecOp::d_copy(OpParam &p) { D__run(f_rcopy<S>,p); }
BL VecOp::d_ccopy(OpParam &p) { D__cun(f_ccopy<S>,p); }
BL VecOp::d_set(OpParam &p) { D__rbin(f_rset<S>,p); }
BL VecOp::d_cset(OpParam &p) { D__cbin(f_cset<S>,p); }
BL VecOp::d_add(OpParam &p) { D__rbin(f_radd<S>,p); }
BL VecOp::d_cadd(OpParam &p) { D__cbin(f_cadd<S>,p); }
BL VecOp::d_sub(OpParam &p) { D__rbin(f_rsub<S>,p); }
BL VecOp::d_csub(OpParam &p) { D__cbin(f_csub<S>,p); }
BL VecOp::d_mul(OpParam &p) { D__rbin(f_rmul<S>,p); }
BL VecOp::d_cmul(OpParam &p) { D__cbin(f_cmul<S>,p); }
BL VecOp::d_div(OpParam &p) { D__rbin(f_rdiv<S>,p); }
BL VecOp::d_cdiv(OpParam &p) { return d__cbin(f_cdiv<S>,p); }
BL VecOp::d_divr(OpParam &p) { D__rbin(f_rdivr<S>,p); }
BL VecOp::d_cdivr(OpParam &p) { return d__cbin(f_cdivr<S>,p); }
BL VecOp::d_mod(OpParam &p) { D__rbin(f_rmod<S>,p); }

BL VecOp::d_min(OpParam &p) { D__rbin(f_min<S>,p); }
BL VecOp::d_rmin(OpParam &p) { return d__cbin(f_rmin<S>,p); }
BL VecOp::d_max(OpParam &p) { D__rbin(f_max<S>,p); }
BL VecOp::d_rmax(OpParam &p) { return d__cbin(f_rmax<S>,p); }

BL VecOp::d_lwr(OpParam &p) { D__rbin(f_rlwr<S>,p); }
BL VecOp::d_gtr(OpParam &p) { D__rbin(f_rgtr<S>,p); }
BL VecOp::d_leq(OpParam &p) { D__rbin(f_rleq<S>,p); }
BL VecOp::d_geq(OpParam &p) { D__rbin(f_rgeq<S>,p); }
BL VecOp::d_equ(OpParam &p) { D__rbin(f_requ<S>,p); }
BL VecOp::d_neq(OpParam &p) { D__rbin(f_rneq<S>,p); }

BL VecOp::d_pow(OpParam &p) { return d__rbin(f_rpow<S>,p); }
BL VecOp::d_cpowi(OpParam &p) { return d__cbin(f_cpowi<S>,p); }
BL VecOp::d_rpow(OpParam &p) { return d__cbin(f_crpow<S>,p); }
BL VecOp::d_radd(OpParam &p) { return d__cbin(f_radd<S>,p); }

BL VecOp::d_gate(OpParam &p) { D__rbin(f_gate<S>,p); }
BL VecOp::d_rgate(OpParam &p) { return d__cbin(f_rgate<S>,p); }
//BL VecOp::d_optq(OpParam &p) { D__rop(f_optq<S>,p); }
//BL VecOp::d_roptq(OpParam &p) { return d__cop(f_roptq<S>,p); }
BL VecOp::d_optf(OpParam &p) { return d__rop(f_optf<S>,p); }
BL VecOp::d_roptf(OpParam &p) { return d__cop(f_roptf<S>,p); }
BL VecOp::d_minq(OpParam &p) { D__rop(f_minq<S>,p); }
BL VecOp::d_maxq(OpParam &p) { D__rop(f_maxq<S>,p); }
BL VecOp::d_aminq(OpParam &p) { D__rop(f_aminq<S>,p); }
BL VecOp::d_amaxq(OpParam &p) { D__rop(f_amaxq<S>,p); }
BL VecOp::d_rminq(OpParam &p) { return d__cop(f_rminq<S>,p); }
BL VecOp::d_rmaxq(OpParam &p) { return d__cop(f_rmaxq<S>,p); }

BL VecOp::d_sqr(OpParam &p) { D__run(f_rsqr<S>,p); }
BL VecOp::d_ssqr(OpParam &p) { return d__run(f_rssqr<S>,p); }
BL VecOp::d_csqr(OpParam &p) { D__cun(f_csqr<S>,p); }
BL VecOp::d_sqrt(OpParam &p) { return d__run(f_rsqrt<S>,p); }
BL VecOp::d_ssqrt(OpParam &p) { return d__run(f_rssqrt<S>,p); }

BL VecOp::d_exp(OpParam &p) { return d__run(f_rexp<S>,p); }
BL VecOp::d_log(OpParam &p) { return d__run(f_rlog<S>,p); }

BL VecOp::d_inv(OpParam &p) { D__run(f_rinv<S>,p); }
BL VecOp::d_cinv(OpParam &p) { D__cun(f_cinv<S>,p); }

BL VecOp::d_cnorm(OpParam &p) { return d__cun(f_cnorm<S>,p); }

BL VecOp::d_abs(OpParam &p) { D__run(f_rabs<S>,p); }
BL VecOp::d_sign(OpParam &p) { D__run(f_rsign<S>,p); }

BL VecOp::d_polar(OpParam &p) { return d__cun(f_polar<S>,p); }
BL VecOp::d_cart(OpParam &p) { return d__cun(f_cart<S>,p); }
//BL VecOp::d_cswap(OpParam &p) { D__cun(f_cswap<S>,p); }
BL VecOp::d_cconj(OpParam &p) { D__cun(f_cconj<S>,p); }

BL VecOp::d_minmax(OpParam &p) { return d__cun(f_minmax<S>,p); }

