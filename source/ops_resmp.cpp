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
	\todo check src/dst overlap!
*/
static BL d_tilt(I cnt,S *src,I sstr,S *dst,I dstr,R factor,R center,I mode = 0) 
{ 
	if(cnt <= 1 || factor == 1) return true;

	if(factor == 0) {
		post("tilt: invalid factor value");
		return false;
	}

	I icenter = (I)center;

	if(center != icenter)
		post("tilt: center position truncated to integer sample");

	if(icenter < 0) {
		post("tilt: center position < 0 -> set to 0");
		icenter = 0;
	}
	else if(icenter >= cnt) {
		post("tilt: center position >= buffer length -> set to max. position");
		icenter = cnt;
	}

	// !lacking interpolation
	if(factor > 1) {
		I i;
//		I pl = (I)(icenter/factor),ql = (I)((cnt-icenter)/factor);
		for(i = 1; i <= icenter; ++i) {
			R sp = icenter-i*factor;
			dst[(icenter-i)*dstr] = sp >= 0 && sp < cnt?src[(I)sp*sstr]:0;
		}
		for(i = 1; i < cnt-icenter; ++i) {
			R sp = icenter+i*factor;
			dst[(icenter+i)*dstr] = sp >= 0 && sp < cnt?src[(I)sp*sstr]:0;
		}
	}
	else {
		I i;
		for(i = icenter; i > 0; --i) {
			R sp = icenter-i*factor;
			dst[(icenter-i)*dstr] = src[(I)sp*sstr];
		}
		for(i = cnt-1-icenter; i > 0; --i) {
			R sp = icenter+i*factor;
			dst[(icenter+i)*dstr] = src[(I)sp*sstr];
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

	\todo src/dst returned vasp
*/
Vasp *Vasp::m_tilt(const Argument &arg,Vasp *dst,BL symm,I mode) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		R factor = flx::GetAFloat(arg.GetList()[0]);
		R center = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;

		RVecBlock *vecs = GetRVecs(symm?"xtilt":"tilt",*this,dst);
		if(vecs) {
			BL ok = true;
			for(I i = 0; ok && i < vecs->Vecs(); ++i) {
				VBuffer *s = vecs->Src(i),*d = vecs->Dst(i);
				if(!d) d = s; // if there's no dst, take src

				I cnt = vecs->Frames();
				I sc = s->Channels(),dc = d->Channels();
				if(symm) {
					// symmetric mode
					I hcnt = cnt/2;
					ok = 
						d_tilt(hcnt,s->Pointer(),sc,d->Pointer(),dc,factor,center) &&
						d_tilt(hcnt,s->Pointer()+(cnt-hcnt),sc,d->Pointer()+(cnt-hcnt),dc,factor,hcnt-1-center);
				}
				else
					// normal mode
					ok = d_tilt(cnt,s->Pointer(),sc,d->Pointer(),dc,factor,center);
			}
			return ok?vecs->ResVasp():NULL;
		}
		else
			return NULL;
	}
	else return NULL;
}

