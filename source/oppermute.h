/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPPERMUTE_H
#define __VASP_OPPERMUTE_H

#include "opparam.h"

#define MAXPERMDIM 2

template<class T>
inline void permswap(T &a,T &b) { register T t = a; a = b; b = t; }

template<class T>
void permutation1(OpParam &p,int (*origination)(int pos, int sz,OpParam &p))
{
	T *ddt = p.rddt;
	const I ds = p.rds;
	const I sz = p.frames;

	if(ddt != p.rsdt) {
		// not in place
		const T *sdt = p.rsdt;
		const I ss = p.rss;
		for(int i = 0; i < sz; ++i) ddt[origination(i,sz,p)*ds] = sdt[i*ss];
	}
	else {
		// in place 
		// \todo try to come from both sides!
		for(int i = 0; i <= sz-2; ++i) {
			int cur = i;
			do { cur = origination(cur,sz,p); } while(cur < i);
			if(cur > i) {
				// swap
				permswap(ddt[cur*ds],ddt[i*ds]);
			}
		}
	}
}

template<class T>
void permutation2(OpParam &p,int (*origination)(int pos, int sz,OpParam &p))
{
	T *rddt = p.rddt,*iddt = p.iddt;
	const I rds = p.rds,ids = p.ids;
	const I sz = p.frames;
	bool rinpl = rddt == p.rsdt,iinpl = iddt == p.isdt;

	if(rinpl == iinpl) {
		// re and im both in place

		for(int i = 0; i <= sz-2; ++i) {
			int cur = i;
			do { cur = origination(cur,sz,p); } while(cur < i);
			if(cur > i) {
				// swap
				permswap(rddt[cur*rds],rddt[i*rds]);
				permswap(iddt[cur*ids],iddt[i*ids]);
			}
		}
	}
	else {
		if(!rinpl) {
			const T *sdt = p.rsdt;
			const I ss = p.rss;
			if(ss == 1 && rds == 1)
				for(int i = 0; i < sz; ++i) *(rddt++) = *(sdt++);
			else
				for(int i = 0; i < sz; ++i,rddt += rds,sdt += ss) *rddt = *sdt;
			rddt = p.rddt;
		}
		else permutation1<T>(p,origination);

		if(!iinpl) {
			const T *sdt = p.isdt;
			const I ss = p.iss;
			if(ss == 1 && ids == 1)
				for(int i = 0; i < sz; ++i) *(iddt++) = *(sdt++);
			else
				for(int i = 0; i < sz; ++i,iddt += ids,sdt += ss) *iddt = *sdt;
			iddt = p.iddt;
		}
		else {
			permswap(p.rddt,p.iddt); permswap(p.rds,p.ids);
			permutation1<T>(p,origination);
			permswap(p.rddt,p.iddt); permswap(p.rds,p.ids);
		}
	}
}


#define PERMUTATION(tp,dim,p,func) permutation ## dim <tp>(p,func)

#endif

