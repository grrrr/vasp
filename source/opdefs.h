/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPDEFS_H
#define __VASP_OPDEFS_H

#define _D_BLOCK 1024

#define _D_MIN(a,b) ((a) < (b)?(a):(b))

#ifdef FLEXT_THREADS
#define _D_LOOP(VAR,LEN) \
	for(I i = 0; i < p.frames; flext_base::ThrYield()) \
	for(I m = _D_MIN(p.frames,i+_D_BLOCK); i < m; ++i)
#else
#define _D_LOOP(VAR,LEN) \
	for(I i = 0; i < p.frames; ++i)
#endif

/*! \brief skeleton for unary real operations
*/
#define _D__run(fun,p)											\
{																\
	register const S *sr = p.rsdt;								\
	register S *dr = p.rddt;									\
	if(sr == dr)												\
		if(p.rds == 1)											\
			_D_LOOP(i,p.frames)									\
			{ fun(*dr,*dr); dr++; }								\
		else													\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*dr); dr += p.rds; }						\
	else														\
		if(p.rss == 1 && p.rds == 1)							\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*sr); sr++,dr++; }						\
		else													\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*sr); sr += p.rss,dr += p.rds; }			\
	return true;												\
}

#define d__run(fun,p) { return _d__run(fun,p); }

/*! \brief skeleton for unary complex operations
*/
#define _D__cun(fun,p)											\
{																\
	register const S *sr = p.rsdt,*si = p.isdt;					\
	register S *dr = p.rddt,*di = p.iddt;						\
	if(sr == dr && si == di)									\
		if(p.rds == 1 && p.ids == 1)							\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*di,*dr,*di); dr++,di++; }				\
		else													\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*di,*dr,*di); dr += p.rds,di += p.ids; }	\
	else														\
		if(p.rss == 1 && p.iss == 1 && p.rds == 1 && p.ids == 1) \
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*di,*sr,*si); sr++,si++,dr++,di++; }		\
		else													\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*di,*sr,*si);	sr += p.rss,si += p.iss,dr += p.rds,di += p.ids; } \
	return true;												\
}

#define d__cun(fun,p) { return _d__cun(fun,p); }


/*! \brief skeleton for binary real operations
*/
#define _D__rbin(fun,p)											\
{																\
	register const S *sr = p.rsdt;								\
	register S *dr = p.rddt;									\
	if(p.HasArg() && p.arg[0].Is()) {											\
		switch(p.arg[0].argtp) {									\
		case OpParam::Arg::arg_v: {								\
			register const S *ar = p.arg[0].v.rdt;					\
			if(p.rsdt == p.rddt)									\
				if(p.rds == 1 && p.arg[0].v.rs == 1)				\
					_D_LOOP(i,p.frames)									\
					{ fun(*dr,*dr,*ar); dr++,ar++; }				\
				else												\
					_D_LOOP(i,p.frames)									\
					{ fun(*dr,*dr,*ar);	dr += p.rds,ar += p.arg[0].v.rs; } \
			else													\
				if(p.rss == 1 && p.rds == 1 && p.arg[0].v.rs == 1)	\
					_D_LOOP(i,p.frames)									\
					{ fun(*dr,*sr,*ar); sr++,dr++,ar++; }			\
				else												\
					_D_LOOP(i,p.frames)									\
					{ fun(*dr,*sr,*ar);	sr += p.rss,dr += p.rds,ar += p.arg[0].v.rs; } \
			break;												\
		}														\
		case OpParam::Arg::arg_env: {							\
			Env::Iter it(*p.arg[0].e.env); it.Init(0);			\
			if(p.rsdt == p.rddt)									\
				if(p.rds == 1)										\
					_D_LOOP(i,p.frames)									\
					{ fun(*dr,*dr,it.ValFwd(i)); dr++; }			\
				else												\
					_D_LOOP(i,p.frames)									\
					{ fun(*dr,*dr,it.ValFwd(i)); dr += p.rds; }		\
			else													\
				if(p.rss == 1 && p.rds == 1)						\
					_D_LOOP(i,p.frames)									\
					{ fun(*dr,*sr,it.ValFwd(i)); sr++,dr++; }		\
				else												\
					_D_LOOP(i,p.frames)								 	\
					{ fun(*dr,*sr,it.ValFwd(i)); sr += p.rss,dr += p.rds; }	\
			break;												\
		}														\
		case OpParam::Arg::arg_x: {							\
			const R v =  p.arg[0].x.r;							\
			if(p.rsdt == p.rddt)									\
				if(p.rds == 1)										\
					_D_LOOP(i,p.frames)									\
					{ fun(*dr,*dr,v); dr++; }						\
				else												\
					_D_LOOP(i,p.frames)								 	\
					{ fun(*dr,*dr,v); dr += p.rds; }				\
			else													\
				if(p.rss == 1 && p.rds == 1)						\
					_D_LOOP(i,p.frames)									\
					{ fun(*dr,*sr,v); sr++,dr++; }					\
				else												\
					_D_LOOP(i,p.frames)								 	\
					{ fun(*dr,*sr,v); sr += p.rss,dr += p.rds; }	\
			break;												\
		}														\
		}														\
	}															\
	else {														\
		register const S v = p.rbin.arg;						\
		if(p.rsdt == p.rddt)									\
			if(p.rds == 1)										\
				_D_LOOP(i,p.frames)									\
				{ fun(*dr,*dr,v); dr++; }						\
			else												\
				_D_LOOP(i,p.frames)									\
				{ fun(*dr,*dr,v); dr += p.rds; }				\
		else													\
			if(p.rss == 1 && p.rds == 1)						\
				_D_LOOP(i,p.frames)									\
				{ fun(*dr,*sr,v); sr++,dr++; }					\
			else												\
				_D_LOOP(i,p.frames)									\
				{ fun(*dr,*sr,v); sr += p.rss,dr += p.rds; }	\
	}															\
	return true;												\
}

#define d__rbin(fun,p) { return _d__rbin(fun,p); }


/*! \brief skeleton for binary complex operations
*/
#define _D__cbin(fun,p)											\
{																\
	register const S *sr = p.rsdt,*si = p.isdt;					\
	register S *dr = p.rddt,*di = p.iddt;						\
	if(p.HasArg() && p.arg[0].Is()) {											\
		switch(p.arg[0].argtp) {									\
		case OpParam::Arg::arg_v: {									\
			register const S *ar = p.arg[0].v.rdt,*ai = p.arg[0].v.idt;				\
			if(ai)													\
				if(sr == dr && si == di)							\
					if(p.rds == 1 && p.ids == 1 && p.arg[0].v.rs == 1 && p.arg[0].v.is == 1) \
						_D_LOOP(i,p.frames)				 \
						{ fun(*dr,*di,*dr,*di,*ar,*ai);	dr++,di++,ar++,ai++; }		\
					else											\
						_D_LOOP(i,p.frames)				 \
						{ fun(*dr,*di,*dr,*di,*ar,*ai);	dr += p.rds,di += p.ids,ar += p.arg[0].v.rs,ai += p.arg[0].v.is; }		\
				else												\
					_D_LOOP(i,p.frames)				 \
					{ fun(*dr,*di,*sr,*si,*ar,*ai);	sr += p.rss,si += p.iss,dr += p.rds,di += p.ids,ar += p.arg[0].v.rs,ai += p.arg[0].v.is; }			\
			else													\
				if(sr == dr && si == di)							\
					_D_LOOP(i,p.frames)				 \
					{ fun(*dr,*di,*dr,*di,*ar,0); dr += p.rds,di += p.ids,ar += p.arg[0].v.rs; }	\
				else												\
					_D_LOOP(i,p.frames)				 \
					{ fun(*dr,*di,*sr,*si,*ar,0); sr += p.rss,si += p.iss,dr += p.rds,di += p.ids,ar += p.arg[0].v.rs; } \
			break;												\
		}														\
		case OpParam::Arg::arg_env: {									\
			Env::Iter it(*p.arg[0].e.env); it.Init(0);				\
			if(sr == dr && si == di)							\
				if(p.rds == 1 && p.ids == 1) \
					_D_LOOP(i,p.frames)				 \
					{ fun(*dr,*di,*dr,*di,it.ValFwd(i),0); dr++,di++; }			\
				else											\
					_D_LOOP(i,p.frames)				 \
					{ fun(*dr,*di,*dr,*di,it.ValFwd(i),0); dr += p.rds,di += p.ids; }			\
			else												\
				_D_LOOP(i,p.frames)				 \
					{ fun(*dr,*di,*sr,*si,it.ValFwd(i),0); sr += p.rss,si += p.iss,dr += p.rds,di += p.ids; }	\
			break;												\
		}														\
		case OpParam::Arg::arg_x: {									\
			register const R ar = p.arg[0].x.r,ai = p.arg[0].x.i;				\
			if(sr == dr && si == di)							\
				if(p.rds == 1 && p.ids == 1) \
					_D_LOOP(i,p.frames)				 \
					{ fun(*dr,*di,*dr,*di,ar,ai); dr++,di++; }			\
				else											\
					_D_LOOP(i,p.frames)				 \
					{ fun(*dr,*di,*dr,*di,ar,ai); dr += p.rds,di += p.ids; }			\
			else												\
				_D_LOOP(i,p.frames)				 \
				{ fun(*dr,*di,*sr,*si,ar,ai); sr += p.rss,si += p.iss,dr += p.rds,di += p.ids; }	\
			break;												\
		}														\
		}														\
	}															\
	else {														\
		register const S rv = p.cbin.rarg,iv = p.cbin.iarg;		\
		if(sr == dr && si == di)								\
			if(p.rds == 1 && p.ids == 1)						\
				_D_LOOP(i,p.frames)						\
				{ fun(*dr,*di,*dr,*di,rv,iv); dr++,di++; }					\
			else												\
				_D_LOOP(i,p.frames)				 \
				{ fun(*dr,*di,*dr,*di,rv,iv); dr += p.rds,di += p.ids; }					\
		else													\
			if(p.rds == 1 && p.ids == 1 && p.rss == 1 && p.iss == 1) \
				_D_LOOP(i,p.frames)				 \
				{ fun(*dr,*di,*sr,*si,rv,iv); sr++,si++,dr++,di++; }					\
			else												\
				_D_LOOP(i,p.frames)				 \
				{ fun(*dr,*di,*sr,*si,rv,iv); sr += p.rss,si += p.iss,dr += p.rds,di += p.ids; }					\
	}															\
	return true;												\
}

#define d__cbin(fun,p) { return _d__cbin(fun,p); }


/*! \brief skeleton for real operations with parameter block
*/
#define _D__rop(fun,p)											\
{																\
	register const S *sr = p.rsdt;								\
	register S *dr = p.rddt;									\
	if(sr == dr)												\
		if(p.rds == 1)											\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*dr,p); dr++; }							\
		else													\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*dr,p); dr += p.rds; }					\
	else														\
		if(p.rss == 1 && p.rds == 1)							\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*sr,p); sr++,dr++; }						\
		else													\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*sr,p); sr += p.rss,dr += p.rds; }		\
	return true;												\
}

#define d__rop(fun,p) { return _d__rop(fun,p); }


/*! \brief skeleton for complex operations with parameter block
*/
#define _D__cop(fun,p)											\
{																\
	register const S *sr = p.rsdt,*si = p.isdt;					\
	register S *dr = p.rddt,*di = p.iddt;						\
	if(sr == dr && si == di)									\
		if(p.rds == 1 && p.ids == 1)							\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*di,*dr,*di,p); dr++,di++; }				\
		else													\
			_D_LOOP(i,p.frames)										\
			{ fun(*dr,*di,*dr,*di,p); dr += p.rds,di += p.ids; } \
	else														\
		if(p.rss == 1 && p.iss == 1 && p.rds == 1 && p.ids == 1) \
			_D_LOOP(i,p.frames)				 \
			{ fun(*dr,*di,*sr,*si,p); sr++,si++,dr++,di++; }	\
		else													\
			_D_LOOP(i,p.frames)				 \
			{ fun(*dr,*di,*sr,*si,p); sr += p.rss,si += p.iss,dr += p.rds,di += p.ids; }	\
	return true;												\
}

#define d__cop(fun,p) { return _d__cop(fun,p); }


#ifdef VASP_COMPACT
#define D__run(fun,p) d__run(fun,p)
#define D__cun(fun,p) d__cun(fun,p)
#define D__rbin(fun,p) d__rbin(fun,p)
#define D__cbin(fun,p) d__cbin(fun,p)
#define D__rop(fun,p) d__rop(fun,p)
#define D__cop(fun,p) d__cop(fun,p)
#else
#define D__run(fun,p) _D__run(fun,p)
#define D__cun(fun,p) _D__cun(fun,p)
#define D__rbin(fun,p) _D__rbin(fun,p)
#define D__cbin(fun,p) _D__cbin(fun,p)
#define D__rop(fun,p) _D__rop(fun,p)
#define D__cop(fun,p) _D__cop(fun,p)
#endif

#endif
