/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPDEFS_H
#define __VASP_OPDEFS_H

#include "oploop.h"
#include "opbase.h"

#ifdef VASP_CHN1  
#define _D_ALWAYS1 1
#else
#define _D_ALWAYS1 0
#endif


namespace VecOp {

/*! \brief skeleton for unary real operations
*/
template<class T,V FUN(T &v,T a)> BL V__run(register const T *sr,I rss,register T *dr,I rds,I frames)
{																
    register I i;
    if(sr == dr)												
		if(_D_ALWAYS1 || rds == 1)							
			_DE_LOOP(i,frames, ( FUN(*dr,*dr), dr++ ) )
		else													
			_DE_LOOP(i,frames, ( FUN(*dr,*dr), dr += rds ) )
	else														
		if(_D_ALWAYS1 || (rss == 1 && rds == 1))			
			_DE_LOOP(i,frames, ( FUN(*dr,*sr), sr++,dr++ ) )
		else													
			_DE_LOOP(i,frames, ( FUN(*dr,*sr), sr += rss,dr += rds ) )
	return true;												
}


/*! \brief skeleton for unary complex operations
*/
template<class T,V FUN(T &rv,T &iv,T ra,T ia)> BL V__cun(register const T *sr,register const T *si,I rss,I iss,register T *dr,register T *di,I rds,I ids,I frames)
{																
    register I i;
	if(sr == dr && si == di)									
		if(_D_ALWAYS1 || (rds == 1 && ids == 1))			
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*dr,*di), dr++,di++ ) )
		else													
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*dr,*di), dr += rds,di += ids ) )
	else														
		if(_D_ALWAYS1 || (rss == 1 && iss == 1 && rds == 1 && ids == 1)) 
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*sr,*si), sr++,si++,dr++,di++ ) )
		else													
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*sr,*si), sr += rss,si += iss,dr += rds,di += ids ) )
	return true;												
}

template<class T,V FUN(T &v,T a)> BL V__vun(I layers,register const T *sr,register T *dr,I frames)
{																
    register I i;
    switch(layers) {
    case 1: 
            V__run<T,FUN>(sr,1,dr,1,frames); 
            break;
    case 2: 
			_DF_LOOP(i,frames, ( vec_un<T,FUN,2>(dr,sr), sr += 2, dr += 2) )
            break;
    case 3: 
			_DF_LOOP(i,frames, ( vec_un<T,FUN,3>(dr,sr), sr += 3, dr += 3) )
            break;
    case 4: 
			_DF_LOOP(i,frames, ( vec_un<T,FUN,4>(dr,sr), sr += 4, dr += 4) )
            break;
    default:
			_DF_LOOP(i,frames, ( vec_un<T,FUN,0>(dr,sr,layers), sr += layers, dr += layers) )
            break;
    }
    return true;
}


template<class T,V FUN(T &v,T a,T b)> BL V__rbin(register const T *sr,I rss,register T *dr,I rds,register const T *ar,I ras,I frames)
{																
	register I i;												
	if(sr == dr)								
		if(_D_ALWAYS1 || (rds == 1 && ras == 1))				
			_DE_LOOP(i,frames, ( FUN(*dr,*dr,*ar), dr++,ar++ ) )
		else												
			_DE_LOOP(i,frames, ( FUN(*dr,*dr,*ar), dr += rds,ar += ras ) )
	else													
		if(_D_ALWAYS1 || (rss == 1 && rds == 1 && ras == 1))	
			_DE_LOOP(i,frames, ( FUN(*dr,*sr,*ar), sr++,dr++,ar++ ) )
		else												
			_DE_LOOP(i,frames, ( FUN(*dr,*sr,*ar), sr += rss,dr += rds,ar += ras ) )
    return true;
}

template<class T,V FUN(T &v,T a,T b)> BL V__cbin(register const T *sr,register const T *si,I rss,I iss,register T *dr,register T *di,I ids,I rds,const T *ar,const T *ai,I ras,I ias,I frames)
{																
	register I i;												
	if(sr == dr && si == di)							
		if(_D_ALWAYS1 || (rds == 1 && ids == 1 && ras == 1 && ias == 1)) 
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*dr,*di,*ar,*ai), dr++,di++,ar++,ai++ ) )
		else											
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*dr,*di,*ar,*ai), dr += rds,di += ids,ar += ras,ai += ias ) )
	else												
		_DE_LOOP(i,frames, ( FUN(*dr,*di,*sr,*si,*ar,*ai), sr += rss,si += iss,dr += rds,di += ids,ar += ras,ai += ias ) )
    return true;
}


template<class TR> inline TR _A__vector(const TR *a,I i,I m) { return a[i*m]; }
template<class TR> inline TR _A__scalar(TR a,I i,I m) { return a; }
inline R _A__env(Env::Iter &a,I i,I m) { return a.ValFwd(i); }

template<class T,class TA,class TR,V FUN(T &v,T a,TR b),TR EVARG(TA a,I i,I m)> BL Vx__rbin(register const T *sr,I rss,register T *dr,I rds,TA ar,I frames)
{																
	register I i;												
	if(sr == dr)								
		if(_D_ALWAYS1 || rds == 1)				
			_DE_LOOP(i,frames, ( FUN(*dr,*dr,EVARG(ar,i,1)), dr++ ) )
		else												
			_DE_LOOP(i,frames, ( FUN(*dr,*dr,EVARG(ar,i,1)), dr += rds ) )
	else													
		if(_D_ALWAYS1 || rss == 1 && rds == 1)	
			_DE_LOOP(i,frames, ( FUN(*dr,*sr,EVARG(ar,i,1)), sr++,dr++ ) )
		else												
			_DE_LOOP(i,frames, ( FUN(*dr,*sr,EVARG(ar,i,1)), sr += rss,dr += rds ) )
    return true;
}

template<class T,class TA1,class TA2,class TR,V FUN(T &v,T a,TR b),TR EVARG1(TA1 a,I i,I m),TR EVARG2(TA2 a,I i,I m)> BL Vx__cbin(register const T *sr,register const T *si,I rss,I iss,register T *dr,register T *di,I ids,I rds,TA1 ar,TA2 ai,I ras,I ias,I frames)
{																
	register I i;												
	if(sr == dr && si == di)							
		if(_D_ALWAYS1 || (rds == 1 && ids == 1 && ras == 1 && ias == 1)) 
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*dr,*di,EVARG1(ar,i,1),EVARG2(ai,i,1)), dr++,di++ ) )
		else											
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*dr,*di,EVARG1(ar,i,ras),EVARG2(ai,i,ias)), dr += rds,di += ids ) )
	else												
		_DE_LOOP(i,frames, ( FUN(*dr,*di,*sr,*si,EVARG1(ar,i,ras),EVARG2(ai,i,ias)), sr += rss,si += iss,dr += rds,di += ids ) )
    return true;
}

template<class T,class TA,class TR,V FUN(T &v,T a,TR b),TR EVARG(TA a,I i,I m)> BL Vx__vbin(I layers,register const T *sr,register T *dr,TA ar,I frames)
{																
    register I i;
    switch(layers) {
    case 1: 
            Vx__rbin<T,TA,TR,FUN,EVARG>(sr,1,dr,1,ar,frames); 
            break;
    case 2: 
			_DF_LOOP(i,frames, ( vec_bin<T,FUN,2>(dr,sr,EVARG(ar,i,2)), sr += 2, dr += 2) )
            break;
    case 3: 
			_DF_LOOP(i,frames, ( vec_bin<T,FUN,3>(dr,sr,EVARG(ar,i,3)), sr += 3, dr += 3) )
            break;
    case 4: 
			_DF_LOOP(i,frames, ( vec_bin<T,FUN,4>(dr,sr,EVARG(ar,i,4)), sr += 4, dr += 4) )
            break;
    default:
			_DF_LOOP(i,frames, ( vec_bin<T,FUN,0>(dr,sr,EVARG(ar,i,layers),layers), sr += layers, dr += layers) )
            break;
    }
    return true;
}

template<class T,V FUN(T &v,T a,T b)> inline BL V__vbin(I layers,register const T *sr,register T *dr,register const T *ar,I frames)
{
    return Vx__vbin<T,const T *,T,FUN,_A__vector<T> >(layers,sr,dr,ar,frames);
}

/*! \brief skeleton for binary real operations
*/
template<class T,V FUN(T &v,T a,T b)> BL _F__rbin(OpParam &p)					
{																
	if(p.HasArg() && p.arg[0].Is()) {							
		switch(p.arg[0].argtp) {								
		case OpParam::Arg::arg_v: {	
            V__rbin<T,FUN>(p.rsdt,p.rss,p.rddt,p.rds,p.arg[0].v.rdt,p.arg[0].v.rs,p.frames);
			break;												
		}														
		case OpParam::Arg::arg_env: {							
			Env::Iter it(*p.arg[0].e.env); it.Init(0);
            Vx__rbin<T,Env::Iter &,R,FUN,_A__env >(p.rsdt,p.rss,p.rddt,p.rds,it,p.frames);
			break;												
		}														
		case OpParam::Arg::arg_x: {							
            Vx__rbin<T,R,T,FUN,_A__scalar<R> >(p.rsdt,p.rss,p.rddt,p.rds,p.arg[0].x.r,p.frames);
			break;												
		}														
		}														
	}															
	else {														
        Vx__rbin<T,T,T,FUN,_A__scalar<T> >(p.rsdt,p.rss,p.rddt,p.rds,p.rbin.arg,p.frames);
	}															
	return true;												
}


/*! \brief skeleton for binary complex operations
*/
template<class T,V FUN(T &rv,T &iv,T ra,T ia,T rb,T ib)> BL _F__cbin(OpParam &p)							
{																
	if(p.HasArg() && p.arg[0].Is()) {							
		switch(p.arg[0].argtp) {									
		case OpParam::Arg::arg_v: {									
			if(p.arg[0].v.idt)
                V__cbin<T,FUN>(p.rsdt,p.isdt,p.rss,p.iss,p.rddt,p.iddt,p.rds,p.ids,p.arg[0].v.rdt,p.arg[0].v.idt,p.arg[0].v.rs,p.arg[0].v.is,p.frames);
            else
                Vx__cbin<T,const T *,T,T,FUN,_A__vector<T>,_A__scalar<T> >(p.rsdt,p.isdt,p.rss,p.iss,p.rddt,p.iddt,p.rds,p.ids,p.arg[0].v.rdt,0,p.arg[0].v.rs,1,p.frames);
			break;												
		}														
		case OpParam::Arg::arg_env: {							
			Env::Iter it(*p.arg[0].e.env); it.Init(0);			
            Vx__cbin<T,Env::Iter &,T,R,FUN,_A__env,_A__scalar<T> >(p.rsdt,p.isdt,p.rss,p.iss,p.rddt,p.iddt,p.rds,p.ids,it,0,1,1,p.frames);
			break;												
		}														
		case OpParam::Arg::arg_x: {								
            Vx__cbin<T,R,R,R,FUN,_A__scalar<R>,_A__scalar<R> >(p.rsdt,p.isdt,p.rss,p.iss,p.rddt,p.iddt,p.rds,p.ids,p.arg[0].x.r,p.arg[0].x.i,1,1,p.frames);
			break;												
		}														
		}														
	}															
	else {														
        Vx__cbin<T,T,T,T,FUN,_A__scalar<T>,_A__scalar<T> >(p.rsdt,p.isdt,p.rss,p.iss,p.rddt,p.iddt,p.rds,p.ids,p.cbin.rarg,p.cbin.iarg,1,1,p.frames);
	}															
	return true;												
}

/*! \brief skeleton for real operations with parameter block
*/
template<class T,class OP,V FUN(T &v,T r,OP p)> BL V__rop(OP p,register const S *sr,I rss,register S *dr,I rds,I frames)
{																
	register I i;												
	if(sr == dr)												
		if(_D_ALWAYS1 || rds == 1)											
			_DE_LOOP(i,frames, ( FUN(*dr,*dr,p), dr++ ) )
		else													
			_DE_LOOP(i,frames, ( FUN(*dr,*dr,p), dr += rds ) )
	else														
		if(_D_ALWAYS1 || (rss == 1 && p.rds == 1))			
			_DE_LOOP(i,frames, ( FUN(*dr,*sr,p), sr++,dr++ ) )
		else													
			_DE_LOOP(i,frames, ( FUN(*dr,*sr,p), sr += rss,dr += rds ) )
	return true;												
}

/*! \brief skeleton for complex operations with parameter block
*/
template<class T,class OP,V FUN(T &rv,T &iv,T ra,T ia,OP p)> BL V__cop(OP p,register const S *sr,register const S *si,I rss,I iss,register S *dr,register S *di,I rds,I ids,I frames)
{																
	register I i;												
	if(sr == dr && si == di)									
		if(_D_ALWAYS1 || (rds == 1 && ids == 1))			
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*dr,*di,p), dr++,di++ ) )
		else													
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*dr,*di,p), dr += rds,di += ids ) )
	else														
		if(_D_ALWAYS1 || (p.rss == 1 && p.iss == 1 && p.rds == 1 && p.ids == 1)) 
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*sr,*si,p), sr++,si++,dr++,di++ ) )
		else													
			_DE_LOOP(i,frames, ( FUN(*dr,*di,*sr,*si,p), sr += rss,si += iss,dr += rds,di += ids ) )
	return true;												
}

template<class T,class CL> inline BL _D__run(OpParam &p) { return V__run<T,CL::run>(p.rsdt,p.rss,p.rddt,p.rds,p.frames); }
template<class T,class CL> inline BL _D__cun(OpParam &p) { return V__cun<T,CL::cun>(p.rsdt,p.isdt,p.rss,p.iss,p.rddt,p.iddt,p.rds,p.ids,p.frames); }
template<class T,class CL> inline BL _D__rbin(OpParam &p) { return _F__rbin<T,CL::rbin>(p); }
template<class T,class CL> inline BL _D__cbin(OpParam &p) { return _F__cbin<T,CL::cbin>(p); }			
template<class T,class CL> inline BL _D__rop(OpParam &p) { return V__rop<T,OpParam &,CL::rop>(p,p.rsdt,p.rss,p.rddt,p.rds,p.frames); }
template<class T,class CL> inline BL _D__cop(OpParam &p) { return V__cop<T,OpParam &,CL::cop>(p,p.rsdt,p.isdt,p.rss,p.iss,p.rddt,p.iddt,p.rds,p.ids,p.frames); }


template<class T,class CL> inline BL d__run(OpParam &p) { return _d__run<T>(CL::run,p); }
template<class T,class CL> inline BL d__cun(OpParam &p) { return _d__cun<T>(CL::cun,p); }
template<class T,class CL> inline BL d__rbin(OpParam &p) { return _d__rbin<T>(CL::rbin,p); }
template<class T,class CL> inline BL d__cbin(OpParam &p) { return _d__cbin<T>(CL::cbin,p); }
template<class T,class CL> inline BL d__rop(OpParam &p) { return _d__rop<T>(CL::rop,p); }
template<class T,class CL> inline BL d__cop(OpParam &p) { return _d__cop<T>(CL::cop,p); }

template<class T,V FUN(T &v,T a)> inline BL f__run(OpParam &p) { return _d__run(FUN,p); }
template<class T,V FUN(T &rv,T &iv,T ra,T ia)> inline BL f__cun(OpParam &p) { return _d__cun(FUN,p); }
template<class T,V FUN(T &v,T a,T b)> inline BL f__rbin(OpParam &p) { return _d__rbin(FUN,p); }
template<class T,V FUN(T &rv,T &iv,T ra,T ia,T rb,T ib)> inline BL f__cbin(OpParam &p) { return _d__cbin(FUN,p); }
template<class T,V FUN(T &v,T r,OpParam &p)> inline BL f__rop(OpParam &p) { return _d__rop(FUN,p); }
template<class T,V FUN(T &rv,T &iv,T ra,T ia,OpParam &p)> inline BL f__cop(OpParam &p) { return _d__cop(FUN,p); }


#ifdef VASP_COMPACT
template<class T,class CL> inline BL D__run(OpParam &p) { return _d__run<T>(CL::run,p); }
template<class T,class CL> inline BL D__cun(OpParam &p) { return _d__cun<T>(CL::cun,p); }
template<class T,class CL> inline BL D__rbin(OpParam &p) { return _d__rbin<T>(CL::rbin,p); }
template<class T,class CL> inline BL D__cbin(OpParam &p) { return _d__cbin<T>(CL::cbin,p); }
template<class T,class CL> inline BL D__rop(OpParam &p) { return _d__rop<T>(CL::rop,p); }
template<class T,class CL> inline BL D__cop(OpParam &p) { return _d__cop<T>(CL::cop,p); }
template<class T,V FUN(T &v,T a)> inline BL F__run(OpParam &p) { return _d__run(FUN,p); }
template<class T,V FUN(T &rv,T &iv,T ra,T ia)> inline BL F__cun(OpParam &p) { return _d__cun<T>(FUN,p); }
template<class T,V FUN(T &v,T a,T b)> inline BL F__rbin(OpParam &p) { return _d__rbin<T>(FUN,p); }
template<class T,V FUN(T &rv,T &iv,T ra,T ia,T rb,T ib)> inline BL F__cbin(OpParam &p) { return _d__cbin<T>(FUN,p); }
template<class T,V FUN(T &v,T r,OpParam &p)> inline BL F__rop(OpParam &p) { return _d__rop<T>(FUN,p); }
template<class T,V FUN(T &rv,T &iv,T ra,T ia,OpParam &p)> inline BL F__cop(OpParam &p) { return _d__cop<T>(FUN,p); }
#else
template<class T,class CL> inline BL D__run(OpParam &p) { return _D__run<T,CL>(p); }
template<class T,class CL> inline BL D__cun(OpParam &p) { return _D__cun<T,CL>(p); }
template<class T,class CL> inline BL D__rbin(OpParam &p) { return _D__rbin<T,CL>(p); }
template<class T,class CL> inline BL D__cbin(OpParam &p) { return _D__cbin<T,CL>(p); }
template<class T,class CL> inline BL D__rop(OpParam &p) { return _D__rop<T,CL>(p); }
template<class T,class CL> inline BL D__cop(OpParam &p) { return _D__cop<T,CL>(p); }
template<class T,V FUN(T &v,T a)> inline BL F__run(OpParam &p) { return V__run<T,FUN>(p.rsdt,p.rss,p.rddt,p.rds,p.frames); }
template<class T,V FUN(T &rv,T &iv,T ra,T ia)> inline BL F__cun(OpParam &p) { return V__cun<T,FUN>(p.rsdt,p.isdt,p.rss,p.iss,p.rddt,p.iddt,p.rds,p.ids,p.frames); }
template<class T,V FUN(T &v,T a,T b)> inline BL F__rbin(OpParam &p) { return _F__rbin<T,FUN>(p); }
template<class T,V FUN(T &rv,T &iv,T ra,T ia,T rb,T ib)> inline BL V__cbin(OpParam &p) { return _F__cbin<T,FUN>(p); }
template<class T,V FUN(T &v,T r,OpParam &p)> inline BL F__rop(OpParam &p) { return V__rop<T,OpParam &,FUN>(p,p.rsdt,p.rss,p.rddt,p.rds,p.frames); }
template<class T,V FUN(T &rv,T &iv,T ra,T ia,OpParam &p)> inline BL F__cop(OpParam &p) { return V__cop<T,OpParam &,FUN>(p,p.rsdt,p.isdt,p.rss,p.iss,p.rddt,p.iddt,p.rds,p.ids,p.frames); }
#endif

} // namespace VecOp

#endif
