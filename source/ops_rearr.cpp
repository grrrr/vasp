/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

static BL d_shift(I cnt,F *dt,I str,F sh) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		// no zero filling!

		I ssh = ish*str;
		if(ish > 0)
			for(I i = cnt-1; i >= ish; ++i,dt += str) *dt = dt[-ssh];
		else
			for(I i = ish; i < cnt; ++i,dt += str) dt[-ssh] = *dt;
		return true;
	}
	else {
		// requires interpolation
		post("non-integer shift not implemented - truncating to integer");
		return d_shift(cnt,dt,str,ish);
	}
}

static BL d_xshift(I cnt,F *dt,I str,F sh) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		return d_shift(cnt/2,dt,str,sh) &&
		// eventual middle sample is left out
			d_shift(cnt/2,dt+(cnt-cnt/2)*str,str,-sh);
	}
	else {
		// requires interpolation
		post("non-integer xshift not implemented - truncating to integer");
		return d_xshift(cnt,dt,str,ish);
	}
}

/*
static BL d_xshift(I cnt,F *dt,I str,F sh) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		// no zero filling!

		F *d;
		I i,hcnt = cnt/2;
		// what to do if one sample remains? -> leave it in the middle

		I ssh = ish*str:
		if(ish > 0) {
//			for(i = hcnt-1; i >= ish; ++i,dt += str) dt[i] = dt[i-ish];
			for(d = dt+(i = hcnt-1)*str; i >= ish; ++i,d += str) *d = d[-ssh];
			
//			for(i = (cnt-hcnt)+ish; i < cnt; ++i) dt[i-ish] = dt[i];
			for(d = dt+(i = (cnt-hcnt)+ish)*str; i < cnt; ++i,d += str) d[-ssh] = *d;
		}
		else {
//			for(i = ish; i < hcnt; ++i) dt[i-ish] = dt[i];
			for(d = dt+(i = ish)*str; i < hcnt; ++i,d += str) d[-ssh] = *d;

//			for(i = cnt-1; i >= (cnt-hcnt)+ish; ++i) dt[i] = dt[i-ish];
			for(d = dt+(i = cnt-1)*str; i >= (cnt-hcnt)+ish; ++i,d += str) *d = d[-ssh];
		}
		return true;
	}
	else {
		// requires interpolation
		post("non-integer xshift not implemented - truncating to integer");
		return d_xshift(cnt,dt,str,ish);
	}
}
*/

static BL d_rot(I cnt,F *dt,I str,F sh) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
/*
		if(ish*2 > cnt) 
			// if more than half is rotated -> change direction
			d_rot(dt,ish-cnt,cnt);
		else {
*/
			I ssh = ish*str;
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
				for(I i = ish; i < cnt-ish; ++i,dt += str) dt[-ssh] = *dt;
			}
//		}
		return false; 
	}
	else {
		// requires interpolation
		post("non-integer rot not implemented - truncating to integer");
		return d_rot(cnt,dt,str,ish);
	}
}

static BL d_xrot(I cnt,F *dt,I str,F sh) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		return d_rot(cnt/2,dt,str,sh) &&
		// eventual middle sample is left out
			d_rot(cnt/2,dt+(cnt-cnt/2)*str,str,-sh);
	}
	else {
		// requires interpolation
		post("non-integer xrot not implemented - truncating to integer");
		return d_xrot(cnt,dt,str,ish);
	}
}

static BL d_mirr(I cnt,F *dt,I str,F) 
{ 
	for(F *dl = dt,*du = dt+(cnt-1)*str; dl < du; dl += str,du -= str) {
		F t;
		t = *dl; *dl = *du; *du = t;
	}
	return true; 
}

static BL d_xmirr(I cnt,F *dt,I str,F) 
{ 
	return d_mirr(cnt/2,dt,str,0) &&
	// eventual middle sample is left out
		d_mirr(cnt/2,dt+(cnt-cnt/2)*str,str,0);
}


/*
Vasp *Vasp::m_shift(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_arg("shift",arg.GetFloat(),d_shift):NULL; 
}


Vasp *Vasp::m_xshift(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_arg("xshift",arg.GetFloat(),d_xshift):NULL; 
}

Vasp *Vasp::m_rot(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_arg("rot",arg.GetFloat(),d_rot):NULL; 
}

Vasp *Vasp::m_xrot(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_arg("xrot",arg.GetFloat(),d_xrot):NULL; 
}

Vasp *Vasp::m_mirr() { return fr_arg("mirr",0,d_mirr); }
Vasp *Vasp::m_xmirr() { return fr_arg("xmirr",0,d_xmirr); } 
*/


Vasp *Vasp::m_shift(const Argument &arg,Vasp *dst,BL symm) 
{ 
	if(!arg.IsFloat()) return NULL;
	
	return fr_arg("shift",arg.GetFloat(),d_shift); 
}



Vasp *Vasp::m_rot(const Argument &arg,Vasp *dst,BL symm) 
{ 
	if(!arg.IsFloat()) return NULL;
	
	return fr_arg("rot",arg.GetFloat(),d_rot); 
}


Vasp *Vasp::m_mirr(Vasp *dst,BL symm) 
{ 
	return fr_arg("mirr",0,d_mirr); 
}

