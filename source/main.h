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

struct complex { F real,imag; };
struct vector { I dim; F *data; };


class vbuffer:
	public flext_base::buffer
{
	typedef flext_base::buffer parent;

public:
	vbuffer(t_symbol *s = NULL,I chn = 0,I len = -1,I offs = 0);
	vbuffer(const vbuffer &v);
	~vbuffer();

	vbuffer &operator =(const vbuffer &v);
	vbuffer &Set(t_symbol *s = NULL,I chn = 0,I len = -1,I offs = 0);

	I Channel() const { return chn; }
	I Offset() const { return offs; }
	I Length() const { return len; }

protected:
	I chn,offs,len;
};




class vasp 
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

	vasp();
	vasp(I argc,t_atom *argv);
	vasp(const vasp &v);
	~vasp();

	const C *thisName() const { return typeid(*this).name(); }

	vasp &operator =(const vasp &v);
	vasp &operator ()(I argc,t_atom *argv /*,BL withvasp = false*/);

	// set used channels to 0
	vasp &Clear() { frames = 0; chns = 0; return *this; }

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
	vbuffer *Buffer(I ix) const;

	// Real/Complex
	vbuffer *ReBuffer() const { return Buffer(0); }
	vbuffer *ImBuffer() const { return Buffer(1); }

	// prepare and reference t_atom list for output
	vasp &MakeList(BL withvasp = true);
	I Atoms() const { return atoms; }
	t_atom *AtomList() { return atomlist; }


	// copy functions
	V m_copy(I argc,t_atom *argv); // copy to (one vec or real)
	V m_ccopy(I argc,t_atom *argv); // complex copy (pairs of vecs or complex)
	V m_mcopy(I argc,t_atom *argv); // copy to (multi-channel)
//	V m_mix(I argc,t_atom *argv); // mix in (one vec or real/complex)
//	V m_mmix(I argc,t_atom *argv); // mix in (multi-channel)

	// binary functions
	V m_add(I argc,t_atom *argv); // add to (one vec or real)
	V m_cadd(I argc,t_atom *argv); // complex add (pairs of vecs or complex)
	V m_madd(I argc,t_atom *argv); // add to (multi-channel)
	V m_sub(I argc,t_atom *argv); // sub from (one vec or real/complex)
	V m_csub(I argc,t_atom *argv); // complex sub (pairs of vecs or complex)
	V m_msub(I argc,t_atom *argv); // sub from (multi-channel)
	V m_mul(I argc,t_atom *argv); // mul with (one vec or real/complex)
	V m_cmul(I argc,t_atom *argv); // complex mul (pairs of vecs or complex)
	V m_mmul(I argc,t_atom *argv); // mul with (multi-channel)
	V m_div(I argc,t_atom *argv); // div by (one vec or real/complex)
	V m_cdiv(I argc,t_atom *argv); // complex div (pairs of vecs or complex)
	V m_mdiv(I argc,t_atom *argv); // div by (multi-channel)

	V m_min(I argc,t_atom *argv); // min (one vec or real)
//	V m_cmin(I argc,t_atom *argv); // complex (abs) min (pairs of vecs or complex)
	V m_mmin(I argc,t_atom *argv); // min (multi-channel)
	V m_max(I argc,t_atom *argv); // max (one vec or real)
//	V m_cmax(I argc,t_atom *argv); // complex (abs) max (pairs of vecs or complex)
	V m_mmax(I argc,t_atom *argv); // max (multi-channel)

	// "unary" functions
	V m_pow(F v); // power
//	V m_cpow(I argc,t_atom *argv); // complex power (with each two channels)
	V m_sqr();   // unsigned square 
	V m_ssqr();   // signed square 
	V m_csqr();  // complex square (with each two channels)
	V m_root(F v); // real root (from abs value)
	V m_sqrt();  // square root (from abs value)
	V m_ssqrt();  // square root (from abs value)
//	V m_csqrt();  // complex square root (how about branches?)

	V m_exp();  // exponential function
//	V m_cexp();  // complex exponential function
	V m_log(); // natural logarithm
//	V m_clog(); // complex logarithm (how about branches?)

	V m_inv();  // invert buffer values
	V m_cinv(); // complex invert buffer values (each two)

	V m_abs();  // absolute values
	V m_sign();  // sign function 
	V m_polar(); // cartesian -> polar (each two)
	V m_cart(); // polar -> cartesian (each two)

	V m_norm();  // normalize
	V m_cnorm(); // complex normalize

	V m_cswap();  // swap real and imaginary parts
	V m_cconj();  // complex conjugate

	// Rearrange buffer - separate object?
	V m_shift(F u);  // shift buffer
	V m_xshift(F u);  // shift buffer (symmetrically)
	V m_rot(F u);  // rotate buffer
	V m_xrot(F u);  // rotate buffer (symmetrically)
	V m_mirr();  // mirror buffer
	V m_xmirr();  // mirror buffer (symmetrically)
/*
	// Generator functions - separate object!
	V m_osc(I argc,t_atom *argv);  // real osc
	V m_cosc(I argc,t_atom *argv);  // complex osc (phase rotates)
	V m_noise(I argc,t_atom *argv);  // real noise
	V m_cnoise(I argc,t_atom *argv); // complex noise (arg and abs random)

	// Fourier transforms - separate object!
	V m_rfft();
	V m_rifft();
	V m_cfft();
	V m_cifft();
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

	static BL IsFloat(I argc,t_atom *argv) { return argc == 1 && flx::IsFloat(argv[0]); }
	static BL IsComplex(I argc,t_atom *argv) { return argc == 2 && flx::IsFloat(argv[0]) && flx::IsFloat(argv[1]); }
	static BL IsVector(I argc,t_atom *argv) { return argc >= 3 && flx::IsFloat(argv[0]) && flx::IsFloat(argv[1]) && flx::IsFloat(argv[3]) ; }

private:
	typedef flext_base flx;

/*
	V fr_nop(const C *op,F v,V (*dofunR)(F *,F,I));
	V fc_nop(const C *op,F vr,F vi,V (*dofunC)(F *,F *,F,F,I));
	V fc_nop(const C *op,I argc,t_atom *argv,V (*dofunC)(F *,F *,F,F,I));
	V fr_arg(const C *op,I argc,t_atom *argv,V (*dofunV)(F *,const F *,I),V (*dofunR)(F *,F,I));
	V fc_arg(const C *op,F vr,F vi,V (*dofunC)(F *,F *,F,F,I));
	V fc_arg(const C *op,I argc,t_atom *argv,V (*dofunCV)(F *,F *,const F *,const F *,I),V (*dofunC)(F *,F *,F,F,I));
	V fm_arg(const C *op,I argc,t_atom *argv,V (*dofunV)(F *,const F *,I));
*/
//	V f_nop(const C *op,I argc,t_atom *argv,const nop_funcs &f);
	V fr_nop(const C *op,F v,V (*f)(F *,F,I));
	V fr_nop(const C *op,F v,const nop_funcs &f) { fr_nop(op,v,f.funR); }
	V fc_nop(const C *op,F vr,F vi,V (*f)(F *,F *,F,F,I));
	V fc_nop(const C *op,F vr,F vi,const nop_funcs &f) { fc_nop(op,vr,vi,f.funC); }
	V fc_nop(const C *op,I argc,t_atom *argv,const nop_funcs &f);

//	V f_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f);
	V fr_arg(const C *op,F v,V (*f)(F *,F,I));
	V fr_arg(const C *op,F v,const arg_funcs &f) { fr_arg(op,v,f.funR); }
	V fr_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f);
	V fc_arg(const C *op,F vr,F vi,V (*f)(F *,F *,F,F,I));
	V fc_arg(const C *op,F vr,F vi,const arg_funcs &f) { fc_arg(op,vr,vi,f.funC); }
	V fc_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f);
	V fm_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f);
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

	virtual V m_bang();						// output current vasp

	virtual V m_vasp(I argc,t_atom *argv); // trigger
	virtual I m_set(I argc,t_atom *argv);  // non trigger

	virtual V m_radio(I argc,t_atom *argv);  // commands for all

	virtual V m_update(I argc = 0,t_atom *argv = NULL);  // graphics update
	virtual V m_unit(xs_unit u);  // unit command

	// destination vasp
	vasp ref;

	virtual V x_work() = 0;
	
	// immediate graphics refresh?
	BL refresh;


	friend class vasp;

	static const t_symbol *sym_vasp;
	static const t_symbol *sym_complex;
	static const t_symbol *sym_vector;
	static const t_symbol *sym_radio;

private:
	static V setup(t_class *);

	xs_unit unit;
	BL log;

	FLEXT_CALLBACK(m_bang)
	FLEXT_CALLBACK_G(m_vasp)
	FLEXT_CALLBACK_G(m_set)

	FLEXT_CALLBACK_G(m_radio)

	FLEXT_CALLBACK_G(m_update)
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
	virtual V x_work();

	const t_symbol *at_hdr;
	I at_cnt;
	t_atom *at_lst;
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
