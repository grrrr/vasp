#include "main.h"
#include <math.h>

#ifndef PI
#define PI 3.1415926535897932385
#endif

R arg(R re,R im)
{
	if(re) 
		return atan(im/re)+(re < 0?PI:0);
	else
		if(im || re) return im > 0?PI/2:-PI/2;
		else return 0;
}




AtomList::AtomList(I argc,t_atom *argv):
	cnt(argc)
{
	lst = new t_atom[cnt];

	if(argv) {
		for(I i = 0; i < argc; ++i) {
			switch(lst[i].a_type = argv[i].a_type) {
			case A_FLOAT:
				lst[i].a_w.w_float = argv[i].a_w.w_float;
				break;
#ifdef MAXMSP
			case A_LONG:
				lst[i].a_w.w_int = argv[i].a_w.w_int;
				break;
#endif
			case A_SYMBOL:
				lst[i].a_w.w_symbol = argv[i].a_w.w_symbol;
				break;
#ifdef PD
			case A_POINTER:
				lst[i].a_w.w_gpointer = argv[i].a_w.w_gpointer;
				break;
#endif
			default:
				post("AtomList - atom type (%i) not supported",lst[i].a_type);
				lst[i].a_type = A_NULL;
				break;
			}
		}
	}
}

AtomList::~AtomList()
{
	if(lst) delete[] lst;
}



Argument::Argument(): tp(tp_none),nxt(NULL) {}
Argument::~Argument() { ClearAll(); }


Argument &Argument::Parse(I argc,t_atom *argv)
{
	if(argc == 0)
		Clear();
	else if(argc == 1 && (flext_base::IsFloat(argv[0]) || flext_base::IsInt(argv[0]))) 
		Set(flext_base::GetAFloat(argv[0]));
	else if(argc == 2 && (flext_base::IsFloat(argv[0]) || flext_base::IsInt(argv[0])) && (flext_base::IsFloat(argv[1]) || flext_base::IsInt(argv[1])))
		Set(flext_base::GetAFloat(argv[1]),flext_base::GetAFloat(argv[2]));
/*
	else if(argc == 3 && flext_base::IsFloat(argv[0]) && flext_base::IsFloat(argv[1]) && flext_base::IsFloat(argv[2])) {
		VX *v = new VX(argc,argv);
		if(v && v->Ok()) Set(v);
		else {
			post("Argument: vector is invalid");
			delete v;
		}
	}
*/
	else {
		Vasp *v = new Vasp(argc,argv);
		if(v && v->Ok()) Set(v);
		else {
			Clear();
			post("Argument: vasp is invalid");
			delete v;
		}
	}
	return *this;
}

Argument &Argument::Clear()
{
	switch(tp) {
	case tp_none:
		break;
	case tp_vasp:
		if(dt.v) { delete dt.v; dt.v = NULL; }
		break;
	case tp_list:
		if(dt.atoms) { delete dt.atoms; dt.atoms = NULL; }
		break;
	case tp_vx:
		if(dt.vx) { delete dt.vx; dt.vx = NULL; }
		break;
	case tp_cx:
		if(dt.cx) { delete dt.cx; dt.cx = NULL; }
		break;
	case tp_int:
	case tp_float:
		break;
	default:
		error("Argument: Internal error - type unknown!");
	}
	tp = tp_none;
	return *this;
}

Argument &Argument::ClearAll()
{
	Clear();
	if(nxt) { delete nxt; nxt = NULL; }
	return *this;
}

Argument &Argument::Set(Vasp *v)
{
	if(tp != tp_none) Clear();
	dt.v = v; tp = tp_vasp;
	return *this;
}

Argument &Argument::Set(I argc,t_atom *argv)
{
	if(tp != tp_none) Clear();
	dt.atoms = new AtomList(argc,argv); tp = tp_list;
	return *this;
}

Argument &Argument::Set(F f)
{
	if(tp != tp_none) Clear();
	dt.f = f; tp = tp_float;
	return *this;
}

Argument &Argument::Set(I i)
{
	if(tp != tp_none) Clear();
	dt.i = i; tp = tp_int;
	return *this;
}

Argument &Argument::Set(F re,F im)
{
	if(tp != tp_none) Clear();
	dt.cx = new CX(re,im); tp = tp_cx;
	return *this;
}

Argument &Argument::Set(VX *vec)
{
	if(tp != tp_none) Clear();
	dt.vx = vec; tp = tp_vx;
	return *this;
}

I Argument::GetAInt() const { return (I)GetADouble(); }

F Argument::GetAFloat() const { return GetADouble(); }

D Argument::GetADouble() const
{
	if(IsInt()) return GetInt();
	else if(IsFloat()) return GetFloat();
	else if(IsDouble()) return GetDouble();
	else return 0;
}

CX Argument::GetAComplex() const
{
	if(IsInt()) return (F)GetInt();
	else if(IsFloat()) return GetFloat();
	else if(IsDouble()) return GetDouble();
	else if(IsComplex()) return GetComplex();
	else return 0;
}

/*
VX Argument::GetAVector() const
{
}
*/


Argument &Argument::Add(Argument *n) 
{ 
	if(nxt) nxt->Add(n);
	else nxt = n;
	return *n;
}

Argument &Argument::Next(I i)
{
	if(i <= 0) return *this;
	else {
		Argument *n = Next();
		if(n) return n->Next(i-1);
		else {
			error("Argument: index not found!");
			return *this;
		}
	}
}

Argument &Argument::Add(Vasp *v) { Argument *a = new Argument; a->Set(v); return Add(a); }

Argument &Argument::Add(I argc,t_atom *argv) { Argument *a = new Argument; a->Set(argc,argv); return Add(a); }

Argument &Argument::Add(I i) { Argument *a = new Argument; a->Set(i); return Add(a); }

Argument &Argument::Add(F f) { Argument *a = new Argument; a->Set(f); return Add(a); }

Argument &Argument::Add(F re,F im) { Argument *a = new Argument; a->Set(re,im); return Add(a); }

Argument &Argument::Add(VX *vec) { Argument *a = new Argument; a->Set(vec); return Add(a); }



VecBlock::VecBlock(BL cx,I msrc,I mdst,I marg,I blarg):
	cplx(cx),asrc(msrc),barg(blarg),aarg(marg*blarg),adst(mdst)
{
	I i,all = asrc+aarg*blarg+adst;
	vecs = new VBuffer *[all];
	for(i = 0; i < all; ++i) vecs[i] = NULL;
}

VecBlock::~VecBlock()
{
	if(vecs) {
		I all = asrc+aarg*barg+adst;
		for(I i = 0; i < all; ++i) 
			if(vecs[i]) delete vecs[i];
		delete[] vecs;
	}
}

Vasp *VecBlock::_DstVasp(I n)
{
	Vasp *ret = new Vasp;
	ret->Frames(Frames());
	for(I i = 0; i < n; ++i) *ret += Vasp::Ref(*_Dst(i));
	return ret;
}

Vasp *VecBlock::_SrcVasp(I n)
{
	Vasp *ret = new Vasp;
	ret->Frames(Frames());
	for(I i = 0; i < n; ++i) *ret += Vasp::Ref(*_Src(i));
	return ret;
}

Vasp *VecBlock::_ResVasp(I n)
{
	return _Dst(0)?_DstVasp(n):_SrcVasp(n);
}


// Duplication of breakpoint lists should be avoided
OpArg &OpArg::operator =(const OpArg &op)
{
	Clear();

	switch(argtp = op.argtp) {
	case arg_x:	x = op.x; break;
	case arg_v:	v = op.v; break;
	case arg_l:	{
		// Copy breakpoint list (find a different way...)
		l.pts = op.l.pts;
		l.r = new R[l.pts];
		l.i = new R[l.pts];
		for(I i = 0; i < l.pts; ++i) 
			l.r[i] = op.l.r[i],l.i[i] = op.l.i[i];
		break;
	}
	}

	return *this;
}

V OpArg::Clear()
{
	if(argtp == arg_l) {
		if(l.r) delete[] l.r;	
		if(l.i) delete[] l.i;	
	}
	argtp = arg_;
}

OpArg &OpArg::SetX(S r,S i)
{
	Clear();
	argtp = arg_x;
	x.r = r,x.i = i;
	return *this;
}

OpArg &OpArg::SetV(S *r,I rs,S *i,I is)
{
	Clear();
	argtp = arg_v;
	v.rdt = r,v.rs = rs;
	v.idt = i,v.is = is;
	return *this;
}

OpArg &OpArg::SetL(I pts,R *r,R *i)
{
	Clear();
	argtp = arg_l;
	l.pts = pts;
	l.r = new R[pts];
	l.i = new R[pts];
	for(I ix = 0; ix < pts; ix) 
		l.r[ix] = r[ix],l.i[ix] = i[ix];
	return *this;
}



/*
V OpParam::SDR_Rev() { SR_Rev(); DR_Rev(); }
V OpParam::SDI_Rev() { SI_Rev(); DI_Rev(); }
V OpParam::SDC_Rev() { SDR_Rev(); SDI_Rev(); }
V OpParam::ADR_Rev() { AR_Rev(); DR_Rev(); }
V OpParam::ADI_Rev() { AI_Rev(); DI_Rev(); }
V OpParam::ADC_Rev() { ADR_Rev(); ADI_Rev(); }
V OpParam::SADR_Rev() { SR_Rev(); AR_Rev(); DR_Rev(); }
V OpParam::SADI_Rev() { SI_Rev(); AI_Rev(); DI_Rev(); }
V OpParam::SADC_Rev() { SADR_Rev(); SADI_Rev(); }
*/

OpParam::OpParam(const C *opnm,I nargs): 
	opname(opnm),frames(0),args(0),arg(NULL),
	/*part(false),*/ ovrlap(false),revdir(false) 
{
	InitArgs(nargs);
}

OpParam::~OpParam() { Clear(); }

V OpParam::InitArgs(I n)
{
	if(arg) Clear();
	args = n;
	if(args) arg = new OpArg[args];
}

V OpParam::Clear()
{
	if(arg) { delete[] arg; arg = NULL;	}
	args = 0;
}


/*! \brief Reverse direction of real vector operation 
	\todo Check for existence of vectors!
*/
V OpParam::R_Rev() 
{ 

	SR_Rev(); 
	DR_Rev();
	AR_Rev(); 
	revdir = true;
}

/*! \brief Reverse direction of complex vector operation 
	\todo Check for existence of vectors!
*/
V OpParam::C_Rev() 
{ 
	SR_Rev(); SI_Rev(); 
	DR_Rev(); DI_Rev();
	AR_Rev(); AI_Rev(); 
	revdir = true;
}


V OpParam::AR_Rev(I bl) 
{ 
	if(arg[bl].argtp == OpArg::arg_v && arg[bl].v.rdt) 
		arg[bl].v.rdt -= (frames-1)*(arg[bl].v.rs = -arg[bl].v.rs); 
}

V OpParam::AI_Rev(I bl) 
{ 
	if(arg[bl].argtp == OpArg::arg_v && arg[bl].v.idt) 
		arg[bl].v.idt -= (frames-1)*(arg[bl].v.is = -arg[bl].v.is); 
}

BL OpParam::AR_In(I bl) const { return arg[bl].argtp == OpArg::arg_v && arg[bl].v.rdt && rddt > arg[bl].v.rdt && rddt < arg[bl].v.rdt+frames*arg[bl].v.rs; } 
BL OpParam::AI_In(I bl) const { return arg[bl].argtp == OpArg::arg_v && arg[bl].v.idt && iddt > arg[bl].v.idt && iddt < arg[bl].v.idt+frames*arg[bl].v.is; } 

BL OpParam::AR_Can(I bl) const { return arg[bl].argtp != OpArg::arg_v || !arg[bl].v.rdt || arg[bl].v.rdt <= rddt || arg[bl].v.rdt >= rddt+frames*rds; } 
BL OpParam::AI_Can(I bl) const { return arg[bl].argtp != OpArg::arg_v || !arg[bl].v.idt || arg[bl].v.idt <= iddt || arg[bl].v.idt >= iddt+frames*ids; } 

BL OpParam::AR_Ovr(I bl) const { return arg[bl].argtp == OpArg::arg_v && arg[bl].v.rdt && rddt != arg[bl].v.rdt && rddt < arg[bl].v.rdt+frames*arg[bl].v.rs && arg[bl].v.rdt < rddt+frames*rds; } 
BL OpParam::AI_Ovr(I bl) const { return arg[bl].argtp == OpArg::arg_v && arg[bl].v.idt && iddt != arg[bl].v.idt && iddt < arg[bl].v.idt+frames*arg[bl].v.is && arg[bl].v.idt < iddt+frames*ids; } 



BL OpParam::AR_In() const
{
	for(I i = 0; i < args; ++i) 
		if(AR_In(i)) return true;
	return false;
}

BL OpParam::AI_In() const
{
	for(I i = 0; i < args; ++i) 
		if(!AI_In(i)) return true;
	return false;
}

BL OpParam::AR_Can() const
{
	for(I i = 0; i < args; ++i) 
		if(!AR_Can(i)) return false;
	return true;
}

BL OpParam::AI_Can() const
{
	for(I i = 0; i < args; ++i) 
		if(!AI_Can(i)) return false;
	return true;
}

BL OpParam::AR_Ovr() const
{
	for(I i = 0; i < args; ++i) 
		if(!AR_Ovr(i)) return false;
	return true;
}

BL OpParam::AI_Ovr() const
{
	for(I i = 0; i < args; ++i) 
		if(!AI_Ovr(i)) return false;
	return true;
}


V OpParam::AR_Rev()
{
	for(I i = 0; i < args; ++i) AR_Rev(i);
}

V OpParam::AI_Rev()
{
	for(I i = 0; i < args; ++i) AI_Rev(i);
}




