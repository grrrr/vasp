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
BL VecOp::d_shift(I cnt,const S *src,I sstr,S *dst,I dstr,R sh) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		// no zero filling!

		I ssh = ish*str;
		if(ish > 0)
			for(I i = cnt-1; i >= ish; ++i,src += sstr,dst += dstr) *dst = src[-ssh];
		else
			for(I i = ish; i < cnt; ++i,src += sstr,dst += dstr) dst[-ssh] = *src;
		return true;
	}
	else {
		// requires interpolation
		post("non-integer shift not implemented - truncating to integer");
		return d_shift(cnt,src,sstr,dst,dstr,ish);
	}
}


/*! \brief rotate buffer
	
	\todo implement!
	\todo check for src/dst overlap
*/
BL VecOp::d_rot(I cnt,const S *src,I sstr,S *dst,I dstr,R sh) 
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
				for(I i = ish; i < cnt-ish; ++i,src += sstr,dst += dstr) dst[-ssh] = *src;
			}
//		}
		return false; 
	}
	else {
		// requires interpolation
		post("non-integer rot not implemented - truncating to integer");
		return d_rot(cnt,src,sstr,dst,dstr,ish);
	}
}

Vasp *VaspOp::m_shift(Vasp &src,const Argument &arg,Vasp *dst,BL shift,BL symm) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		R sh = flx::GetADouble(arg.GetList()[0]);

		RVecBlock *vecs = GetRVecs(shift?(symm?"xshift":"shift"):(symm?"xrot":"rot"),src,dst);
		if(vecs) {
			BL ok = true;
			for(I i = 0; ok && i < vecs->Vecs(); ++i) {
				VBuffer *s = vecs->Src(i),*d = vecs->Dst(i);
				if(!d) d = s; // if there's no dst, take src

				I cnt = vecs->Frames();
				I sc = s->Channels(),dc = d->Channels();
				if(shift) {
					if(symm) {
						// symmetric mode
						I hcnt = cnt/2;
						ok = 
							d_shift(hcnt,s->Pointer(),sc,d->Pointer(),dc,sh) &&
							d_shift(hcnt,s->Pointer()+(cnt-hcnt),sc,d->Pointer()+(cnt-hcnt),dc,sh);
					}
					else
						// normal mode
						ok = d_shift(cnt,s->Pointer(),sc,d->Pointer(),dc,sh);
				}
				else {
					if(symm) {
						// symmetric mode
						I hcnt = cnt/2;
						ok = 
							d_rot(hcnt,s->Pointer(),sc,d->Pointer(),dc,sh) &&
							d_rot(hcnt,s->Pointer()+(cnt-hcnt),sc,d->Pointer()+(cnt-hcnt),dc,sh);
					}
					else
						// normal mode
						ok = d_rot(cnt,s->Pointer(),sc,d->Pointer(),dc,sh);
				}
			}
			return ok?vecs->ResVasp():NULL;
		}
		else
			return NULL;
	}
	else return NULL;
}


/*! \brief mirror buffer
	
	\todo check for src/dst overlap
*/
BL VecOp::d_mirr(I cnt,const S *src,I sstr,S *dst,I dstr) 
{ 
	if(dst == src) 
		for(S *dl = dst,*du = dst+(cnt-1)*dstr; dl < du; dl += dstr,du -= dstr) {
			F t;
			t = *dl; *dl = *du; *du = t;
		}
	else {
		const S *ds = src;
		S *dd = dst+(cnt-1)*dstr;
		for(I i = 0; i < cnt; ++cnt,ds += sstr,dd -= dstr) *dd = *ds;
	}
	return true; 
}

Vasp *VaspOp::m_mirr(Vasp &src,const Argument &arg,Vasp *dst,BL symm) 
{ 
	RVecBlock *vecs = GetRVecs(symm?"xmirr":"mirr",src,dst);
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
					d_mirr(hcnt,s->Pointer(),sc,d->Pointer(),dc) &&
					d_mirr(hcnt,s->Pointer()+(cnt-hcnt),sc,d->Pointer()+(cnt-hcnt),dc);
			}
			else
				// normal mode
				ok = d_mirr(cnt,s->Pointer(),sc,d->Pointer(),dc);
		}
		return ok?vecs->ResVasp():NULL;
	}
	else
		return NULL;
}

