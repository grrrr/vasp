#include "arg.h"
//#include <math.h>

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


