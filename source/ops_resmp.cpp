/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file vasp__resmp.cpp
	\brief Routines for resampling
*/

#include "main.h"


// --- resample ---------------------

/*! \brief Subroutine for resampling.

	\param cnt frame count
	\param src source data
	\param sstr source data stride
	\param dst destination data
	\param dstr destination data stride
	\param factor resampling factor
	\param center resampling center (this will remain untouched by the transformation)
	\param mode interpolation mode
	\return true on success

	\todo implement more interpolation methods
	\todo check for operation direction!
	\todo symmetric operation!
*/
BL VecOp::d_tilt(OpParam &p) 
{ 
	if(p.frames <= 1 || p.tilt.factor == 1) return true;

	if(p.tilt.factor == 0) {
		post("%s - invalid factor value",p.opname);
		return false;
	}

	if(p.symm >= 0) {
		post("%s - Sorry, symmetric operation not implemented yet",p.opname);
		return false;
	}

	I icenter = (I)p.tilt.center;

	if(p.tilt.center != icenter)
		post("tilt: center position truncated to integer sample");

	if(icenter < 0) {
		post("tilt: center position < 0 -> set to 0");
		icenter = 0;
	}
	else if(icenter >= p.frames) {
		post("tilt: center position >= buffer length -> set to max. position");
		icenter = p.frames;
	}


	// !lacking interpolation
	if(p.tilt.factor > 1) {
		I i;
//		I pl = (I)(icenter/factor),ql = (I)((cnt-icenter)/factor);
		for(i = 1; i <= icenter; ++i) {
			R sp = icenter-i*p.tilt.factor;
			p.rddt[(icenter-i)*p.rds] = sp >= 0 && sp < p.frames?p.rsdt[(I)sp*p.rss]:0;
		}
		for(i = 1; i < p.frames-icenter; ++i) {
			R sp = icenter+i*p.tilt.factor;
			p.rddt[(icenter+i)*p.rds] = sp >= 0 && sp < p.frames?p.rsdt[(I)sp*p.rss]:0;
		}
	}
	else {
		I i;
		for(i = icenter; i > 0; --i) {
			R sp = icenter-i*p.tilt.factor;
			p.rddt[(icenter-i)*p.rds] = p.rsdt[(I)sp*p.rss];
		}
		for(i = p.frames-1-icenter; i > 0; --i) {
			R sp = icenter+i*p.tilt.factor;
			p.rddt[(icenter+i)*p.rds] = p.rsdt[(I)sp*p.rss];
		}
	}

	return true;
}


/*! \brief Does vasp resampling.

	\param arg argument list 
	\param arg.factor factor for resampling 
	\param arg.center center of resampling
	\param dst destination vasp (NULL for in-place operation)
	\param symm true for symmetric operation
	\param mode interpolation mode
	\return normalized destination vasp
	
	\todo symmetric stuff
*/
Vasp *VaspOp::m_tilt(Vasp &src,const Argument &arg,Vasp *dst,BL symm,I mode) 
{ 
	Vasp *ret = NULL;
	OpParam p(symm?"xtilt":"tilt");
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		RVecBlock *vecs = GetRVecs(p.opname,src,dst);
		if(vecs) {
			p.tilt.factor = flx::GetAFloat(arg.GetList()[0]);
			p.tilt.center = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;
			p.tilt.mode = mode;

			ret = DoOp(vecs,VecOp::d_tilt,p,symm);

			delete vecs;
		}
	}

	return ret;
}
