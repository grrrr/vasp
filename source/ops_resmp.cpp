/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file vasp__resmp.cpp
	\brief Routines for resampling

	\todo check for src/dst overlap
	\todo support negative tilt factors
*/

#include "ops_resmp.h"


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
	\todo flag for filling remaining space
*/

static V do_tilt(OpParam &p)
{
	const I icenter = p.tilt.center;

	switch(p.tilt.mode) {
	case 1: {
		break;
	}
	case 2: {
		break;
	}
	case 0: 
	default: {
		if(p.rsdt != p.rddt) p.rddt[icenter*p.rds] = p.rsdt[icenter*p.rss];

		if(p.tilt.factor > 1) {
			I i;
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
		break;
	}
	}
}


BL VecOp::d_tilt(OpParam &p) 
{ 
	if(p.frames <= 1 || p.tilt.factor == 1) return true;

	if(p.tilt.factor <= 0) {
		post("%s - invalid factor value (%f)",p.opname,p.tilt.factor);
		return false;
	}

	I icenter = (I)p.tilt.center;
	if(p.tilt.center != icenter) {
		p.tilt.center = icenter;
		post("tilt: center position truncated to integer sample");
	}

	if(p.tilt.center < 0) {
		post("tilt: center position < 0 -> set to 0");
		p.tilt.center = 0;
	}
	else if(p.tilt.center >= p.frames) {
		post("tilt: center position >= buffer length -> set to max. position");
		p.tilt.center = p.frames;
	}

	// symmetric operation
	if(p.symm == 1) 
		p.tilt.center = p.frames-1-p.tilt.center;

	do_tilt(p); 

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
*/
Vasp *VaspOp::m_tilt(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL symm,I mode) 
{ 
	Vasp *ret = NULL;
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
	else
		post("%s - no arguments: no operation",p.opName());

	return ret;
}

VASP_ANYOP("vasp.tilt",tilt,1,true,VASP_ARG_R(1),"")
VASP_ANYOP("vasp.xtilt",xtilt,1,true,VASP_ARG_R(1),"")
