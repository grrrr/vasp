/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPPARAM_H
#define __VASP_OPPARAM_H

#include "vecblk.h"

class OpParam {
public:
	OpParam(const C *opnm,I args);
	~OpParam();

	V InitArgs(I nargs);
	V Clear();

	BL HasArg() const { return arg != NULL; }

	// check for overlap 
	// \remark if on same vector, stride is the same for src, arg, dst!
	inline BL SR_In() const { return rddt > rsdt && rddt < rsdt+frames*rss; } 
	inline BL SI_In() const { return iddt > isdt && iddt < isdt+frames*iss; } 
	BL AR_In(I bl) const;
	BL AI_In(I bl) const;
	BL AR_In() const;
	BL AI_In() const;
	
	// Can we reverse direction?
	inline BL SR_Can() const { return rsdt <= rddt || rsdt >= rddt+frames*rds; } 
	inline BL SI_Can() const { return isdt <= iddt || isdt >= iddt+frames*ids; } 
	BL AR_Can(I bl) const;
	BL AI_Can(I bl) const;
	BL AR_Can() const;
	BL AI_Can() const;
	
	// does it overlap? (works only with rss,rds,ras.... > 0)
	inline BL SR_Ovr() const { return rddt != rsdt && rddt < rsdt+frames*rss && rsdt < rddt+frames*rds; } 
	inline BL SI_Ovr() const { return iddt != isdt && iddt < isdt+frames*iss && isdt < iddt+frames*ids; } 
	BL AR_Ovr(I bl) const;
	BL AI_Ovr(I bl) const;
	BL AR_Ovr() const;
	BL AI_Ovr() const;
	
	V AR_Rev();
	V AI_Rev();
	
	V R_Rev();
	V C_Rev();

	const C *opname;
	I frames,symm,args;
	BL ovrlap,revdir;
	S *rsdt,*isdt; I rss,iss;
	S *rddt,*iddt; I rds,ids;
	
	class Arg
	{
	public:
		Arg(): argtp(arg_) {}
		Arg(const Arg &op) { operator =(op); }
		~Arg() { Clear(); }
		V Clear();

		BL Is() const { return argtp != arg_; }

		Arg &operator =(const Arg &op);
		Arg &operator =(const Argument &arg);

		Arg &SetX(S r,S i = 0);
		Arg &SetV(S *r,I rs,S *i = NULL,I is = 0);
		Arg &SetL(I pts,R *r,R *i);

		enum { arg_ = 0,arg_x,arg_v,arg_l } argtp;
		union {
			struct { R r,i; } x;
			struct { S *rdt,*idt; I rs,is; } v;
			struct { I pts; R *r,*i; } l;
		};
	};

	Arg *arg;
	
//	S **radt,**iadt; I *ras,*ias;
	union {
		struct { R coef,carry; I rep; } flt;
		struct { R carry; I rep; } intdif;
		struct { BL cx; R density; I cnt; } peaks;
		struct { R ph,phinc; } gen;
		struct { R factor,center; I mode; } tilt; 
		struct { R cur,inc; } bvl;
		struct { R sh; I ish; } sh;
		struct { I wndtp; } wnd;
		struct { R minmax,scl; } norm;
		struct { I arg; } ibin;
		struct { R arg; } rbin; 
		struct { R rarg,iarg; } cbin; 
	};

private:
	// reverse direction
	inline V SR_Rev() { rsdt -= (frames-1)*(rss = -rss); }
	inline V SI_Rev() { isdt -= (frames-1)*(iss = -iss); }
	V AR_Rev(I bl);
	V AI_Rev(I bl);
	inline V DR_Rev() { rddt -= (frames-1)*(rds = -rds); }
	inline V DI_Rev() { iddt -= (frames-1)*(ids = -ids); }

};

#endif