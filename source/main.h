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

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 203)
#error You need at least flext version 0.2.3
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
	Argument &Set(F re,F im);
	Argument &Set(VX *vec);

	Argument *Next() { return nxt; }
	Argument &Next(I i);
	Argument &Add(Argument *a);

	Argument &Add(Vasp *v);
	Argument &Add(I argc,t_atom *argv);
	Argument &Add(I i);
	Argument &Add(F f);
	Argument &Add(F re,F im);
	Argument &Add(VX *vec);

	BL IsNone() const { return tp == tp_none; }
	BL IsVasp() const { return tp == tp_vasp; }
	BL IsList() const { return tp == tp_list; }
	BL IsInt() const { return tp == tp_int; }
	BL CanbeInt() const { return tp == tp_int || tp == tp_float; }
	BL IsFloat() const { return tp == tp_float; }
	BL CanbeFloat() const { return tp == tp_float || tp == tp_int; }
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
	const CX &GetComplex() const { return *dt.cx; }
	CX GetAComplex() const;
	const VX &GetVector() const { return *dt.vx; }
	VX GetAVector() const;

protected:
	enum {
		tp_none,tp_vasp,tp_list,tp_int,tp_float,tp_cx,tp_vx
	} tp;

	union {
		Vasp *v;
		AtomList *atoms;
		F f;
		I i;
		CX *cx;
		VX *vx;
	} dt;

	Argument *nxt;
};


F arg(F re,F im);
inline F arg(const CX &c) { return arg(c.real,c.imag); }
inline F abs(F re,F im) { return re*re+im*im; }
inline F abs(const CX &c) { return abs(c.real,c.imag); }
inline F sgn(F x) { return x < 0.?-1.F:1.F; }
inline V swap(F &a,F &b) { F c = a; a = b; b = c; }


class Vasp 
{
public:
	class Ref {
	public:
		Ref(): sym(NULL) {}
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
	

	// copy functions
	Vasp *m_copy(const Argument &arg); // copy to (one vec or real)
	Vasp *m_ccopy(const Argument &arg); // complex copy (pairs of vecs or complex)
	Vasp *m_vcopy(const Argument &arg); // copy to (multi-channel)

	// simple binary math
	Vasp *m_add(const Argument &arg); // add to (one vec or real)
	Vasp *m_cadd(const Argument &arg); // complex add (pairs of vecs or complex)
	Vasp *m_vadd(const Argument &arg); // add to (multi-channel)

	Vasp *m_sub(const Argument &arg); // sub from (one vec or real/complex)
	Vasp *m_csub(const Argument &arg); // complex sub (pairs of vecs or complex)
	Vasp *m_vsub(const Argument &arg); // sub from (multi-channel)

	Vasp *m_mul(const Argument &arg); // mul with (one vec or real/complex)
	Vasp *m_cmul(const Argument &arg); // complex mul (pairs of vecs or complex)
	Vasp *m_vmul(const Argument &arg); // mul with (multi-channel)

	Vasp *m_div(const Argument &arg); // div by (one vec or real/complex)
	Vasp *m_cdiv(const Argument &arg); // complex div (pairs of vecs or complex)
	Vasp *m_vdiv(const Argument &arg); // div by (multi-channel)

	Vasp *m_min(const Argument &arg); // min (one vec or real)
//	Vasp *m_cmin(const Argument &arg); // complex (abs) min (pairs of vecs or complex)
	Vasp *m_vmin(const Argument &arg); // min (multi-channel)

	Vasp *m_max(const Argument &arg); // max (one vec or real)
//	Vasp *m_cmax(const Argument &arg); // complex (abs) max (pairs of vecs or complex)
	Vasp *m_vmax(const Argument &arg); // max (multi-channel)

	Vasp *m_minmax(); // min/max 

	// "unary" functions
	Vasp *m_int(); // integrate
	Vasp *m_dif(); // differentiate
	Vasp *m_peaks(const Argument &arg); // find peaks

	Vasp *m_pow(const Argument &arg); // power
	Vasp *m_root(const Argument &arg); // real root (from abs value)
//	Vasp *m_cpow(const Argument &arg); // complex power (with each two channels)
	Vasp *m_sqr();   // unsigned square 
	Vasp *m_ssqr();   // signed square 
	Vasp *m_csqr();  // complex square (with each two channels)
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

	// Rearrange buffer 
	Vasp *m_shift(const Argument &arg);  // shift buffer
	Vasp *m_xshift(const Argument &arg);  // shift buffer (symmetrically)
	Vasp *m_rot(const Argument &arg);  // rotate buffer
	Vasp *m_xrot(const Argument &arg);  // rotate buffer (symmetrically)
	Vasp *m_mirr();  // mirror buffer
	Vasp *m_xmirr();  // mirror buffer (symmetrically)

	// Generator functions 
	Vasp *m_osc(const Argument &arg);  // real osc
	Vasp *m_mosc(const Argument &arg);  // * real osc
	Vasp *m_cosc(const Argument &arg);  // complex osc (phase rotates)
	Vasp *m_mcosc(const Argument &arg);  // * complex osc (phase rotates)
	Vasp *m_phasor(const Argument &arg);  // phasor
	Vasp *m_mphasor(const Argument &arg);  // * phasor
	Vasp *m_noise();  // real noise
	Vasp *m_cnoise(); // complex noise (arg and abs random)
	Vasp *m_bevelup();  // bevel up 
	Vasp *m_mbevelup();  // * bevel up (fade in)
	Vasp *m_beveldn();  // bevel down
	Vasp *m_mbeveldn();  // * bevel down (fade out)
	Vasp *m_window(const Argument &arg);  // window curve
	Vasp *m_mwindow(const Argument &arg);  // * window curve

	// Fourier transforms 
	Vasp *m_rfft();
	Vasp *m_rifft();
	Vasp *m_cfft();
	Vasp *m_cifft();

	typedef BL (*argfunRA)(I,F *,I,Argument &);
	typedef BL (*argfunCA)(I,F *,I,F *,I,Argument &);
	
	typedef BL (*argfunR)(I,F *,I,F);
	typedef BL (*argfunC)(I,F *,I,F *,I,F,F);
	typedef BL (*argfunV)(I,F *,I,const F *,I);
	typedef BL (*argfunCV)(I,F *,I,F *,I,const F *,I,const F *,I);

	struct nop_funcs {
		argfunR funR;
		argfunC funC;
	};

	struct arg_funcs {
		argfunR funR;
		argfunC funC;
		argfunV funV;
		argfunCV funCV;
	};

protected:
	I frames; // length counted in frames
	I chns; // used channels
	I refs; // allocated channels (>= chns)
	Ref *ref;

	V Resize(I rcnt);

	static I min(I a,I b) { return a < b?a:b; }
	static I max(I a,I b) { return a > b?a:b; }

private:
	typedef flext_base flx;

	Vasp *fr_nop(const C *op,F v,const nop_funcs &f) { return fr_arg(op,v,f.funR); }
	Vasp *fc_nop(const C *op,const CX &cx,const nop_funcs &f) { return fc_arg(op,cx,f.funC); }
	Vasp *fc_nop(const C *op,const Argument &params,const nop_funcs &f);

	Vasp *fr_prm(const C *op,argfunRA f,Argument &params);
	Vasp *fc_prm(const C *op,argfunCA f,Argument &params);
//	Vasp *fr_arg(const C *op,argfunVA f,Argument &a);
//	Vasp *fc_arg(const C *op,argfunCV f,Argument &a);
//	Vasp *fv_arg(const C *op,argfunV f,Argument &a);

	Vasp *fr_arg(const C *op,F v,argfunR f);
	Vasp *fr_arg(const C *op,F v,const arg_funcs &f) { return fr_arg(op,v,f.funR); }
	Vasp *fr_arg(const C *op,const Vasp &v,argfunV f);
	Vasp *fr_arg(const C *op,const Vasp &v,const arg_funcs &f) { return fr_arg(op,v,f.funV); }
	Vasp *fr_arg(const C *op,const Argument &arg,const arg_funcs &f);
	Vasp *fc_arg(const C *op,const CX &cx,argfunC f);
	Vasp *fc_arg(const C *op,const CX &cx,const arg_funcs &f) { return fc_arg(op,cx,f.funC); }
	Vasp *fc_arg(const C *op,const Vasp &v,argfunCV f);
	Vasp *fc_arg(const C *op,const Vasp &v,const arg_funcs &f) { return fc_arg(op,v,f.funCV); }
	Vasp *fc_arg(const C *op,const Argument &arg,const arg_funcs &f);
	Vasp *fv_arg(const C *op,const Vasp &v,argfunV f);
	Vasp *fv_arg(const C *op,const Vasp &v,const arg_funcs &f) { return fv_arg(op,v,f.funV); }
	Vasp *fv_arg(const C *op,const Argument &arg,const arg_funcs &f);
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
	vasp_op();

	virtual V m_bang() = 0;						// do! and output current Vasp

	virtual V m_vasp(I argc,t_atom *argv); // trigger
	virtual I m_set(I argc,t_atom *argv);  // non trigger

	virtual V m_update(I argc = 0,t_atom *argv = NULL);  // graphics update

	// destination vasp
	Vasp ref;

private:
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
	vasp_unop();

	virtual Vasp *x_work();
	virtual Vasp *tx_work();
};


// base class for binary operations

class vasp_binop:
	public vasp_tx
{
	FLEXT_HEADER(vasp_binop,vasp_tx)

protected:
	vasp_binop(I argc,t_atom *argv);

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
	vasp_anyop(I argc,t_atom *argv);

	// assignment functions
	virtual V a_list(I argc,t_atom *argv); 
/*
	virtual V a_vasp(I argc,t_atom *argv);
	virtual V a_float(F f); 
	virtual V a_complex(I argc,t_atom *argv); 
	virtual V a_vector(I argc,t_atom *argv); 
*/
	virtual Vasp *x_work();
	virtual Vasp *tx_work(const Argument &arg);

	Argument arg;

private:
	FLEXT_CALLBACK_G(a_list)
/*
	FLEXT_CALLBACK_G(a_vasp)
	FLEXT_CALLBACK_1(a_float,F)
	FLEXT_CALLBACK_G(a_complex)
	FLEXT_CALLBACK_G(a_vector)
*/
};



#define VASP_UNARY(name,op)														\
class vasp_u_##op:																\
	public vasp_unop															\
{																				\
	FLEXT_HEADER(vasp_u_##op,vasp_unop)											\
public:																			\
	vasp_u_##op() {}															\
protected:																		\
	virtual Vasp *tx_work() { return ref.m_##op(); }							\
};																				\
FLEXT_LIB(name,vasp_u_##op)

#define VASP_UNARY_SETUP(op) FLEXT_SETUP(vasp_u_##op);  


#define VASP_BINARY(name,op)													\
class vasp_b_ ## op:															\
	public vasp_binop															\
{																				\
	FLEXT_HEADER(vasp_b_##op,vasp_binop)										\
public:																			\
	vasp_b_##op(I argc,t_atom *argv): vasp_binop(argc,argv) {}					\
protected:																		\
	virtual Vasp *tx_work(const Argument &arg) { return ref.m_##op(arg); }		\
};																				\
FLEXT_LIB_G(name,vasp_b_##op)

#define VASP_BINARY_SETUP(op) FLEXT_SETUP(vasp_b_##op);  


#define VASP_ANYOP(name,op)													\
class vasp_l_ ## op:															\
	public vasp_anyop															\
{																				\
	FLEXT_HEADER(vasp_l_##op,vasp_anyop)										\
public:																			\
	vasp_l_##op(I argc,t_atom *argv): vasp_anyop(argc,argv) {}					\
protected:																		\
	virtual Vasp *tx_work(const Argument &arg) { return ref.m_##op(arg); }		\
};																				\
FLEXT_LIB_G(name,vasp_l_##op)

#define VASP_ANYOP_SETUP(op) FLEXT_SETUP(vasp_l_##op);  


#endif
