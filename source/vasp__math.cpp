/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include "vasp__op.h"

#include <math.h>

#define BIG 1.e10


Vasp *VaspOp::m_cpowi(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		I powi = 1;
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeInt(arg.GetList()[0]))
			powi = flx::GetAInt(arg.GetList()[0]);
		else 
			post("%s - power arg is invalid -> set to 1",p.opname);

		if(powi < 0) {
			post("%s - negative integer power is not allowed",p.opname);
		}
		else {
			switch(powi) {
			case 0: {
				p.cbin.rarg = 1,p.cbin.iarg = 0;
				ret = DoOp(vecs,VecOp::d_cset,p);
				break;
			}
			case 1: {
				// set arg to src
				ret = DoOp(vecs,VecOp::d_ccopy,p);
				break;
			}
			case 2: {
				ret = DoOp(vecs,VecOp::d_csqr,p);
				break;
			}
			default: {
				p.ibin.arg = powi;
				ret = DoOp(vecs,VecOp::d_cpowi,p);
				break;
			}
			}
		}

		delete vecs;
	}
	return ret;
}



Vasp *VaspOp::m_rpow(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.cbin.rarg = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - argument is invalid -> set to 1",p.opname);
			p.cbin.rarg = 1;
		}
		p.cbin.iarg = 0; // not used anyway

		ret = DoOp(vecs,VecOp::d_rpow,p);
		delete vecs;
	}
	return ret;
}


Vasp *VaspOp::m_radd(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.cbin.rarg = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - argument is invalid -> set to 0",p.opname);
			p.cbin.rarg = 0;
		}
		p.cbin.iarg = 0; // not used anyway

		ret = DoOp(vecs,VecOp::d_radd,p);
		delete vecs;
	}
	return ret;
}


Vasp *VaspOp::m_gate(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.rbin.arg = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - argument is invalid -> set to 1",p.opname);
			p.rbin.arg = 1;
		}

		ret = DoOp(vecs,VecOp::d_gate,p);
		delete vecs;
	}
	return ret;
}


Vasp *VaspOp::m_rgate(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.cbin.rarg = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - argument is invalid -> set to 1",p.opname);
			p.cbin.rarg = 1;
		}
		p.cbin.iarg = 0; // not used anyway

		ret = DoOp(vecs,VecOp::d_rgate,p);
		delete vecs;
	}
	return ret;
}




/*! \class vasp_qmin
	\remark \b vasp.min?
	\brief Get minimum sample value
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qmin:
	public vasp_unop
{
	FLEXT_HEADER(vasp_qmin,vasp_unop)

public:
	vasp_qmin(): vasp_unop(true,XletCode(xlet::tp_float,0)) {}

	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = BIG;
		Vasp *ret = VaspOp::m_qmin(p,ref); 
		if(p.norm.minmax == BIG) p.norm.minmax = 0;
		return ret;
	}
		
	virtual Vasp *tx_work() 
	{ 
		OpParam p(thisName(),0);													
		Vasp *ret = do_opt(p);
		ToOutFloat(1,p.norm.minmax);
		return ret;
	}

	virtual V m_help() { post("%s - Get a vasp's minimum sample value",thisName()); }
};

FLEXT_LIB("vasp.min?",vasp_qmin)


/*! \class vasp_qamin
	\remark \b vasp.amin?
	\brief Get minimum absolute sample value
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qamin:
	public vasp_qmin
{
	FLEXT_HEADER(vasp_qamin,vasp_qmin)
public:
	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = BIG;
		Vasp *ret = VaspOp::m_qamin(p,ref); 
		if(p.norm.minmax == BIG) p.norm.minmax = 0;
		return ret;
	}

	virtual V m_help() { post("%s - Get a vasp's minimum absolute sample value",thisName()); }
};

FLEXT_LIB("vasp.amin?",vasp_qamin)



/*! \class vasp_qmax
	\remark \b vasp.max?
	\brief Get maximum sample value
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qmax:
	public vasp_qmin
{
	FLEXT_HEADER(vasp_qmax,vasp_qmin)
public:
	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = -BIG;
		Vasp *ret = VaspOp::m_qmax(p,ref); 
		if(p.norm.minmax == -BIG) p.norm.minmax = 0;
		return ret;
	}

	virtual V m_help() { post("%s - Get a vasp's maximum sample value",thisName()); }
};

FLEXT_LIB("vasp.max?",vasp_qmax)



/*! \class vasp_qamax
	\remark \b vasp.amax?
	\brief Get minimum absolute sample value
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qamax:
	public vasp_qmax
{
	FLEXT_HEADER(vasp_qamax,vasp_qmax)
public:
	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = 0;
		return VaspOp::m_qamax(p,ref); 
	}

	virtual V m_help() { post("%s - Get a vasp's maximum absolute sample value",thisName()); }
};

FLEXT_LIB("vasp.amax?",vasp_qamax)




/*! \class vasp_qrmin
	\remark \b vasp.rmin?
	\brief Get minimum complex radius of samples
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qrmin:
	public vasp_unop
{
	FLEXT_HEADER(vasp_qrmin,vasp_unop)

public:
	vasp_qrmin(): vasp_unop(true,XletCode(xlet::tp_float,0)) {}

	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = BIG;
		Vasp *ret = VaspOp::m_qrmin(p,ref); 
		if(p.norm.minmax == BIG) p.norm.minmax = 0;
		return ret;
	}
		
	virtual Vasp *tx_work() 
	{ 
		OpParam p(thisName(),0);													
		Vasp *ret = do_opt(p);
		ToOutFloat(1,p.norm.minmax);
		return ret;
	}

	virtual V m_help() { post("%s - Get a vasp's minimum complex radius",thisName()); }
};

FLEXT_LIB("vasp.rmin?",vasp_qrmin)



/*! \class vasp_qrmax
	\remark \b vasp.rmax?
	\brief Get maximum complex radius of samples
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qrmax:
	public vasp_qrmin
{
	FLEXT_HEADER(vasp_qrmax,vasp_qrmin)
public:
	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = 0;
		return VaspOp::m_qrmax(p,ref); 
	}

	virtual V m_help() { post("%s - Get a vasp's maximum complex radius",thisName()); }
};

FLEXT_LIB("vasp.rmax?",vasp_qrmax)



