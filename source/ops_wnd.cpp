/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_wnd.h"
#include "oploop.h"
#include <math.h>
#include <string.h>

// --- window --------------------------

BL VecOp::d_window(OpParam &p) 
{ 
	switch(p.wnd.wndtp) {	
	case 0: { // bevel
		BL rev = ((p.revdir?1:0)^(p.symm == 1?1:0)^(p.wnd.inv?1:0)) != 0;
		register R inc,cur;
		
		if(p.oddrem) p.SkipOddMiddle(1);

		inc = (rev?-1.:1.)/p.frames; // increase
		cur = rev?(1+inc/2):inc/2; // start

		if(!p.wnd.mul) {
			register S *dd = p.rddt;
			register I i;
			if(p.rds == 1)
				_D_LOOP(i,p.frames) *(dd++) = cur,cur += inc;
			else
				_D_LOOP(i,p.frames) *dd = cur,dd += p.rds,cur += inc;
		}
		else {
			register const S *sd = p.rsdt;
			register S *dd = p.rddt;
			register I i;
			if(sd == dd)
				if(p.rss == 1 && p.rds == 1)
					_D_LOOP(i,p.frames) *(dd++) *= cur,cur += inc;
				else
					_D_LOOP(i,p.frames) *dd *= cur,dd += p.rds,cur += inc;
			else
				if(p.rss == 1 && p.rds == 1)
					_D_LOOP(i,p.frames) *(dd++) = *(sd++) * cur,cur += inc;
				else
					_D_LOOP(i,p.frames) *dd = *sd * cur,sd += p.rss,dd += p.rds,cur += inc;
		}
		break;
	}
	case 1: { // sine
	}
	case 2: { // cosine
	}	
	default: {
		post("%s: Window function #%i not known",p.opname,p.wnd.wndtp);
		return false;
	}
	}
	
	return true;
}

Vasp *VaspOp::m_window(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL inv,BL mul,BL symm) 
{ 
	static const int wndnum = 3;
	static const char *wndtps[wndnum] = {"lin","sin","cos"};

	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		p.wnd.wndtp = -1;

		if(arg.IsList() && arg.GetList().Count() >= 1) {
			// window mode
			const flext_base::AtomList &l = arg.GetList();
			if(flx::IsSymbol(l[0])) {
				I i;
				const C *s = flx::GetString(l[0]);
				p.wnd.wndtp = -1;
				for(i = 0; i < wndnum; ++i)
					if(!strcmp(wndtps[i],s)) { p.wnd.wndtp = i; break; }
			}
			else if(flx::CanbeInt(l[0])) {
				p.wnd.wndtp = flx::GetAInt(l[0]);
			}
			else p.wnd.wndtp = -1;
		}
		
		if(p.wnd.wndtp < 0) {
			post("%s - invalid window type - using lin",p.opname);
			p.wnd.wndtp = 0;
		}
		
		p.wnd.inv = inv;
		p.wnd.mul = mul;			
		ret = DoOp(vecs,VecOp::d_window,p,symm);
		delete vecs;
	}

	return ret;
}

VASP_ANYOP("vasp.window vasp.wnd",window,0,false,VASP_ARG(),"Sets target vasp to window function")
VASP_ANYOP("vasp.*window vasp.*wnd",mwindow,0,true,VASP_ARG(),"Multiplies a vasp by window function")
VASP_ANYOP("vasp.!window vasp.!wnd",iwindow,0,false,VASP_ARG(),"Sets target vasp to reverse window function")
VASP_ANYOP("vasp.*!window vasp.!wnd",miwindow,0,true,VASP_ARG(),"Multiplies a vasp by reverse window function")
VASP_ANYOP("vasp.xwindow vasp.xwnd",xwindow,0,false,VASP_ARG(),"Sets target vasp to symmetrical window function")
VASP_ANYOP("vasp.*xwindow vasp.*xwnd",mxwindow,0,true,VASP_ARG(),"Multiplies a vasp by symmetrical window function")



