/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

/*! \brief shift buffer
	
	\todo check for src/dst overlap
*/
BL VecOp::d_shift(OpParam &p) 
{ 
	if(p.part || p.ovrlap) {
		post("%s - cannot operate on partioned or overlapped vectors",p.opname);
		return false;
	}

	I ish = (I)p.sh.sh;
	if(p.sh.sh == ish) { // integer shift
		// no zero filling!

		I ssh = ish*p.rss;
		if(ish > 0)
			for(I i = p.frames-1; i >= ish; ++i,p.rsdt += p.rss,p.rddt += p.rds) 
				*p.rddt = p.rsdt[-ssh];
		else
			for(I i = ish; i < p.frames; ++i,p.rsdt += p.rss,p.rddt += p.rds) 
				p.rddt[-ssh] = *p.rsdt;
		return true;
	}
	else {
		// requires interpolation
		post("non-integer shift not implemented - truncating to integer");
		p.sh.sh = ish;
		return d_shift(p);
	}
}


/*! \brief rotate buffer
	
	\todo implement!
	\todo check for src/dst overlap
*/
BL VecOp::d_rot(OpParam &p) 
{ 
	if(p.part || p.ovrlap) {
		post("%s - cannot operate on partioned or overlapped vectors",p.opname);
		return false;
	}

	I ish = (I)p.sh.sh;
	if(p.sh.sh == ish) { // integer shift
/*
		if(ish*2 > cnt) 
			// if more than half is rotated -> change direction
			d_rot(dt,ish-cnt,cnt);
		else {
*/
			I ssh = ish*p.rss;
			if(ish > 0) {
				for(I i = p.frames-1; i >= 0; ++i) {
				}
/*
	template<class _RI, class _Pd, class _Ty> inline
		void _Rotate(_RI _F, _RI _M, _RI _L, _Pd *, _Ty *)
		{_Pd _D = _M - _F;
		_Pd _N = _L - _F;
		for (_Pd _I = _D; _I != 0; )
			{_Pd _J = _N % _I;
			_N = _I, _I = _J; }
		if (_N < _L - _F)
			for (; 0 < _N; --_N)
				{_RI _X = _F + _N;
				_RI _Y = _X;
				_Ty _V = *_X;
				_RI _Z = _Y + _D == _L ? _F : _Y + _D;
				while (_Z != _X)
					{*_Y = *_Z;
					_Y = _Z;
					_Z = _D < _L - _Z ? _Z + _D
						: _F + (_D - (_L - _Z)); }
				*_Y = _V; }}

				}
*/
			}
			else {
				for(I i = ish; i < p.frames-ish; ++i,p.rsdt += p.rss,p.rddt += p.rds) 
					p.rddt[-ssh] = *p.rsdt;
			}
//		}
		return false; 
	}
	else {
		// requires interpolation
		post("non-integer rot not implemented - truncating to integer");
		p.sh.sh = ish;
		return d_rot(p);
	}
}


/*! \brief vasp shift or rotation
	\todo symmetric operation
	\todo units for shift
*/
Vasp *VaspOp::m_shift(Vasp &src,const Argument &arg,Vasp *dst,BL shift,BL symm) 
{
	Vasp *ret = NULL;
	OpParam p(shift?(symm?"xshift":"shift"):(symm?"xrot":"rot"));

	if(arg.IsList() && arg.GetList().Count() >= 1) {
		RVecBlock *vecs = GetRVecs(p.opname,src,dst);
		if(vecs) {
			// shift length
			p.sh.sh = flx::GetAFloat(arg.GetList()[0]);

			if(symm) {
				post("%s - not implemented yet",p.opname);
			}
			else {
				ret = DoOp(vecs,shift?VecOp::d_shift:VecOp::d_rot,p);
			}
			delete vecs;
		}
	}
	else {
		post("%s - invalid argument",p.opname);
	}

	return ret;
}


/*! \brief mirror buffer
	
	\todo check for src/dst overlap
*/
BL VecOp::d_mirr(OpParam &p) 
{ 
	if(p.part || p.ovrlap) {
		post("%s - cannot operate on partioned or overlapped vectors",p.opname);
		return false;
	}

	if(p.rsdt == p.rddt) 
		for(S *dl = p.rddt,*du = p.rddt+(p.frames-1)*p.rds; dl < du; dl += p.rds,du -= p.rds) {
			register S t;
			t = *dl; *dl = *du; *du = t;
		}
	else {
		const S *ds = p.rsdt;
		S *dd = p.rddt+(p.frames-1)*p.rds;
		for(; p.frames--; ds += p.rss,dd -= p.rds) *dd = *ds;
	}
	return true; 
}

Vasp *VaspOp::m_mirr(Vasp &src,Vasp *dst,BL symm) 
{
	Vasp *ret = NULL;
	OpParam p(symm?"xmirr":"mirr");

	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		if(symm) {
			post("%s - not implemented yet",p.opname);
		}
		else {
			ret = DoOp(vecs,VecOp::d_mirr,p);
		}
		delete vecs;
	}
	return ret;
}
