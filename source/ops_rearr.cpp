/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_rearr.h"

/*! \brief shift buffer
	
	\todo flag for zero filling?
*/
BL VecOp::d_shift(OpParam &p) 
{ 
	if(p.ovrlap) {
		post("%s - cannot operate on overlapped vectors",p.opname);
		return false;
	}

	I ish = (I)p.sh.sh;
	if(p.sh.sh != ish) { // integer shift
		// requires interpolation
		post("non-integer shift not implemented - truncating to integer");
		p.sh.sh = ish;
	}

	ish = ish%p.frames;
	if(p.symm == 1) ish = -ish;

	// no zero filling!
	I cnt = p.frames-abs(ish);
	const S *sd = p.rsdt-ish*p.rss;
	S *dd = p.rddt;

	if(ish > 0) {
		sd += (p.frames-1)*p.rss,dd += (p.frames-1)*p.rds;
		p.rss = -p.rss,p.rds = -p.rds;
	}

	if(p.rss == 1 && p.rds == 1)
		for(I i = 0; i < cnt; ++i) *(dd++) = *(sd++);
	else
		for(I i = 0; i < cnt; ++i,sd += p.rss,dd += p.rds) *dd = *sd;

	return true;
}




int rotation(int ij, int n,OpParam &p) { return (ij+n-p.sh.ish)%n; }

template <class T,int DIM>
int permutation(OpParam &p,int (*origination)(int ij, int n,OpParam &p))
{
	int di,ij, oij, dij, n_to_do;
	T b[DIM],*sdt[DIM],*ddt[DIM];
	I ss[DIM],ds[DIM];

	sdt[0] = p.rsdt,ddt[0] = p.rddt; ss[0] = p.rss,ds[0] = p.rds;
	if(DIM == 2) {
		sdt[1] = p.isdt,ddt[1] = p.iddt; ss[1] = p.iss,ds[1] = p.ids;
	}

	I n = p.frames;
	n_to_do = n;
	for(ij = 0; ij < n && n_to_do > 0; ij++,n_to_do--) {
		/* Test for previously permuted */
		for (oij = origination(ij,n,p); oij > ij; oij = origination(oij,n,p)) (void)0;

		if (oij < ij) continue;

		/* Chase the cycle */
		dij = ij;
		
		for(di = 0; di < DIM; ++di) b[di] = sdt[di][ij*ss[di]];

		for(oij = origination(dij,n,p); oij != ij; oij = origination(dij,n,p),n_to_do--) {
			for(di = 0; di < DIM; ++di) ddt[di][dij*ds[di]] = sdt[di][oij*ss[di]];
			dij = oij;
		}

		for(di = 0; di < DIM; ++di) ddt[di][dij*ds[di]] = b[di];
	} 
	return 0;
}


/*! \brief rotate buffer
*/
BL VecOp::d_rot(OpParam &p) 
{ 
	if(p.ovrlap) {
		post("%s - cannot operate on overlapped vectors",p.opname);
		return false;
	}

	p.sh.ish = (I)p.sh.sh;
	if(p.sh.sh != p.sh.ish) { 
		// requires interpolation
		post("%s - non-integer shift not implemented - truncating to integer",p.opname);
	}

	p.sh.ish = p.sh.ish%p.frames;
	if(p.symm == 1) p.sh.ish = -p.sh.ish;

	permutation<S,1>(p,rotation);
	return true; 
}


/*! \brief vasp shift or rotation
	\todo units for shift
	\todo include padding modes (on command line?)
*/
Vasp *VaspOp::m_shift(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL shift,BL symm) 
{
	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0])) {
			// shift length
			p.sh.sh = flx::GetAFloat(arg.GetList()[0]);
		}
		else {
			post("%s - invalid argument -> set to 0",p.opname);
			p.sh.sh = 0;
		}

		ret = DoOp(vecs,shift?VecOp::d_shift:VecOp::d_rot,p,symm);
		delete vecs;
	}

	return ret;
}


/*! \brief mirror buffer
*/
BL VecOp::d_mirr(OpParam &p) 
{ 
	if(p.ovrlap) {
		post("%s - cannot operate on overlapped vectors",p.opname);
		return false;
	}

	if(p.rsdt == p.rddt) 
		for(S *dl = p.rddt,*du = p.rddt+(p.frames-1)*p.rds; dl < du; dl += p.rds,du -= p.rds) {
			register S t;
			t = *dl; *dl = *du; *du = t;
		}
	else {
		const S *ds = p.rsdt;
		S *dd = p.rddt+(p.frames-1)*p.rds;
		for(I i = 0; i < p.frames; ++i,ds += p.rss,dd -= p.rds) *dd = *ds;
	}
	return true; 
}

/*! \brief vasp mirror
*/
Vasp *VaspOp::m_mirr(OpParam &p,Vasp &src,Vasp *dst,BL symm) 
{
	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,VecOp::d_mirr,p,symm);
		delete vecs;
	}
	return ret;
}


VASP_ANYOP("vasp.shift",shift,0,true,VASP_ARG_I(0),"")
VASP_ANYOP("vasp.xshift",xshift,0,true,VASP_ARG_I(0),"")
VASP_ANYOP("vasp.rot",rot,0,true,VASP_ARG_I(0),"")
VASP_ANYOP("vasp.xrot",xrot,0,true,VASP_ARG_I(0),"")
VASP_UNARY("vasp.mirr",mirr,true,"")
VASP_UNARY("vasp.xmirr",xmirr,true,"")

