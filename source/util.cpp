#include "main.h"
#include <math.h>

#ifndef PI
#define PI 3.1415926535897932385
#endif

F arg(F re,F im)
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
		Set(flext_base::GetFloat(argv[0]));
	else if(argc == 2 && (flext_base::IsFloat(argv[0]) || flext_base::IsInt(argv[0])) && (flext_base::IsFloat(argv[1]) || flext_base::IsInt(argv[1])))
		Set(flext_base::GetFloat(argv[1]),flext_base::GetFloat(argv[2]));
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

I Argument::GetAInt() const 
{
	if(IsInt()) return GetInt();
	else if(IsFloat()) return (I)GetFloat();
	else return 0;
}

F Argument::GetAFloat() const
{
	if(IsInt()) return GetInt();
	else if(IsFloat()) return GetFloat();
	else return 0;
}

CX Argument::GetAComplex() const
{
	if(IsInt()) return (F)GetInt();
	else if(IsFloat()) return GetFloat();
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



VecBlock::VecBlock(I msrc,I marg,I mdst):
	asrc(msrc),aarg(marg),adst(mdst)
{
	I i,all = asrc+aarg+adst;
	vecs = new VBuffer *[all];
	for(i = 0; i < all; ++i) vecs[i] = NULL;
}

VecBlock::~VecBlock()
{
	if(vecs) {
		I all = asrc+aarg+adst;
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


V OpParam::SDR_Rev() { SR_Rev(); DR_Rev(); }
V OpParam::SDI_Rev() { SI_Rev(); DI_Rev(); }
V OpParam::SDC_Rev() { SDR_Rev(); SDI_Rev(); }
V OpParam::SADR_Rev() { SR_Rev(); AR_Rev(); DR_Rev(); }
V OpParam::SADI_Rev() { SI_Rev(); AI_Rev(); DI_Rev(); }
V OpParam::SADC_Rev() { SADR_Rev(); SADI_Rev(); }

