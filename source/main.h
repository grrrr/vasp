/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_H
#define __VASP_H

#define VASP_VERSION "0.0.2"


#include <flext.h>

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 203)
#error You need at least flext version 0.2.3
#endif

#include <typeinfo.h>
#include <stdlib.h>

#define I int
#define L long
#define F float
#define D double
#define C char
#define BL bool
#define V void
#define S t_sample // type for samples
#define R double // type for internal calculations
#define CX complex
#define VX vector

class complex 
{ 
public:
	complex() {}
	complex(F re,F im = 0): real(re),imag(im) {}

	F real,imag; 
};

class vector 
{ 
public:
	vector(): dim(0),data(NULL) {}
	~vector() { if(data) delete[] data; }

	I Dim() const { return dim; }
	F *Data() { return data; }
	const F *Data() const { return data; }
protected:
	I dim; F *data; 
};

#ifndef PI
#define PI 3.1415926535897932385
#endif



class VBuffer:
	public flext_base::buffer
{
	typedef flext_base::buffer parent;

public:
	VBuffer(t_symbol *s = NULL,I chn = 0,I len = -1,I offs = 0);
	VBuffer(const VBuffer &v);
	~VBuffer();

	VBuffer &operator =(const VBuffer &v);
	VBuffer &Set(t_symbol *s = NULL,I chn = 0,I len = -1,I offs = 0);

	I Channel() const { return chn; }
	I Offset() const { return offs; }
	I Length() const { return len; }

	S *Pointer() { return Data()+Offset()*Channels()+Channel(); }

protected:
	I chn,offs,len;
};


class AtomList
{
public:
	AtomList(I argc,t_atom *argv = NULL);
	~AtomList();

	I Count() const { return cnt; }
	t_atom &operator [](I ix) { return lst[ix]; }
	const t_atom &operator [](I ix) const { return lst[ix]; }

	t_atom *Atoms() { return lst; }
	const t_atom *Atoms() const { return lst; }
	
protected:
	I cnt;
	t_atom *lst;
};


class XAtomList:
	public AtomList
{
public:
	XAtomList(const t_symbol *h,I argc,t_atom *argv): AtomList(argc,argv),hdr(h) {}

	const t_symbol *Header() const { return hdr; }
	
protected:
	const t_symbol *hdr;
};



class Vasp;

class Argument
{
public:
	Argument();
	~Argument();
	
	Argument &Parse(I argc,t_atom *argv);
	Argument &Clear();
	Argument &ClearAll();

	Argument &Set(Vasp *v);
	Argument &Set(I argc,t_atom *argv);
	Argument &Set(I i);
	Argument &Set(F f);
	Argument &Set(D d);
	Argument &Set(F re,F im);
	Argument &Set(VX *vec);

	Argument *Next() { return nxt; }
	Argument &Next(I i);
	Argument &Add(Argument *a);

	Argument &Add(Vasp *v);
	Argument &Add(I argc,t_atom *argv);
	Argument &Add(I i);
	Argument &Add(F f);
	Argument &Add(D d);
	Argument &Add(F re,F im);
	Argument &Add(VX *vec);

	BL IsNone() const { return tp == tp_none; }
	BL IsVasp() const { return tp == tp_vasp; }
	BL IsList() const { return tp == tp_list; }
	BL IsInt() const { return tp == tp_int; }
	BL CanbeInt() const { return tp == tp_int || tp == tp_float || tp_double; }
	BL IsFloat() const { return tp == tp_float; }
	BL CanbeFloat() const { return tp == tp_float || tp == tp_double || tp == tp_int; }
	BL IsDouble() const { return tp == tp_double; }
	BL CanbeDouble() const { return tp == tp_double || tp == tp_float || tp == tp_int; }
	BL IsComplex() const { return tp == tp_cx; }
	BL CanbeComplex() const { return tp == tp_cx || CanbeFloat(); }
	BL IsVector() const { return tp == tp_vx; }
	BL CanbeVector() const { return tp == tp_vx || CanbeComplex(); }

	const Vasp &GetVasp() const { return *dt.v; }
	const AtomList &GetList() const { return *dt.atoms; }
	I GetInt() const { return dt.i; }
	I GetAInt() const;
	F GetFloat() const { return dt.f; }
	F GetAFloat() const;
	D GetDouble() const { return dt.d; }
	D GetADouble() const;
	const CX &GetComplex() const { return *dt.cx; }
	CX GetAComplex() const;
	const VX &GetVector() const { return *dt.vx; }
	VX GetAVector() const;

protected:
	enum {
		tp_none,tp_vasp,tp_list,tp_int,tp_float,tp_double,tp_cx,tp_vx
	} tp;

	union {
		Vasp *v;
		AtomList *atoms;
		F f;
		D d;
		I i;
		CX *cx;
		VX *vx;
	} dt;

	Argument *nxt;
};


R arg(R re,R im);
inline R arg(const CX &c) { return arg(c.real,c.imag); }
inline F sqabs(F re,F im) { return re*re+im*im; }
inline F sqabs(const CX &c) { return sqabs(c.real,c.imag); }
inline F sgn(F x) { return x < 0.?-1.F:1.F; }
inline V swap(F &a,F &b) { F c = a; a = b; b = c; }

inline I min(I a,I b) { return a < b?a:b; }
inline I max(I a,I b) { return a > b?a:b; }


class VecBlock 
{
public:

	I Frames() const { return frms; }
	V Frames(I fr) { frms = fr; }
	I ArgFrames() const { return afrms; }
	V ArgFrames(I fr) { afrms = fr; }

	BL Complex() { return cplx; }

protected:
	VecBlock(BL cplx,I msrc,I mdst,I marg,I blarg);
	~VecBlock();

	Vasp *_SrcVasp(I n);
	Vasp *_DstVasp(I n);
	Vasp *_ResVasp(I n); // either Dst or Src

	VBuffer *_Src(I ix) { return vecs[ix]; }
	VBuffer *_Arg(I ix,I bl = 0) { return vecs[asrc+bl*aarg+ix]; }
	VBuffer *_Dst(I ix) { return vecs[asrc+aarg*barg+ix]; }
	V _Src(I ix,VBuffer *v) { vecs[ix] = v; }
	V _Arg(I ix,VBuffer *v,I bl = 0) { vecs[asrc+bl*aarg+ix] = v; }
	V _Dst(I ix,VBuffer *v) { vecs[asrc+aarg*barg+ix] = v; }
	
	I ArgBlks() const { return barg; }

private:
	BL cplx;
	I asrc,adst,aarg,barg;
	VBuffer **vecs;
	I frms,afrms;
};


class RVecBlock:
	public VecBlock
{
public:
	RVecBlock(I _n,I _a = 0,I _ba = 1): VecBlock(false,_n,_n,_a,_ba),n(_n),a(_a) {}

	VBuffer *Src(I ix) { return _Src(ix); }
	VBuffer *Arg(I ix,I bl = 0) { return _Arg(ix,bl); }
	VBuffer *Dst(I ix) { return _Dst(ix); }
	V Src(I ix,VBuffer *v) { _Src(ix,v); }
	V Arg(I ix,VBuffer *v,I bl = 0) { _Arg(ix,v,bl); }
	V Dst(I ix,VBuffer *v) { _Dst(ix,v); }

	I Vecs() const { return n; }
	I Args() const { return a; }

	Vasp *SrcVasp() { return _SrcVasp(n); }
	Vasp *DstVasp() { return _DstVasp(n); }
	Vasp *ResVasp() { return _ResVasp(n); }

protected:
	I n,a;
};

class CVecBlock:
	public VecBlock
{
public:
	CVecBlock(I _np,I _ap = 0,I _bap = 1): VecBlock(true,_np*2,_np*2,_ap*2,_bap),np(_np),ap(_ap) {}

	VBuffer *ReSrc(I ix) { return _Src(ix*2); }
	VBuffer *ImSrc(I ix) { return _Src(ix*2+1); }
	VBuffer *ReArg(I ix,I bl = 0) { return _Arg(ix*2,bl); }
	VBuffer *ImArg(I ix,I bl = 0) { return _Arg(ix*2+1,bl); }
	VBuffer *ReDst(I ix) { return _Dst(ix*2); }
	VBuffer *ImDst(I ix) { return _Dst(ix*2+1); }
	V Src(I ix,VBuffer *vre,VBuffer *vim) { _Src(ix*2,vre); _Src(ix*2+1,vim); }
	V Arg(I ix,VBuffer *vre,VBuffer *vim,I bl = 0) { _Arg(ix*2,vre,bl); _Arg(ix*2+1,vim,bl); }
	V Dst(I ix,VBuffer *vre,VBuffer *vim) { _Dst(ix*2,vre); _Dst(ix*2+1,vim); }

	I Pairs() const { return np; }
	I Args() const { return ap; }

	Vasp *SrcVasp() { return _SrcVasp(np*2); }
	Vasp *DstVasp() { return _DstVasp(np*2); }
	Vasp *ResVasp() { return _ResVasp(np*2); }

protected:
	I np,ap;
};


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
	inline BL AR_In(I bl) const { return arg[bl].v.rdt && rddt > arg[bl].v.rdt && rddt < arg[bl].v.rdt+frames*arg[bl].v.rs; } 
	inline BL AI_In(I bl) const { return arg[bl].v.idt && iddt > arg[bl].v.idt && iddt < arg[bl].v.idt+frames*arg[bl].v.is; } 
	BL AR_In() const;
	BL AI_In() const;
	
	// Can we reverse direction?
	inline BL SR_Can() const { return rsdt <= rddt || rsdt >= rddt+frames*rds; } 
	inline BL SI_Can() const { return isdt <= iddt || isdt >= iddt+frames*ids; } 
	inline BL AR_Can(I bl) const { return !arg[bl].v.rdt || arg[bl].v.rdt <= rddt || arg[bl].v.rdt >= rddt+frames*rds; } 
	inline BL AI_Can(I bl) const { return !arg[bl].v.idt || arg[bl].v.idt <= iddt || arg[bl].v.idt >= iddt+frames*ids; } 
	BL AR_Can() const;
	BL AI_Can() const;
	
	// does it overlap? (works only with rss,rds,ras.... > 0)
	inline BL SR_Ovr() const { return rddt != rsdt && rddt < rsdt+frames*rss && rsdt < rddt+frames*rds; } 
	inline BL SI_Ovr() const { return iddt != isdt && iddt < isdt+frames*iss && isdt < iddt+frames*ids; } 
	inline BL AR_Ovr(I bl) const { return arg[bl].v.rdt && rddt != arg[bl].v.rdt && rddt < arg[bl].v.rdt+frames*arg[bl].v.rs && arg[bl].v.rdt < rddt+frames*rds; } 
	inline BL AI_Ovr(I bl) const { return arg[bl].v.idt && iddt != arg[bl].v.idt && iddt < arg[bl].v.idt+frames*arg[bl].v.is && arg[bl].v.idt < iddt+frames*ids; } 
	BL AR_Ovr() const;
	BL AI_Ovr() const;
	
	// reverse direction
	inline V SR_Rev() { rsdt -= (frames-1)*(rss = -rss); }
	inline V SI_Rev() { isdt -= (frames-1)*(iss = -iss); }
	inline V AR_Rev(I bl) { if(arg[bl].v.rdt) arg[bl].v.rdt -= (frames-1)*(arg[bl].v.rs = -arg[bl].v.rs); }
	inline V AI_Rev(I bl) { if(arg[bl].v.idt) arg[bl].v.idt -= (frames-1)*(arg[bl].v.is = -arg[bl].v.is); }
	inline V DR_Rev() { rddt -= (frames-1)*(rds = -rds); }
	inline V DI_Rev() { iddt -= (frames-1)*(ids = -ids); }
	V AR_Rev();
	V AI_Rev();
	
	V R_Rev();
	V C_Rev();

	const C *opname;
	I frames,symm,args;
	BL ovrlap,revdir;
	S *rsdt,*isdt; I rss,iss;
	S *rddt,*iddt; I rds,ids;
	
	struct arg_t {
		enum { arg_ = 0,arg_x,arg_v,arg_l } argtp;
		union {
			struct { S r,i; } x;
			struct { S *rdt,*idt; I rs,is; } v;
			struct { I pts; S *r,*i; } l;
		};
	} *arg;
	
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
		struct { R fnorm,scl; } norm;
		struct { R arg; } rbin; 
		struct { R rarg,iarg; } cbin; 
	};
};

namespace VecOp {
	typedef BL opfun(OpParam &p);

	BL d__run(V fun(S &v,S a),OpParam &p);
	BL d__cun(V fun(S &rv,S &iv,S ra,S ia),OpParam &p);
	BL d__rbin(V fun(S &v,S a,S b),OpParam &p);
	BL d__cbin(V fun(S &rv,S &iv,S ra,S ia,S rb,S ib),OpParam &p);
	BL d__rop(V fun(S &v,S a,OpParam &p),OpParam &p);
	BL d__cop(V fun(S &rv,S &iv,S ra,S ia,OpParam &p),OpParam &p);

	BL d_copy(OpParam &p); 
	BL d_set(OpParam &p); 
	BL d_add(OpParam &p); 
	BL d_sub(OpParam &p); 
	BL d_mul(OpParam &p); 
	BL d_div(OpParam &p); 
	BL d_divr(OpParam &p); 
	BL d_mod(OpParam &p); 
	BL d_min(OpParam &p); 
	BL d_max(OpParam &p); 
	BL d_pow(OpParam &p); 

	BL d_lwr(OpParam &p); 
	BL d_gtr(OpParam &p); 
	BL d_leq(OpParam &p); 
	BL d_geq(OpParam &p); 
	BL d_equ(OpParam &p); 
	BL d_neq(OpParam &p); 

	BL d_ccopy(OpParam &p); 
	BL d_cset(OpParam &p); 
	BL d_cadd(OpParam &p); 
	BL d_csub(OpParam &p); 
	BL d_cmul(OpParam &p); 
	BL d_cdiv(OpParam &p); 
	BL d_cdivr(OpParam &p); 
	BL d_rmin(OpParam &p); 
	BL d_rmax(OpParam &p); 
	BL d_cpowi(OpParam &p); 
	BL d_rpow(OpParam &p); 
	BL d_radd(OpParam &p); 

	BL d_minmax(OpParam &p); 
	BL d_sqr(OpParam &p); 
	BL d_ssqr(OpParam &p); 
	BL d_sqrt(OpParam &p); 
	BL d_ssqrt(OpParam &p); 
	BL d_exp(OpParam &p); 
	BL d_log(OpParam &p); 
	BL d_inv(OpParam &p); 
	BL d_abs(OpParam &p); 
	BL d_sign(OpParam &p); 
	BL d_optq(OpParam &p); 
	BL d_optf(OpParam &p); 

	BL d_csqr(OpParam &p); 
	BL d_cinv(OpParam &p); 
	BL d_cabs(OpParam &p); 
//	BL d_cswap(OpParam &p); 
	BL d_cconj(OpParam &p); 
	BL d_polar(OpParam &p); 
	BL d_cart(OpParam &p); 
	BL d_roptq(OpParam &p); 
	BL d_roptf(OpParam &p); 
	BL d_cnorm(OpParam &p); 

	BL d_gate(OpParam &p); 
	BL d_rgate(OpParam &p); 

	BL d_int(OpParam &p);
	BL d_dif(OpParam &p); 
	
	BL d_flp(OpParam &p);
	BL d_fhp(OpParam &p);
	
	BL d_tilt(OpParam &p);

	BL d_shift(OpParam &p);
	BL d_rot(OpParam &p);
	BL d_mirr(OpParam &p);
	
	BL d_osc(OpParam &p);
	BL d_cosc(OpParam &p);
	BL d_mosc(OpParam &p);
	BL d_mcosc(OpParam &p);
	BL d_phasor(OpParam &p);
	BL d_mphasor(OpParam &p);
	BL d_noise(OpParam &p);
	BL d_cnoise(OpParam &p);

	BL d_bevel(OpParam &p); 
	BL d_mbevel(OpParam &p); 

	BL d_window(OpParam &p);
	BL d_mwindow(OpParam &p);
	BL d_vwindow(OpParam &p);
	BL d_vmwindow(OpParam &p);
}

namespace VaspOp {
	typedef flext_base flx;

	RVecBlock *GetRVecs(const C *op,Vasp &src,Vasp *dst = NULL);
	CVecBlock *GetCVecs(const C *op,Vasp &src,Vasp *dst = NULL,BL full = false);
	RVecBlock *GetRVecs(const C *op,Vasp &src,const Vasp &arg,Vasp *dst = NULL,I multi = -1,BL ssize = true);
	CVecBlock *GetCVecs(const C *op,Vasp &src,const Vasp &arg,Vasp *dst = NULL,I multi = -1,BL ssize = true,BL full = false);
	
	Vasp *DoOp(RVecBlock *vecs,VecOp::opfun *fun,OpParam &p,BL symm = false);
	Vasp *DoOp(CVecBlock *vecs,VecOp::opfun *fun,OpParam &p,BL symm = false);

	// -------- transformations -----------------------------------

	// unary functions
	Vasp *m_run(OpParam &p,Vasp &src,Vasp *dst,VecOp::opfun *fun); // real unary (one vec or real)
	Vasp *m_cun(OpParam &p,Vasp &src,Vasp *dst,VecOp::opfun *fun); // complex unary (one vec or complex)
	// binary functions
	Vasp *m_rbin(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,VecOp::opfun *fun); // real binary (one vec or real)
	Vasp *m_cbin(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,VecOp::opfun *fun); // complex binary (one vec or complex)

	inline Vasp *m_copy(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_copy); } // copy to (one vec or real)
	inline Vasp *m_set(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_set); } // copy to (one vec or real)
	inline Vasp *m_add(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_add); } // add to (one vec or real)
	inline Vasp *m_sub(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_sub); } // sub from (one vec or real)
	inline Vasp *m_mul(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_mul); } // mul with (one vec or real)
	inline Vasp *m_div(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_div); } // div by (one vec or real)
	inline Vasp *m_divr(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_divr); } // reserve div by (one vec or real)
	inline Vasp *m_mod(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_mod); } // modulo by (one vec or real)
	inline Vasp *m_min(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_min); } // min (one vec or real)
	inline Vasp *m_max(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_max); } // max (one vec or real)
	inline Vasp *m_pow(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_pow); } // power

	inline Vasp *m_lwr(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_lwr); } // lower than
	inline Vasp *m_gtr(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_gtr); } // greater than
	inline Vasp *m_leq(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_leq); } // lower than
	inline Vasp *m_geq(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_geq); } // greater than
	inline Vasp *m_equ(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_equ); } // lower than
	inline Vasp *m_neq(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_rbin(p,src,arg,dst,VecOp::d_neq); } // greater than

	inline Vasp *m_ccopy(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_ccopy); }  // complex copy (pairs of vecs or complex)
	inline Vasp *m_cset(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_cbin(p,src,arg,dst,VecOp::d_cset); }  // complex copy (pairs of vecs or complex)
	inline Vasp *m_cadd(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_cbin(p,src,arg,dst,VecOp::d_cadd); }  // complex add (pairs of vecs or complex)
	inline Vasp *m_csub(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_cbin(p,src,arg,dst,VecOp::d_csub); }  // complex sub (pairs of vecs or complex)
	inline Vasp *m_cmul(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_cbin(p,src,arg,dst,VecOp::d_cmul); }  // complex mul (pairs of vecs or complex)
	inline Vasp *m_cdiv(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_cbin(p,src,arg,dst,VecOp::d_cdiv); }  // complex div (pairs of vecs or complex)
	inline Vasp *m_cdivr(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_cbin(p,src,arg,dst,VecOp::d_cdivr); }  // complex reverse div (pairs of vecs or complex)
	inline Vasp *m_rmin(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_cbin(p,src,arg,dst,VecOp::d_rmin); }  // complex (radius) min (pairs of vecs or complex)
	inline Vasp *m_rmax(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_cbin(p,src,arg,dst,VecOp::d_rmax); }  // complex (radius) max (pairs of vecs or complex)

	Vasp *m_rpow(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL); // radius power (with each two channels)
	Vasp *m_cpowi(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL); // complex integer power (with each two channels)
	Vasp *m_radd(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL); // radius offset
	Vasp *m_gate(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL); // gate
	Vasp *m_rgate(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL); // radius gate
	Vasp *m_optf(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL); // scaling across max
	Vasp *m_roptf(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL); // radius scaling across max

	Vasp *m_qmin(OpParam &p,Vasp &src); // get minimum sample value
	Vasp *m_qmax(OpParam &p,Vasp &src); // get maximum sample value

	inline Vasp *m_minmax(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_minmax); } // min/max 

	inline Vasp *m_sqr(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_sqr); }    // unsigned square 
	inline Vasp *m_ssqr(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_ssqr); }   // signed square 
	inline Vasp *m_sqrt(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_sqrt); }  // square root (from abs value)
	inline Vasp *m_ssqrt(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_ssqrt); }  // square root (from abs value)

	inline Vasp *m_csqr(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_csqr); }  // complex square (with each two channels)
//	inline Vasp *m_csqrt(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_csqrt); }  // complex square root (how about branches?)

	inline Vasp *m_exp(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_exp); }  // exponential function
	inline Vasp *m_log(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_log); } // natural logarithm

//	inline Vasp *m_cexp(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_cexp); }  // complex exponential function
//	inline Vasp *m_clog(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_clog); } // complex logarithm (how about branches?)

	inline Vasp *m_inv(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_inv); }  // invert buffer values
	inline Vasp *m_cinv(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_cinv); } // complex invert buffer values (each two)

	inline Vasp *m_abs(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_abs); }  // absolute values
	inline Vasp *m_cabs(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_cabs); }  // absolute values

	inline Vasp *m_sign(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_run(p,src,dst,VecOp::d_sign); }  // sign function 

	inline Vasp *m_polar(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_polar); } // cartesian -> polar (each two)
	inline Vasp *m_cart(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_cart); } // polar -> cartesian (each two)

	Vasp *m_opt(OpParam &p,Vasp &src,Vasp *dst = NULL);  // optimize
	Vasp *m_ropt(OpParam &p,Vasp &src,Vasp *dst = NULL);  // complex radius optimize

	inline Vasp *m_cnorm(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_cnorm); } // complex normalize

//	inline Vasp *m_cswap(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_cswap); }  // swap real and imaginary parts
	inline Vasp *m_cconj(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cun(p,src,dst,VecOp::d_cconj); }  // complex conjugate

	// int/dif functions
	Vasp *m_int(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL,BL inv = false); //! integrate
	inline Vasp *m_dif(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_int(p,src,arg,dst,true); } //! differentiate
	
	// extrema functions
	Vasp *m_peaks(OpParam &p,Vasp &src,Vasp *dst = NULL,BL inv = false); //! find peaks
	inline Vasp *m_valleys(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_peaks(p,src,dst,true); } //! find valleys	
	Vasp *m_rpeaks(OpParam &p,Vasp &src,Vasp *dst = NULL,BL inv = false); //! find radius peaks
	inline Vasp *m_rvalleys(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_rpeaks(p,src,dst,true); } //! find radius valleys	

	// Filter functions
	Vasp *m_fhp(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL,BL hp = true); //! hi pass
	inline Vasp *m_flp(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_fhp(p,src,arg,dst,false); } //! lo pass
	
	// Rearrange buffer 
	Vasp *m_shift(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL,BL sh = true,BL symm = false);  // shift buffer
	inline Vasp *m_xshift(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_shift(p,src,arg,dst,true,true); }  // shift buffer (symmetrically)
	inline Vasp *m_rot(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL) { return m_shift(p,src,arg,dst,false,false); } // rotate buffer
	inline Vasp *m_xrot(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL)  { return m_shift(p,src,arg,dst,false,true); }  // rotate buffer (symmetrically)
	Vasp *m_mirr(OpParam &p,Vasp &src,Vasp *dst = NULL,BL symm = false);  //! mirror buffer
	inline Vasp *m_xmirr(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_mirr(p,src,dst,true); } //! mirror buffer (symmetrically)

	// Generator functions 
	Vasp *m_osc(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL mul = false);  // real osc
	inline Vasp *m_mosc(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) { return m_osc(p,src,arg,dst,true); }   // * real osc
	Vasp *m_cosc(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL mul = false);  // complex osc (phase rotates)
	inline Vasp *m_mcosc(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) { return m_cosc(p,src,arg,dst,true); }  // * complex osc (phase rotates)
	Vasp *m_phasor(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL mul = false);  // phasor
	inline Vasp *m_mphasor(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) { return m_phasor(p,src,arg,dst,true); }  // * phasor
	Vasp *m_noise(OpParam &p,Vasp &src,Vasp *dst);  // real noise
	Vasp *m_cnoise(OpParam &p,Vasp &src,Vasp *dst); // complex noise (arg and abs random)
	Vasp *m_bevelup(OpParam &p,Vasp &src,Vasp *dst,BL up = true,BL mul = false);  // bevel up 
	inline Vasp *m_mbevelup(OpParam &p,Vasp &src,Vasp *dst) { return m_bevelup(p,src,dst,true,true); }   // * bevel up (fade in)
	inline Vasp *m_beveldn(OpParam &p,Vasp &src,Vasp *dst) { return m_bevelup(p,src,dst,false,false); }  // bevel down
	inline Vasp *m_mbeveldn(OpParam &p,Vasp &src,Vasp *dst) { return m_bevelup(p,src,dst,false,true); }   // * bevel down (fade out)
	Vasp *m_window(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL mul = false);  // window curve
	inline Vasp *m_mwindow(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) { return m_window(p,src,arg,dst,true); }  // * window curve

	// Resampling (around center sample)
	Vasp *m_tilt(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL,BL symm = false,I mode = 0); 
	// Symmetric resampling (around center sample)
	inline Vasp *m_xtilt(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst = NULL,I mode = 0) { return m_tilt(p,src,arg,dst,true,mode); }

	// Fourier transforms 
	Vasp *m_rfft(OpParam &p,Vasp &src,Vasp *dst = NULL,BL inv = false);  // real forward
	inline Vasp *m_rifft(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_rfft(p,src,dst,true); } // real inverse
	Vasp *m_cfft(OpParam &p,Vasp &src,Vasp *dst = NULL,BL inv = false); // complex forward
	inline Vasp *m_cifft(OpParam &p,Vasp &src,Vasp *dst = NULL) { return m_cfft(p,src,dst,true); } // complex inverse

}



class Vasp 
{
public:
	class Ref {
	public:
		Ref(): sym(NULL) {}
		Ref(VBuffer &b): sym(b.Symbol()),chn(b.Channel()),offs(b.Offset()) {}
		Ref(t_symbol *s,I c,I o): sym(s),chn(c),offs(o) {}

		V Clear() { sym = NULL; }
		BL Ok() const { return sym != NULL; }

		t_symbol *Symbol() const { return sym; }
		V Symbol(t_symbol *s) { sym = s; }
		I Channel() const { return chn; }
		V Channel(I c) { chn = c; }
		I Offset() const { return offs; }
		V Offset(I o) { offs = o; }
		V OffsetD(I o) { offs += o; }

	protected:
		t_symbol *sym;
		I chn;
		I offs; // counted in frames
	};

	Vasp();
	Vasp(I argc,t_atom *argv);
	Vasp(const Vasp &v);
	Vasp(I frames,const Ref &r);
	~Vasp();

	const C *thisName() const { return typeid(*this).name(); }

	Vasp &operator =(const Vasp &v);
	Vasp &operator ()(I argc,t_atom *argv /*,BL withvasp = false*/);

	// add another vector
	Vasp &operator +=(const Ref &r);
	// add vectors of another vasp
	Vasp &operator +=(const Vasp &v);

	// set used channels to 0
	Vasp &Clear() { frames = 0; chns = 0; return *this; }

	// used vectors
	I Vectors() const { return chns; }

	// length of the vasp (in frames)
	I Frames() const { return frames; }
	// set frame count
	V Frames(I fr) { frames = fr; }
	// set frame count differentially
	V FramesD(I frd) { frames += frd; }

	// actual length of the vasp (in frames)
	I ChkFrames() const;

	// set offset(s)
	V Offset(I fr);
	// set offset(s) differentially
	V OffsetD(I fr);

	BL Ok() const { return ref && Vectors() > 0; }
	BL IsComplex() const { return ref && Vectors() >= 2 && ref[1].Symbol() != NULL; }

	// get any vector - test if in range 0..Vectors()-1!
	const Ref &Vector(I ix) const { return ref[ix]; }
	Ref &Vector(I ix) { return ref[ix]; }

	// get real part - be sure that Ok!
	const Ref &Real() const { return Vector(0); }
	Ref &Real() { return Vector(0); }

	// get imaginary part - be sure that Complex!
	const Ref &Imag() const { return Vector(1); }
	Ref &Imag() { return Vector(1); }

	// get buffer associated to a channel
	VBuffer *Buffer(I ix) const;

	// Real/Complex
	VBuffer *ReBuffer() const { return Buffer(0); }
	VBuffer *ImBuffer() const { return Buffer(1); }

	// prepare and reference t_atom list for output
	AtomList *MakeList(BL withvasp = true);

	// make a graphical update of all buffers in vasp
	V Refresh();
	
protected:
	I frames; // length counted in frames
	I chns; // used channels
	I refs; // allocated channels (>= chns)
	Ref *ref;

	V Resize(I rcnt);
};


class vasp_base:
	public flext_base
{
	FLEXT_HEADER_S(vasp_base,flext_base,setup)

	enum xs_unit {
		xsu__ = -1,  // don't change
		xsu_sample = 0,xsu_buffer,xsu_ms,xsu_s
	};	

protected:
	vasp_base();
	virtual ~vasp_base();

	virtual V m_radio(I argc,t_atom *argv);  // commands for all

	virtual V m_argchk(BL chk);  // precheck argument on arrival
	virtual V m_loglvl(I lvl);  // noise level of log messages
	virtual V m_unit(xs_unit u);  // unit command

	BL refresh;  // immediate graphics refresh?
	BL argchk;   // pre-operation argument feasibility check
	xs_unit unit;  // time units
	I loglvl;	// noise level for log messages

	friend class Vasp;

	static const t_symbol *sym_vasp;
	static const t_symbol *sym_complex;
	static const t_symbol *sym_vector;
	static const t_symbol *sym_radio;

	BL ToOutVasp(I outlet,Vasp &v);

private:
	static V setup(t_class *);

	FLEXT_CALLBACK_G(m_radio)

	FLEXT_CALLBACK_B(m_argchk)
	FLEXT_CALLBACK_I(m_loglvl)
	FLEXT_CALLBACK_1(m_unit,xs_unit)
};


class vasp_op:
	public vasp_base
{
	FLEXT_HEADER(vasp_op,vasp_base)

protected:
	vasp_op(BL withto = false);

	virtual V m_bang() = 0;						// do! and output current Vasp

	virtual V m_vasp(I argc,t_atom *argv); // trigger
	virtual I m_set(I argc,t_atom *argv);  // non trigger
	virtual V m_to(I argc,t_atom *argv); // set destinatioc

	virtual V m_update(I argc = 0,t_atom *argv = NULL);  // graphics update

	// destination vasp
	Vasp ref,dst;

	FLEXT_CALLBACK_G(m_to)

	FLEXT_CALLBACK(m_bang)
	FLEXT_CALLBACK_G(m_vasp)
	FLEXT_CALLBACK_G(m_set)
	FLEXT_CALLBACK_G(m_update)
};



class vasp_tx:
	public vasp_op
{
	FLEXT_HEADER(vasp_tx,vasp_op)

protected:
	vasp_tx(BL withto = false);

	virtual V m_bang();						// do! and output current Vasp

	virtual Vasp *x_work() = 0;
};




#define VASP_SETUP(op) FLEXT_SETUP(vasp_##op);  



// base class for unary operations

class vasp_unop:
	public vasp_tx
{
	FLEXT_HEADER(vasp_unop,vasp_tx)

protected:
	vasp_unop(BL withto = false,I addouts = 0);

	virtual Vasp *x_work();
	virtual Vasp *tx_work();
};


// base class for binary operations

class vasp_binop:
	public vasp_tx
{
	FLEXT_HEADER(vasp_binop,vasp_tx)

protected:
	vasp_binop(I argc,t_atom *argv,BL withto = false,I addouts = 0);

	// assignment functions
	virtual V a_vasp(I argc,t_atom *argv);
	virtual V a_list(I argc,t_atom *argv); 
	virtual V a_float(F f); 
	virtual V a_complex(I argc,t_atom *argv); 
	virtual V a_vector(I argc,t_atom *argv); 

	virtual Vasp *x_work();
	virtual Vasp *tx_work(const Argument &arg);

	Argument arg;

private:
	FLEXT_CALLBACK_G(a_vasp)
	FLEXT_CALLBACK_G(a_list)
	FLEXT_CALLBACK_1(a_float,F)
	FLEXT_CALLBACK_G(a_complex)
	FLEXT_CALLBACK_G(a_vector)
};


// base class for non-parsed (list) arguments

class vasp_anyop:
	public vasp_tx
{
	FLEXT_HEADER(vasp_anyop,vasp_tx)

protected:
	vasp_anyop(I argc,t_atom *argv,BL withto = false,I addouts = 0);

	// assignment functions
	virtual V a_list(I argc,t_atom *argv); 

	virtual Vasp *x_work();
	virtual Vasp *tx_work(const Argument &arg);

	Argument arg;

private:
	FLEXT_CALLBACK_G(a_list)
};



#define VASP_UNARY(name,op,to)													\
class vasp__##op:																\
	public vasp_unop															\
{																				\
	FLEXT_HEADER(vasp__##op,vasp_unop)											\
public:																			\
	vasp__##op(): vasp_unop(to) {}												\
protected:																		\
	virtual Vasp *tx_work()														\
	{																			\
		OpParam p(thisName(),0);												\
		return VaspOp::m_##op(p,ref,&dst);										\
	}																			\
};																				\
FLEXT_LIB(name,vasp__##op)


#define VASP_BINARY(name,op,to)													\
class vasp__ ## op:																\
	public vasp_binop															\
{																				\
	FLEXT_HEADER(vasp__##op,vasp_binop)											\
public:																			\
	vasp__##op(I argc,t_atom *argv): vasp_binop(argc,argv,to) {}				\
protected:																		\
	virtual Vasp *tx_work(const Argument &arg)									\
	{																			\
		OpParam p(thisName(),1);												\
		return VaspOp::m_##op(p,ref,arg,&dst);									\
	}																			\
};																				\
FLEXT_LIB_G(name,vasp__##op)


#define VASP_ANYOP(name,op,args,to)												\
class vasp__ ## op:																\
	public vasp_anyop															\
{																				\
	FLEXT_HEADER(vasp__##op,vasp_anyop)											\
public:																			\
	vasp__##op(I argc,t_atom *argv): vasp_anyop(argc,argv,to) {}				\
protected:																		\
	virtual Vasp *tx_work(const Argument &arg)									\
	{																			\
		OpParam p(thisName(),args);												\
		return VaspOp::m_##op(p,ref,arg,&dst); }								\
};																				\
FLEXT_LIB_G(name,vasp__##op)


#define VASP__SETUP(op) FLEXT_SETUP(vasp__##op);  


#endif
