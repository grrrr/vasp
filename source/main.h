/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_H
#define __VASP_H

#define VASP_VERSION "0.0.1"


#include <flext.h>

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 202)
#error You need at least flext version 0.2.2
#endif

#include <typeinfo.h>

#define I int
#define L long
#define F float
#define D double
#define C char
#define BL bool
#define V void
#define CX complex
#define VX vector

class complex 
{ 
public:
	complex() {}
	complex(F re,F im = 0): real(re),imag(im) {}

	F real,imag; 
};

struct vector { I dim; F *data; };


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

protected:
	I chn,offs,len;
};


class AtomList
{
public:
	AtomList(I argc,t_atom *argv);
	~AtomList();

	I Count() const { return cnt; }
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
	
	V Parse(I argc,t_atom *argv);
	V Clear();

	V Set(Vasp *v);
//	V Set(I argc,t_atom *argv);
	V Set(F f);
	V Set(F re,F im);
	V Set(VX *vec);

	BL IsNone() const { return tp == tp_none; }
	BL IsVasp() const { return tp == tp_vasp; }
//	BL IsList() const { return tp == tp_list; }
	BL IsFloat() const { return tp == tp_float; }
	BL IsComplex() const { return tp == tp_cx; }
	BL IsVector() const { return tp == tp_vx; }

	const Vasp &GetVasp() const { return *dt.v; }
//	const AtomList &GetList() const { return *dt.atoms; }
	F GetFloat() const { return dt.f; }
	const CX &GetComplex() const { return *dt.cx; }
	const VX &GetVector() const { return *dt.vx; }

protected:
	enum {
		tp_none,tp_vasp,tp_list,tp_float,tp_cx,tp_vx
	} tp;

	union {
		Vasp *v;
//		AtomList *atoms;
		F f;
		CX *cx;
		VX *vx;
	} dt;
};



class Vasp 
{
public:
	struct Ref {
		t_symbol *Symbol() const { return sym; }
		I Channel() const { return chn; }
		I Offset() const { return offs; }

		t_symbol *sym;
		I chn;
		I offs; // counted in frames
	};

	Vasp();
	Vasp(I argc,t_atom *argv);
	Vasp(const Vasp &v);
	~Vasp();

	const C *thisName() const { return typeid(*this).name(); }

	Vasp &operator =(const Vasp &v);
	Vasp &operator ()(I argc,t_atom *argv /*,BL withvasp = false*/);

	// set used channels to 0
	Vasp &Clear() { frames = 0; chns = 0; return *this; }

	// used vectors
	I Vectors() const { return chns; }

	// length of the vasp (in frames)
	I Frames() const { return frames; }

	BL Ok() const { return ref && Vectors() > 0; }
	BL Complex() const { return ref && Vectors() >= 2 && ref[1].sym != NULL; }

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
	Vasp &MakeList(BL withvasp = true);
	I Atoms() const { return atoms; }
	t_atom *AtomList() { return atomlist; }


	// copy functions
	Vasp *m_copy(const Argument &arg); // copy to (one vec or real)
	Vasp *m_ccopy(const Argument &arg); // complex copy (pairs of vecs or complex)
	Vasp *m_mcopy(const Argument &arg); // copy to (multi-channel)
/*
	Vasp *m_copy(I argc,t_atom *argv); // copy to (one vec or real)
	Vasp *m_ccopy(I argc,t_atom *argv); // complex copy (pairs of vecs or complex)
	Vasp *m_mcopy(I argc,t_atom *argv); // copy to (multi-channel)
*/
//	Vasp *m_mix(I argc,t_atom *argv); // mix in (one vec or real/complex)
//	Vasp *m_mmix(I argc,t_atom *argv); // mix in (multi-channel)

	// binary functions
	Vasp *m_add(const Argument &arg); // add to (one vec or real)
	Vasp *m_cadd(const Argument &arg); // complex add (pairs of vecs or complex)
	Vasp *m_madd(const Argument &arg); // add to (multi-channel)

	Vasp *m_sub(const Argument &arg); // sub from (one vec or real/complex)
	Vasp *m_csub(const Argument &arg); // complex sub (pairs of vecs or complex)
	Vasp *m_msub(const Argument &arg); // sub from (multi-channel)

	Vasp *m_mul(const Argument &arg); // mul with (one vec or real/complex)
	Vasp *m_cmul(const Argument &arg); // complex mul (pairs of vecs or complex)
	Vasp *m_mmul(const Argument &arg); // mul with (multi-channel)

	Vasp *m_div(const Argument &arg); // div by (one vec or real/complex)
	Vasp *m_cdiv(const Argument &arg); // complex div (pairs of vecs or complex)
	Vasp *m_mdiv(const Argument &arg); // div by (multi-channel)

	Vasp *m_min(const Argument &arg); // min (one vec or real)
//	Vasp *m_cmin(const Argument &arg); // complex (abs) min (pairs of vecs or complex)
	Vasp *m_mmin(const Argument &arg); // min (multi-channel)

	Vasp *m_max(const Argument &arg); // max (one vec or real)
//	Vasp *m_cmax(const Argument &arg); // complex (abs) max (pairs of vecs or complex)
	Vasp *m_mmax(const Argument &arg); // max (multi-channel)

	// "unary" functions
	Vasp *m_pow(F v); // power
//	Vasp *m_cpow(I argc,t_atom *argv); // complex power (with each two channels)
	Vasp *m_sqr();   // unsigned square 
	Vasp *m_ssqr();   // signed square 
	Vasp *m_csqr();  // complex square (with each two channels)
	Vasp *m_root(F v); // real root (from abs value)
	Vasp *m_sqrt();  // square root (from abs value)
	Vasp *m_ssqrt();  // square root (from abs value)
//	Vasp *m_csqrt();  // complex square root (how about branches?)

	Vasp *m_exp();  // exponential function
//	Vasp *m_cexp();  // complex exponential function
	Vasp *m_log(); // natural logarithm
//	Vasp *m_clog(); // complex logarithm (how about branches?)

	Vasp *m_inv();  // invert buffer values
	Vasp *m_cinv(); // complex invert buffer values (each two)

	Vasp *m_abs();  // absolute values
	Vasp *m_sign();  // sign function 
	Vasp *m_polar(); // cartesian -> polar (each two)
	Vasp *m_cart(); // polar -> cartesian (each two)

	Vasp *m_norm();  // normalize
	Vasp *m_cnorm(); // complex normalize

	Vasp *m_cswap();  // swap real and imaginary parts
	Vasp *m_cconj();  // complex conjugate

	// Rearrange buffer - separate object?
	Vasp *m_shift(F u);  // shift buffer
	Vasp *m_xshift(F u);  // shift buffer (symmetrically)
	Vasp *m_rot(F u);  // rotate buffer
	Vasp *m_xrot(F u);  // rotate buffer (symmetrically)
	Vasp *m_mirr();  // mirror buffer
	Vasp *m_xmirr();  // mirror buffer (symmetrically)
/*
	// Generator functions - separate object!
	Vasp *m_osc(I argc,t_atom *argv);  // real osc
	Vasp *m_cosc(I argc,t_atom *argv);  // complex osc (phase rotates)
	Vasp *m_noise(I argc,t_atom *argv);  // real noise
	Vasp *m_cnoise(I argc,t_atom *argv); // complex noise (arg and abs random)

	// Fourier transforms - separate object!
	Vasp *m_rfft();
	Vasp *m_rifft();
	Vasp *m_cfft();
	Vasp *m_cifft();
*/

	struct nop_funcs {
		V (*funR)(F *,F,I);
		V (*funC)(F *,F *,F,F,I);
	};

	struct arg_funcs {
		V (*funR)(F *,F,I);
		V (*funC)(F *,F *,F,F,I);
		V (*funV)(F *,const F *,I);
		V (*funCV)(F *,F *,const F *,const F *,I);
	};

protected:
	I frames; // length counted in frames
	I chns; // used channels
	I refs; // allocated channels
	Ref *ref;

	I atoms; // elements of list
	t_atom *atomlist;

	static I min(I a,I b) { return a < b?a:b; }
	static I max(I a,I b) { return a > b?a:b; }
/*
	static BL IsFloat(I argc,t_atom *argv) { return argc == 1 && flx::IsFloat(argv[0]); }
	static BL IsComplex(I argc,t_atom *argv) { return argc == 2 && flx::IsFloat(argv[0]) && flx::IsFloat(argv[1]); }
	static BL IsVector(I argc,t_atom *argv) { return argc >= 3 && flx::IsFloat(argv[0]) && flx::IsFloat(argv[1]) && flx::IsFloat(argv[3]) ; }
*/
private:
	typedef flext_base flx;

//	Vasp *f_nop(const C *op,I argc,t_atom *argv,const nop_funcs &f);
	Vasp *fr_nop(const C *op,F v,V (*f)(F *,F,I));
	Vasp *fr_nop(const C *op,F v,const nop_funcs &f) { return fr_nop(op,v,f.funR); }
	Vasp *fc_nop(const C *op,const CX &cx,V (*f)(F *,F *,F,F,I));
	Vasp *fc_nop(const C *op,const CX &cx,const nop_funcs &f) { return fc_nop(op,cx,f.funC); }
//	Vasp *fc_nop(const C *op,I argc,t_atom *argv,const nop_funcs &f);
	Vasp *fc_nop(const C *op,const Argument &arg,const nop_funcs &f);

//	Vasp *f_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f);
	Vasp *fr_arg(const C *op,F v,V (*f)(F *,F,I));
	Vasp *fr_arg(const C *op,F v,const arg_funcs &f) { return fr_arg(op,v,f.funR); }
	Vasp *fr_arg(const C *op,const Vasp &v,V (*f)(F *,const F *,I));
	Vasp *fr_arg(const C *op,const Vasp &v,const arg_funcs &f) { return fr_arg(op,v,f.funV); }
	Vasp *fr_arg(const C *op,const Argument &arg,const arg_funcs &f);
//	Vasp *fr_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f);
	Vasp *fc_arg(const C *op,const CX &cx,V (*f)(F *,F *,F,F,I));
	Vasp *fc_arg(const C *op,const CX &cx,const arg_funcs &f) { return fc_arg(op,cx,f.funC); }
	Vasp *fc_arg(const C *op,const Vasp &v,V (*f)(F *,F *,const F *,const F *,I));
	Vasp *fc_arg(const C *op,const Vasp &v,const arg_funcs &f) { return fc_arg(op,v,f.funCV); }
	Vasp *fc_arg(const C *op,const Argument &arg,const arg_funcs &f);
//	Vasp *fc_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f);
	Vasp *fm_arg(const C *op,const Vasp &v,V (*f)(F *,const F *,I));
	Vasp *fm_arg(const C *op,const Vasp &v,const arg_funcs &f) { return fm_arg(op,v,f.funV); }
	Vasp *fm_arg(const C *op,const Argument &arg,const arg_funcs &f);
//	Vasp *fm_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f);
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

	virtual V m_bang();						// output current Vasp

	virtual V m_vasp(I argc,t_atom *argv); // trigger
	virtual I m_set(I argc,t_atom *argv);  // non trigger

	virtual V m_radio(I argc,t_atom *argv);  // commands for all

	virtual V m_argchk(BL chk);  // precheck argument on arrival
	virtual V m_update(I argc = 0,t_atom *argv = NULL);  // graphics update
	virtual V m_unit(xs_unit u);  // unit command

	// destination vasp
	Vasp ref;

	virtual Vasp *x_work() = 0;
	
	// immediate graphics refresh?
	BL refresh;


	friend class Vasp;

	static const t_symbol *sym_vasp;
	static const t_symbol *sym_complex;
	static const t_symbol *sym_vector;
	static const t_symbol *sym_radio;

private:
	static V setup(t_class *);

	xs_unit unit;
	BL argchk,log;

	FLEXT_CALLBACK(m_bang)
	FLEXT_CALLBACK_G(m_vasp)
	FLEXT_CALLBACK_G(m_set)

	FLEXT_CALLBACK_G(m_radio)

	FLEXT_CALLBACK_G(m_update)
	FLEXT_CALLBACK_B(m_argchk)
	FLEXT_CALLBACK_1(m_unit,xs_unit)
};


class vasp_tx:
	public vasp_base
{
	FLEXT_HEADER(vasp_tx,vasp_base)

public:
	vasp_tx(I argc,t_atom *argv);
	~vasp_tx();

	// assignment functions
	virtual V a_vasp(I argc,t_atom *argv);
	virtual V a_list(I argc,t_atom *argv); 
	virtual V a_float(F f); 
	virtual V a_complex(I argc,t_atom *argv); 
	virtual V a_vector(I argc,t_atom *argv); 

	FLEXT_CALLBACK_G(a_vasp)
	FLEXT_CALLBACK_G(a_list)
	FLEXT_CALLBACK_1(a_float,F)
	FLEXT_CALLBACK_G(a_complex)
	FLEXT_CALLBACK_G(a_vector)

protected:
	virtual Vasp *x_work();

	virtual Vasp *tx_work(const Argument &arg);

	Argument arg;
};


#if 0

class vasp_tx:
	public vasp_base
{
	FLEXT_HEADER(vasp_tx,vasp_base)

public:
	vasp_tx(I argc,t_atom *argv);
	~vasp_tx();

	virtual I m_set(I argc,t_atom *argv); // set destination (non-triggering)

	// assignment functions
	virtual V m_copy(I argc,t_atom *argv); // copy to (one vec or real)
	virtual V m_ccopy(I argc,t_atom *argv); // complex copy (pairs of vecs or complex)
	virtual V m_mcopy(I argc,t_atom *argv); // copy to (multi-channel)
//	virtual V m_mix(I argc,t_atom *argv); // mix in (one vec or real/complex)
//	virtual V m_mmix(I argc,t_atom *argv); // mix in (multi-channel)

	// binary functions
	virtual V m_add(I argc,t_atom *argv); // add to (one vec or real)
	virtual V m_cadd(I argc,t_atom *argv); // complex add (pairs of vecs or complex)
	virtual V m_madd(I argc,t_atom *argv); // add to (multi-channel)
	virtual V m_sub(I argc,t_atom *argv); // sub from (one vec or real/complex)
	virtual V m_csub(I argc,t_atom *argv); // complex sub (pairs of vecs or complex)
	virtual V m_msub(I argc,t_atom *argv); // sub from (multi-channel)
	virtual V m_mul(I argc,t_atom *argv); // mul with (one vec or real/complex)
	virtual V m_cmul(I argc,t_atom *argv); // complex mul (pairs of vecs or complex)
	virtual V m_mmul(I argc,t_atom *argv); // mul with (multi-channel)
	virtual V m_div(I argc,t_atom *argv); // div by (one vec or real/complex)
	virtual V m_cdiv(I argc,t_atom *argv); // complex div (pairs of vecs or complex)
	virtual V m_mdiv(I argc,t_atom *argv); // div by (multi-channel)

	virtual V m_min(I argc,t_atom *argv); // min (one vec or real)
//	virtual V m_cmin(I argc,t_atom *argv); // complex (abs) min (pairs of vecs or complex)
	virtual V m_mmin(I argc,t_atom *argv); // min (multi-channel)
	virtual V m_max(I argc,t_atom *argv); // max (one vec or real)
//	virtual V m_cmax(I argc,t_atom *argv); // complex (abs) max (pairs of vecs or complex)
	virtual V m_mmax(I argc,t_atom *argv); // max (multi-channel)

	// "unary" functions
	virtual V m_pow(F v); // power
//	virtual V m_cpow(I argc,t_atom *argv); // complex power (with each two channels)
	virtual V m_sqr();   // unsigned square 
	virtual V m_ssqr();   // signed square 
	virtual V m_csqr();  // complex square (with each two channels)
	virtual V m_root(F v); // real root (from abs value)
	virtual V m_sqrt();  // square root (from abs value)
	virtual V m_ssqrt();  // square root (from abs value)
//	virtual V m_csqrt();  // complex square root (how about branches?)

	virtual V m_exp();  // exponential function
//	virtual V m_cexp();  // complex exponential function
	virtual V m_log(); // natural logarithm
//	virtual V m_clog(); // complex logarithm (how about branches?)

	virtual V m_inv();  // invert buffer values
	virtual V m_cinv(); // complex invert buffer values (each two)

	virtual V m_abs();  // absolute values
	virtual V m_sign();  // sign function 
	virtual V m_polar(); // cartesian -> polar (each two)
	virtual V m_cart(); // polar -> cartesian (each two)

	virtual V m_norm();  // normalize
	virtual V m_cnorm(); // complex normalize


	virtual V m_cswap();  // swap real and imaginary parts
	virtual V m_cconj();  // complex conjugate

	// Rearrange buffer - separate object?
	virtual V m_shift(F u);  // shift buffer
	virtual V m_xshift(F u);  // shift buffer (symmetrically)
	virtual V m_rot(F u);  // rotate buffer
	virtual V m_xrot(F u);  // rotate buffer (symmetrically)
	virtual V m_mirr();  // mirror buffer
	virtual V m_xmirr();  // mirror buffer (symmetrically)
/*
	// Generator functions - separate object!
	virtual V m_osc(I argc,t_atom *argv);  // real osc
	virtual V m_cosc(I argc,t_atom *argv);  // complex osc (phase rotates)
	virtual V m_noise(I argc,t_atom *argv);  // real noise
	virtual V m_cnoise(I argc,t_atom *argv); // complex noise (arg and abs random)

	// Fourier transforms - separate object!
	virtual V m_rfft();
	virtual V m_rifft();
	virtual V m_cfft();
	virtual V m_cifft();
*/

private:
	V fr_transf(const C *op,F v,V (*dofunR)(F *,F,I));
	V fc_transf(const C *op,I argc,t_atom *argv,V (*dofunC)(F *,F *,F,F,I));
	V fr_assign(const C *op,I argc,t_atom *argv,V (*dofunV)(F *,const F *,I),V (*dofunR)(F *,F,I));
	V fc_assign(const C *op,I argc,t_atom *argv,V (*dofunCV)(F *,F *,const F *,const F *,I),V (*dofunC)(F *,F *,F,F,I));
	V fm_assign(const C *op,I argc,t_atom *argv,V (*dofunV)(F *,const F *,I));


	FLEXT_CALLBACK_G(m_copy)
	FLEXT_CALLBACK_G(m_ccopy)
	FLEXT_CALLBACK_G(m_mcopy)
//	FLEXT_CALLBACK_G(m_mix)
//	FLEXT_CALLBACK_G(m_mmix)

	FLEXT_CALLBACK_G(m_add)
	FLEXT_CALLBACK_G(m_cadd)
	FLEXT_CALLBACK_G(m_madd)
	FLEXT_CALLBACK_G(m_sub)
	FLEXT_CALLBACK_G(m_csub)
	FLEXT_CALLBACK_G(m_msub)
	FLEXT_CALLBACK_G(m_mul)
	FLEXT_CALLBACK_G(m_cmul)
	FLEXT_CALLBACK_G(m_mmul)
	FLEXT_CALLBACK_G(m_div)
	FLEXT_CALLBACK_G(m_cdiv)
	FLEXT_CALLBACK_G(m_mdiv)

	FLEXT_CALLBACK_G(m_min)
//	FLEXT_CALLBACK_G(m_cmin)
	FLEXT_CALLBACK_G(m_mmin)
	FLEXT_CALLBACK_G(m_max)
//	FLEXT_CALLBACK_G(m_cmax)
	FLEXT_CALLBACK_G(m_mmax)

	FLEXT_CALLBACK_1(m_pow,F)
//	FLEXT_CALLBACK_G(m_cpow)
	FLEXT_CALLBACK(m_sqr)
	FLEXT_CALLBACK(m_csqr)
	FLEXT_CALLBACK_1(m_root,F)
	FLEXT_CALLBACK(m_sqrt)

	FLEXT_CALLBACK(m_exp)
//	FLEXT_CALLBACK(m_cexp)
	FLEXT_CALLBACK(m_log)

	FLEXT_CALLBACK(m_inv)
	FLEXT_CALLBACK(m_cinv)

	FLEXT_CALLBACK(m_abs)
	FLEXT_CALLBACK(m_sign)
	FLEXT_CALLBACK(m_polar)
	FLEXT_CALLBACK(m_cart)

	FLEXT_CALLBACK(m_norm)
	FLEXT_CALLBACK(m_cnorm)

	FLEXT_CALLBACK(m_cswap)
	FLEXT_CALLBACK(m_cconj)


	FLEXT_CALLBACK_1(m_shift,F)
	FLEXT_CALLBACK_1(m_xshift,F)
	FLEXT_CALLBACK_1(m_rot,F)
	FLEXT_CALLBACK_1(m_xrot,F)
	FLEXT_CALLBACK(m_mirr)
	FLEXT_CALLBACK(m_xmirr)
/*
	FLEXT_CALLBACK_G(m_osc)
	FLEXT_CALLBACK_G(m_cosc)
	FLEXT_CALLBACK_G(m_noise)
	FLEXT_CALLBACK_G(m_cnoise)

	FLEXT_CALLBACK(m_rfft)
	FLEXT_CALLBACK(m_rifft)
	FLEXT_CALLBACK(m_cfft)
	FLEXT_CALLBACK(m_cifft)
*/
};


class vasp_tx_copy:
	public flext_base
{
	FLEXT_HEADER(vasp_tx_copy,flext_base)

public:
	vasp_tx_copy(I argc,t_atom *argv);
	~vasp_tx_copy();

	// assignment functions
	virtual V m_copy(I argc,t_atom *argv); // copy to (one vec or real)

protected:

	FLEXT_CALLBACK_G(m_copy)
};

#endif

#endif
