/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


///////////////////////////////////////////////////////////////////////////
// VBuffer class
///////////////////////////////////////////////////////////////////////////

VBuffer::VBuffer(t_symbol *s,I c,I l,I o): 
	chn(0),len(0),offs(0) 
{	
	Set(s,c,l,o); 
}

VBuffer::VBuffer(const VBuffer &v):
	chn(0),len(0),offs(0) 
{ 
	operator =(v); 
}


VBuffer::~VBuffer() {}

VBuffer &VBuffer::operator =(const VBuffer &v)
{
	return Set(v.Symbol(),v.Channel(),v.Length(),v.Offset());
}

VBuffer &VBuffer::Set(t_symbol *s,I c,I l,I o)
{
	parent::Set(s);

	chn = c;
	if(chn > Channels()) chn = Channels();
	offs = o;
	if(offs > Frames()) offs = Frames();
	len = l;
	if(offs+len > Frames()) len = Frames()-offs;

	return *this;
}



///////////////////////////////////////////////////////////////////////////
// Vasp class
///////////////////////////////////////////////////////////////////////////

Vasp::Vasp(): 
	atoms(0),atomlist(NULL),
	refs(0),chns(0),ref(NULL),
	frames(0) 
{ 
}

Vasp::Vasp(I argc,t_atom *argv):
	atoms(0),atomlist(NULL),
	refs(0),chns(0),ref(NULL),
	frames(0) 
{ 
	operator ()(argc,argv); 
}

Vasp::Vasp(const Vasp &v): 
	atoms(0),atomlist(NULL),
	refs(0),chns(0),ref(NULL),
	frames(0) 
{ 
	operator =(v); 
}

Vasp::~Vasp()
{
	if(ref) delete[] ref;
	if(atomlist) delete[] atomlist;
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


// parse argument list
Vasp &Vasp::operator ()(I argc,t_atom *argv)
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

	if(argc > ix && (flext_base::IsFlint(argv[ix]) || flext_base::IsFloat(argv[ix]))) {
		frames = flext_base::GetAFlint(argv[ix]);
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
		r.sym = bsym;

		if(argc > ix && (flext_base::IsFlint(argv[ix]) || flext_base::IsFloat(argv[ix]))) {
			r.chn = flext_base::GetAFlint(argv[ix]);
			ix++;
		}
		else
			r.chn = 0;

		if(argc > ix && (flext_base::IsFlint(argv[ix]) || flext_base::IsFloat(argv[ix]))) {
			r.offs = flext_base::GetAFlint(argv[ix]);
			ix++;
		}
		else
			r.offs = 0;

		chns++;
	}

	if(!lenset) {
		// set length to maximum!
		// or let it be -1 to represent the maximum?!

		// if len is already set then where to check for oversize?
	}

	return *this;
}


// generate Vasp list of buffer references
Vasp &Vasp::MakeList(BL withvasp)
{
	I voffs = withvasp?1:0;
	I needed = voffs+1+Vectors()*3;
	if(!atomlist || needed != atoms) {
		if(atomlist) delete[] atomlist;
		atomlist = new t_atom[atoms = needed];
	}

	if(withvasp)
		flext_base::SetSymbol(atomlist[0],vasp_base::sym_vasp);  // VASP

	flext_base::SetFlint(atomlist[voffs],frames);  // frames

	for(I ix = 0; ix < Vectors(); ++ix) {
		Ref &r = Vector(ix);
		flext_base::SetSymbol(atomlist[voffs+1+ix*3],r.sym);  // buf
		flext_base::SetFlint(atomlist[voffs+2+ix*3],r.chn);  // chn
		flext_base::SetFlint(atomlist[voffs+3+ix*3],r.offs);  // offs
	}

	return *this;
}

VBuffer *Vasp::Buffer(I ix) const
{
	if(ix >= Vectors()) 
		return NULL;
	else {
		const Ref &r = Vector(ix);
		return new VBuffer(r.Symbol(),r.Channel(),Frames(),r.Offset());
	}
}




