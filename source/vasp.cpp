/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "classes.h"
#include "util.h"
#include "buflib.h"

///////////////////////////////////////////////////////////////////////////
// Vasp class
///////////////////////////////////////////////////////////////////////////

Vasp::Ref::Ref(VBuffer &b): sym(b.Symbol()),chn(b.Channel()),offs(b.Offset()) {}
Vasp::Ref::Ref(VSymbol &s,I c,I o): sym(s),chn(c),offs(o) {}
Vasp::Ref::~Ref() {}

Vasp::Ref &Vasp::Ref::operator =(const Ref &r)
{
	sym = r.sym,chn = r.chn,offs = r.offs;
	return *this;
}

V Vasp::Ref::Symbol(const VSymbol &s) { sym = s; }


Vasp::Vasp(): 
	refs(0),chns(0),ref(NULL),
	frames(0) 
{ 
}

Vasp::Vasp(I argc,const t_atom *argv):
	refs(0),chns(0),ref(NULL),
	frames(0) 
{ 
	operator ()(argc,argv); 
}

Vasp::Vasp(const Vasp &v): 
	refs(0),chns(0),ref(NULL),
	frames(0) 
{ 
	operator =(v); 
}

Vasp::Vasp(I fr,const Ref &r):
	refs(0),chns(0),ref(NULL),
	frames(fr) 
{
	operator +=(r);
}


Vasp::~Vasp()
{
	if(ref) delete[] ref;
}


BL Vasp::ChkArgs(I argc,const t_atom *argv)
{
	I ix = 0;

	// vasp keyword
	t_symbol *v = ix < argc?flext_base::GetASymbol(argv[ix]):NULL;
	if(v && v == vasp_base::sym_vasp) ix++; // if it is "vasp" ignore it

	// length argument
	if(argc > ix && flext_base::CanbeInt(argv[ix])) ix++;

	while(argc > ix) {
		// check for symbol
		t_symbol *bsym = flext_base::GetASymbol(argv[ix]);
		if(!bsym || !flext_base::GetString(bsym) || !flext_base::GetString(bsym)[0]) {  // expect a symbol
			// not symbol -> bail out
			return false;
		}
		else
			ix++;

		// check for offset
		if(argc > ix && flext_base::CanbeInt(argv[ix])) ix++;

		// check for channel
		if(argc > ix && flext_base::CanbeInt(argv[ix])) ix++;
	}

	return true;
}

V Vasp::Resize(I rcnt) {
	if(!ref) {
		ref = new Ref[refs = rcnt];
		chns = 0;
	}
	else if(rcnt > refs) {
		Ref *rnew = new Ref[refs = rcnt];
		for(I ix = 0; ix < chns; ++ix) rnew[ix] = ref[ix];
		delete[] ref;
		ref = rnew; 
	}
}



Vasp &Vasp::operator =(const Vasp &v)
{
	if(!v.Ok()) 
		Clear();
	else {
		frames = v.frames;
		if(!ref || v.chns > refs) {
			if(ref) delete[] ref;
			ref = new Ref[refs = v.chns];
		}

		chns = v.chns;
		for(I ix = 0; ix < chns; ++ix) {
			ref[ix] = v.ref[ix];
		}
	}

	return *this;
}


Vasp &Vasp::operator +=(const Ref &r)
{
	Resize(chns+1);
	ref[chns++] = r;
	return *this;
}


Vasp &Vasp::operator +=(const Vasp &v)
{
	if(v.Ok()) {
		if(!Ok()) *this = v;
		else {
			if(Frames() != v.Frames()) {
				post("vasp - Frame count of joined vasps is different - taking the minimum");
				Frames(min(Frames(),v.Frames()));
			}

			Resize(Vectors()+v.Vectors());
			for(I i = 0; i < v.Vectors(); ++i) *this += v.Vector(i);
		}
	}
	return *this;
}

// parse argument list
Vasp &Vasp::operator ()(I argc,const t_atom *argv)
{
	BL lenset = false;
	I ix = 0;

	I maxneeded = argc; // maximum number of ref'd buffers
	// rather use a temp storage 
	if(!ref || refs < maxneeded) {
		if(ref) delete[] ref;
		ref = new Ref[refs = maxneeded];
	}

	t_symbol *v = ix < argc?flext_base::GetASymbol(argv[ix]):NULL;
	if(v && v == vasp_base::sym_vasp) ix++; // if it is "vasp" ignore it

	if(argc > ix && flext_base::CanbeInt(argv[ix])) {
		frames = flext_base::GetAInt(argv[ix]);
		lenset = true;
		ix++;
	}
	else
		frames = -1;

	chns = 0;
	while(argc > ix) {
		t_symbol *bsym = flext_base::GetASymbol(argv[ix]);
		if(!bsym || !flext_base::GetString(bsym) || !flext_base::GetString(bsym)[0]) {  // expect a symbol
			Clear();
			return *this;
		}
		else
			ix++;

		// is a symbol!
		Ref &r = ref[chns];
		r.Symbol(VSymbol(bsym));

		if(argc > ix && (flext_base::IsFlint(argv[ix]) || flext_base::IsFloat(argv[ix]))) {
			r.Offset(flext_base::GetAFlint(argv[ix]));
			ix++;
		}
		else
			r.Offset(0);

		if(argc > ix && (flext_base::IsFlint(argv[ix]) || flext_base::IsFloat(argv[ix]))) {
			r.Channel(flext_base::GetAFlint(argv[ix]));
			ix++;
		}
		else
			r.Channel(0);

		chns++;
	}

	if(!lenset) {
		// set length to maximum!
		// or let it be -1 to represent the maximum?!
		frames = -1;
		// if len is already set then where to check for oversize?
	}

	return *this;
}


VBuffer *Vasp::Buffer(I ix) const
{
	if(ix >= Vectors()) 
		return NULL;
	else {
		const Ref &r = Vector(ix);
		VBuffer *ret = BufLib::Get(r.Symbol(),r.Channel(),Frames(),r.Offset());
		return ret;
	}
}

// generate Vasp list of buffer references
flext_base::AtomList *Vasp::MakeList(BL withvasp)
{
	I voffs = withvasp?1:0;
	I needed = voffs+1+Vectors()*3;
	flext_base::AtomList *ret = new flext_base::AtomList(needed);

	if(withvasp) 
		flext_base::SetSymbol((*ret)[0],vasp_base::sym_vasp);  // VASP

	flext_base::SetFlint((*ret)[voffs],frames);  // frames

	for(I ix = 0; ix < Vectors(); ++ix) {
		Ref &r = Vector(ix);
		flext_base::SetSymbol((*ret)[voffs+1+ix*3],r.Symbol().Symbol());  // buf
		flext_base::SetFlint((*ret)[voffs+2+ix*3],r.Offset());  // offs
		flext_base::SetFlint((*ret)[voffs+3+ix*3],r.Channel());  // chn
	}

	return ret;
}


V Vasp::Refresh()
{
	for(I i = 0; i < Vectors(); ++i) {
		VBuffer *vb = Buffer(i);
		if(vb) {
			vb->Refresh();
			delete vb;
		}
	}
}

V Vasp::Offset(I o)
{
	for(I i = 0; i < Vectors(); ++i) Vector(i).Offset(o);
}

V Vasp::OffsetD(I od)
{
	for(I i = 0; i < Vectors(); ++i) Vector(i).OffsetD(od);
}


V Vasp::Size(I s)
{
	for(I i = 0; i < Vectors(); ++i) {
		VBuffer *buf = Buffer(i);
		if(buf) { 
			buf->Frames(s,true);
			delete buf;
		}
	}
}

V Vasp::SizeD(I sd)
{
	for(I i = 0; i < Vectors(); ++i) {
		VBuffer *buf = Buffer(i);
		if(buf) { 
			I s = buf->Frames()+sd;
			buf->Frames(s >= 0?s:0,true);
			delete buf;
		}
	}
}


V Vasp::SizeM(R f)
{
	for(I i = 0; i < Vectors(); ++i) {
		VBuffer *buf = Buffer(i);
		if(buf) { 
			I s = buf->Frames()*f;
			buf->Frames(s >= 0?s:0,true);
			delete buf;
		}
	}
}


I Vasp::ChkFrames() const
{
	if(Vectors() == 0) return 0;

	I frms = -1;
	for(I i = 0; i < Vectors(); ++i) {
		VBuffer *buf = Buffer(i);
		if(buf) {
			I f = buf->Length();
			if(frms < 0 || f < frms) frms = f;
			delete buf;
		}
	}

	return frms < 0?0:frms;
}

