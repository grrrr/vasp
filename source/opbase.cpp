/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

Vasp *VaspOp::m_run(Vasp &src,Vasp *dst,VecOp::opfun *fun,const C *opnm) 
{ 
	Vasp *ret = NULL;
	OpParam p(opnm);

	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,fun,p);
		delete vecs;
	}

	return ret;
}

Vasp *VaspOp::m_cun(Vasp &src,Vasp *dst,VecOp::opfun *fun,const C *opnm) 
{ 
	Vasp *ret = NULL;
	OpParam p(opnm);

	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,fun,p);
		delete vecs;
	}

	return ret;
}

Vasp *VaspOp::m_rbin(Vasp &src,const Argument &arg,Vasp *dst,VecOp::opfun *fun,const C *opnm) 
{ 
	Vasp *ret = NULL;
	OpParam p(opnm);
	BL argvasp = arg.IsVasp();

	RVecBlock *vecs = argvasp?GetRVecs(p.opname,src,arg.GetVasp(),dst):GetRVecs(p.opname,src,dst);
	if(vecs) {
		if(!argvasp) p.rbin.arg = arg.GetADouble(); // if no arg vasp
	
		ret = DoOp(vecs,fun,p);
		delete vecs;
	}

	return ret;
}

Vasp *VaspOp::m_cbin(Vasp &src,const Argument &arg,Vasp *dst,VecOp::opfun *fun,const C *opnm) 
{ 
	Vasp *ret = NULL;
	OpParam p(opnm);
	BL argvasp = arg.IsVasp();

	CVecBlock *vecs = argvasp?GetCVecs(p.opname,src,arg.GetVasp(),dst):GetCVecs(opnm,src,dst);
	if(vecs) {
		if(!argvasp) {
			CX z = arg.GetAComplex();
			p.cbin.rarg = z.real; 
			p.cbin.iarg = z.imag; 
		}

		ret = DoOp(vecs,fun,p);
		delete vecs;
	}

	return ret;
}

