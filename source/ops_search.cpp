/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_search.h"
#include "util.h"


Vasp *VaspOp::m_search(OpParam &p,Vasp &src,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,d_search,p);
		delete vecs;
	}
	return ret;
}

class vasp_search:
	public vasp_anyop
{																				
	FLEXT_HEADER(vasp_search,vasp_anyop)
public:			
	
	vasp_search(I argc,t_atom *argv): 
		vasp_anyop(argc,argv,VASP_ARG(),true,XletCode(xlet::tp_float,0)) 
	{}

	virtual Vasp *do_peaks(OpParam &p) { return VaspOp::m_peaks(p,ref,&dst); }
		
	virtual Vasp *tx_work(const Argument &arg) 
	{ 
		OpParam p(thisName(),0);													
		
		if(arg.IsList() && arg.GetList().Count() >= 1 && CanbeFloat(arg.GetList()[0])) {
			p.peaks.density = GetAFloat(arg.GetList()[0]);
		}
		else {
			if(!arg.IsNone()) post("%s - invalid desity argument -> set to 1",p.opname);
			p.peaks.density = 1;
		}
		
		Vasp *ret = do_peaks(p);
		ToOutFloat(1,p.peaks.density);
		return ret;
	}
};																				

class vasp_soffset:
	public vasp_search
{																				
	FLEXT_HEADER(vasp_soffset,vasp_search)
public:			
	vasp_soffset(I argc,t_atom *argv): vasp_search(argc,argv) {}
	virtual Vasp *do_peaks(OpParam &p) { return VaspOp::m_valleys(p,ref,&dst); }
};																				
FLEXT_LIB_V("vasp, vasp.offset= vasp.o=",vasp_soffset)
