/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPDEFS_H
#define __VASP_OPDEFS_H

/*! \brief skeleton for unary real operations
*/
#define _D__run(fun,p)											\
{																\
	register const S *sr = p.rsdt;								\
	register S *dr = p.rddt;									\
	if(sr == dr)												\
		if(p.rds == 1)											\
			for(I i = 0; i < p.frames; ++i,dr++)				\
				fun(*dr,*dr);									\
		else													\
			for(I i = 0; i < p.frames; ++i,dr += p.rds)			\
				fun(*dr,*dr);									\
	else														\
		if(p.rss == 1 && p.rds == 1)							\
			for(I i = 0; i < p.frames; ++i,sr++,dr++)			\
				fun(*dr,*sr);									\
		else													\
			for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds)	\
				fun(*dr,*sr);									\
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
			for(I i = 0; i < p.frames; ++i,dr++,di++)			\
				fun(*dr,*di,*dr,*di);							\
		else													\
			for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) \
				fun(*dr,*di,*dr,*di);							\
	else														\
		if(p.rss == 1 && p.iss == 1 && p.rds == 1 && p.ids == 1) \
			for(I i = 0; i < p.frames; ++i,sr++,si++,dr++,di++) \
				fun(*dr,*di,*sr,*si);							\
		else													\
			for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) \
				fun(*dr,*di,*sr,*si);							\
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
					for(I i = 0; i < p.frames; ++i,dr++,ar++)		\
						fun(*dr,*dr,*ar);							\
				else												\
					for(I i = 0; i < p.frames; ++i,dr += p.rds,ar += p.arg[0].v.rs) \
						fun(*dr,*dr,*ar);							\
			else													\
				if(p.rss == 1 && p.rds == 1 && p.arg[0].v.rs == 1)	\
					for(I i = 0; i < p.frames; ++i,sr++,dr++,ar++)  \
						fun(*dr,*sr,*ar);							\
				else												\
					for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds,ar += p.arg[0].v.rs) \
						fun(*dr,*sr,*ar);							\
			break;												\
		}														\
		case OpParam::Arg::arg_env: {							\
			Env::Iter it(*p.arg[0].e.env); it.Init(0);			\
			if(p.rsdt == p.rddt)									\
				if(p.rds == 1)										\
					for(I i = 0; i < p.frames; ++i,dr++)			\
						fun(*dr,*dr,it.ValFwd(i));								\
				else												\
					for(I i = 0; i < p.frames; ++i,dr += p.rds) 	\
						fun(*dr,*dr,it.ValFwd(i));								\
			else													\
				if(p.rss == 1 && p.rds == 1)						\
					for(I i = 0; i < p.frames; ++i,sr++,dr++)		\
						fun(*dr,*sr,it.ValFwd(i));								\
				else												\
					for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds) 	\
						fun(*dr,*sr,it.ValFwd(i));							\
			break;												\
		}														\
		case OpParam::Arg::arg_x: {							\
			const R v =  p.arg[0].x.r;							\
			if(p.rsdt == p.rddt)									\
				if(p.rds == 1)										\
					for(I i = 0; i < p.frames; ++i,dr++)			\
						fun(*dr,*dr,v);								\
				else												\
					for(I i = 0; i < p.frames; ++i,dr += p.rds) 	\
						fun(*dr,*dr,v);								\
			else													\
				if(p.rss == 1 && p.rds == 1)						\
					for(I i = 0; i < p.frames; ++i,sr++,dr++)		\
						fun(*dr,*sr,v);								\
				else												\
					for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds) 	\
						fun(*dr,*sr,v);							\
			break;												\
		}														\
		}														\
	}															\
	else {														\
		register const S v = p.rbin.arg;						\
		if(p.rsdt == p.rddt)									\
			if(p.rds == 1)										\
				for(I i = 0; i < p.frames; ++i,dr++)			\
					fun(*dr,*dr,v);								\
			else												\
				for(I i = 0; i < p.frames; ++i,dr += p.rds)		\
					fun(*dr,*dr,v);								\
		else													\
			if(p.rss == 1 && p.rds == 1)						\
				for(I i = 0; i < p.frames; ++i,sr++,dr++)		\
					fun(*dr,*sr,v);								\
			else												\
				for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds) \
					fun(*dr,*sr,v);								\
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
						for(I i = 0; i < p.frames; ++i,dr++,di++,ar++,ai++) \
							fun(*dr,*di,*dr,*di,*ar,*ai);			\
					else											\
						for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids,ar += p.arg[0].v.rs,ai += p.arg[0].v.is) \
							fun(*dr,*di,*dr,*di,*ar,*ai);			\
				else												\
					for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids,ar += p.arg[0].v.rs,ai += p.arg[0].v.is) \
						fun(*dr,*di,*sr,*si,*ar,*ai);				\
			else													\
				if(sr == dr && si == di)							\
					for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids,ar += p.arg[0].v.rs) \
						fun(*dr,*di,*dr,*di,*ar,0);					\
				else												\
					for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids,ar += p.arg[0].v.rs) \
						fun(*dr,*di,*sr,*si,*ar,0);					\
			break;												\
		}														\
		case OpParam::Arg::arg_env: {									\
			Env::Iter it(*p.arg[0].e.env); it.Init(0);				\
			if(sr == dr && si == di)							\
				if(p.rds == 1 && p.ids == 1) \
					for(I i = 0; i < p.frames; ++i,dr++,di++) \
						fun(*dr,*di,*dr,*di,it.ValFwd(i),0);			\
				else											\
					for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) \
						fun(*dr,*di,*dr,*di,it.ValFwd(i),0);			\
			else												\
				for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) \
					fun(*dr,*di,*sr,*si,it.ValFwd(i),0);				\
			break;												\
		}														\
		case OpParam::Arg::arg_x: {									\
			register const R ar = p.arg[0].x.r,ai = p.arg[0].x.i;				\
			if(sr == dr && si == di)							\
				if(p.rds == 1 && p.ids == 1) \
					for(I i = 0; i < p.frames; ++i,dr++,di++) \
						fun(*dr,*di,*dr,*di,ar,ai);			\
				else											\
					for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) \
						fun(*dr,*di,*dr,*di,ar,ai);			\
			else												\
				for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) \
					fun(*dr,*di,*sr,*si,ar,ai);				\
			break;												\
		}														\
		}														\
	}															\
	else {														\
		register const S rv = p.cbin.rarg,iv = p.cbin.iarg;		\
		if(sr == dr && si == di)								\
			if(p.rds == 1 && p.ids == 1)						\
				for(I i = 0; i < p.frames; ++i,dr++,di++)		\
					fun(*dr,*di,*dr,*di,rv,iv);					\
			else												\
				for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) \
					fun(*dr,*di,*dr,*di,rv,iv);					\
		else													\
			if(p.rds == 1 && p.ids == 1 && p.rss == 1 && p.iss == 1) \
				for(I i = 0; i < p.frames; ++i,sr++,si++,dr++,di++) \
					fun(*dr,*di,*sr,*si,rv,iv);					\
			else												\
				for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) \
					fun(*dr,*di,*sr,*si,rv,iv);					\
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
			for(I i = 0; i < p.frames; ++i,dr++)				\
				fun(*dr,*dr,p);									\
		else													\
			for(I i = 0; i < p.frames; ++i,dr += p.rds)			\
				fun(*dr,*dr,p);									\
	else														\
		if(p.rss == 1 && p.rds == 1)							\
			for(I i = 0; i < p.frames; ++i,sr++,dr++)			\
				fun(*dr,*sr,p);									\
		else													\
			for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds)	\
				fun(*dr,*sr,p);									\
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
			for(I i = 0; i < p.frames; ++i,dr++,di++)			\
				fun(*dr,*di,*dr,*di,p);							\
		else													\
			for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) \
				fun(*dr,*di,*dr,*di,p);							\
	else														\
		if(p.rss == 1 && p.iss == 1 && p.rds == 1 && p.ids == 1) \
			for(I i = 0; i < p.frames; ++i,sr++,si++,dr++,di++) \
				fun(*dr,*di,*sr,*si,p);							\
		else													\
			for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) \
				fun(*dr,*di,*sr,*si,p);							\
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
