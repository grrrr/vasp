/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

static BL d_shift(F *dt,F sh,I cnt) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		// no zero filling!

		if(ish > 0)
			for(I i = cnt-1; i >= ish; ++i,++dt) *dt = dt[-ish];
		else
			for(I i = ish; i < cnt; ++i,++dt) dt[-ish] = *dt;
		return true;
	}
	else {
		// requires interpolation
		post("non-integer shift not implemented - truncating to integer");
		return d_shift(dt,ish,cnt);
	}
}

static BL d_xshift(F *dt,F sh,I cnt) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		// no zero filling!

		I i,hcnt = cnt/2;
		// what to do if one sample remains? -> leave it in the middle

		if(ish > 0) {
			for(i = hcnt-1; i >= ish; ++i) dt[i] = dt[i-ish];
			for(i = (cnt-hcnt)+ish; i < cnt; ++i) dt[i-ish] = dt[i];
		}
		else {
			for(i = ish; i < hcnt; ++i) dt[i-ish] = dt[i];
			for(i = cnt-1; i >= (cnt-hcnt)+ish; ++i) dt[i] = dt[i-ish];
		}
		return true;
	}
	else {
		// requires interpolation
		post("non-integer xshift not implemented - truncating to integer");
		return d_xshift(dt,ish,cnt);
	}
}

static BL d_rot(F *dt,F sh,I cnt) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
/*
		if(ish*2 > cnt) 
			// if more than half is rotated -> change direction
			d_rot(dt,ish-cnt,cnt);
		else {
*/
			if(ish > 0) {
				for(I i = cnt-1; i >= 0; ++i) {
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
				for(I i = ish; i < cnt-ish; ++i,++dt) dt[-ish] = *dt;
			}
//		}
		return false; 
	}
	else {
		// requires interpolation
		post("non-integer rot not implemented - truncating to integer");
		return d_rot(dt,ish,cnt);
	}
}

static BL d_xrot(F *dt,F sh,I cnt) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		return d_rot(dt,sh,cnt/2) &&
		// eventual middle sample is left out
			d_rot(dt+(cnt-cnt/2),-sh,cnt/2);
	}
	else {
		// requires interpolation
		post("non-integer xrot not implemented - truncating to integer");
		return d_xrot(dt,ish,cnt);
	}
}

static BL d_mirr(F *dt,F,I cnt) 
{ 
	for(F *dl = dt,*du = dt+cnt-1; dl < du; ++dl,--du) {
		F t;
		t = *dl; *dl = *du; *du = t;
	}
	return true; 
}

static BL d_xmirr(F *dt,F,I cnt) 
{ 
	return d_mirr(dt,0,cnt/2) &&
	// eventual middle sample is left out
		d_mirr(dt+(cnt-cnt/2),0,cnt/2);
}


Vasp *Vasp::m_shift(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_nop("shift",arg.GetFloat(),d_shift):NULL; 
}

Vasp *Vasp::m_xshift(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_nop("xshift",arg.GetFloat(),d_xshift):NULL; 
}

Vasp *Vasp::m_rot(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_nop("rot",arg.GetFloat(),d_rot):NULL; 
}

Vasp *Vasp::m_xrot(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_nop("xrot",arg.GetFloat(),d_xrot):NULL; 
}

Vasp *Vasp::m_mirr() { return fr_nop("mirr",0,d_mirr); }
Vasp *Vasp::m_xmirr() { return fr_nop("xmirr",0,d_xmirr); } 


