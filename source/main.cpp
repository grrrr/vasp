/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

#define VASPVASP "vasp"


#ifdef PD
// Initialization for pd xsample library

extern "C" {
// base classes
FLEXT_EXT V vasp_v_setup();

// utils
FLEXT_EXT V vasp_prepend_setup();


FLEXT_EXT V vasp_setup()
{
	post("VASP - vector assembling signal processor, (C)2002 Thomas Grill");
	post("");

	// call the objects' setup routines

	vasp_v_setup();

	vasp_prepend_setup();
}
}
#endif


///////////////////////////////////////////////////////////////////////////
// vbuffer class
///////////////////////////////////////////////////////////////////////////

vbuffer::vbuffer(t_symbol *s,I c,I l,I o) {	Set(s,c,l,o); }

vbuffer::vbuffer(const vbuffer &v) { operator =(v); }

vbuffer::~vbuffer() {}

vbuffer &vbuffer::operator =(const vbuffer &v)
{
	return Set(v.Symbol(),v.Channel(),v.Length(),v.Offset());
}

vbuffer &vbuffer::Set(t_symbol *s,I c,I l,I o)
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
// vasp class
///////////////////////////////////////////////////////////////////////////

t_symbol *const vasp::vaspsym = gensym(VASPVASP);


vasp::vasp(): 
	atoms(0),atomlist(NULL),
	refs(0),chns(0),ref(NULL),
	frames(0) 
{ 
}

vasp::vasp(I argc,t_atom *argv):
	atoms(0),atomlist(NULL),
	refs(0),chns(0),ref(NULL),
	frames(0) 
{ 
	operator ()(argc,argv); 
}

vasp::vasp(const vasp &v): 
	atoms(0),atomlist(NULL),
	refs(0),chns(0),ref(NULL),
	frames(0) 
{ 
	operator =(v); 
}

vasp::~vasp()
{
	if(ref) delete[] ref;
	if(atomlist) delete[] atomlist;
}



vasp &vasp::operator =(const vasp &v)
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
vasp &vasp::operator ()(I argc,t_atom *argv)
{
	BL lenset = false;
	I ix = 0;

	I maxneeded = argc; // maximum number of ref'd buffers
	// rather use a temp storage 
	if(!ref || refs < maxneeded) {
		if(ref) delete[] ref;
		ref = new Ref[refs = maxneeded];
	}

	t_symbol *v = atom_getsymbolarg(ix,argc,argv);
	if(v && v == vaspsym) ix++; // if it is "vasp" ignore it

	if(argc > ix && (ISFLINT(argv[ix]) || ISFLOAT(argv[ix]))) {
		frames = atom_getflintarg(ix,argc,argv);
		lenset = true;
		ix++;
	}
	else
		frames = -1;

	chns = 0;
	while(argc > ix) {
		t_symbol *bsym = atom_getsymbolarg(ix,argc,argv);
		if(!bsym || !bsym->s_name || !*bsym->s_name) {  // expect a symbol
			Clear();
			return *this;
		}
		else
			ix++;

		// is a symbol!
		Ref &r = ref[chns];
		r.sym = bsym;

		if(argc > ix && (ISFLINT(argv[ix]) || ISFLOAT(argv[ix]))) {
			r.chn = atom_getflintarg(ix,argc,argv);
			ix++;
		}
		else
			r.chn = 0;

		if(argc > ix && (ISFLINT(argv[ix]) || ISFLOAT(argv[ix]))) {
			r.offs = atom_getflintarg(ix,argc,argv);
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


// generate vasp list of buffer references
vasp &vasp::MakeList(BL withvasp)
{
	I voffs = withvasp?1:0;
	I needed = voffs+1+Channels()*3;
	if(!atomlist || needed != atoms) {
		if(atomlist) delete[] atomlist;
		atomlist = new t_atom[atoms = needed];
	}

	if(withvasp)
		SETSYMBOL(&atomlist[0],vaspsym);  // VASP

	SETFLINT(&atomlist[voffs],frames);  // frames

	for(I ix = 0; ix < Channels(); ++ix) {
		Ref &r = Channel(ix);
		SETSYMBOL(&atomlist[voffs+1+ix*3],r.sym);  // buf
		SETFLINT(&atomlist[voffs+2+ix*3],r.chn);  // chn
		SETFLINT(&atomlist[voffs+3+ix*3],r.offs);  // offs
	}

	return *this;
}



///////////////////////////////////////////////////////////////////////////
// vasp_msg class
///////////////////////////////////////////////////////////////////////////

V vasp_msg::cb_setup(t_class *c)
{
	FLEXT_ADDBANG(c,m_bang);
//	FLEXT_ADDMETHOD_G(c,"vasp",m_vasp);
	FLEXT_ADDMETHOD_G(c,"set",m_set);

	FLEXT_ADDMETHOD_E(c,"unit",m_unit);
}

vasp_msg::vasp_msg()
{
}

vasp_msg::~vasp_msg()
{
}

/*
I vasp_msg::m_set(I argc,t_atom *argv)
{

	I ret = buf->Set(atom_getsymbolarg(0,argc,argv));
	if(ret) {
		m_offset(Offset());
		m_length(Length());
	}
	return ret;
}
*/

V vasp_msg::m_bang()
{
	if(ref.Ok()) {
		ref.MakeList(false);
		to_out_anything(0,vasp::vaspsym,ref.Atoms(),ref.AtomList());
	}
}

V vasp_msg::m_vasp(I argc,t_atom *argv)
{
	m_set(argc,argv);
	m_bang();
}


V vasp_msg::m_unit(xs_unit u) {	unit = u; }




///////////////////////////////////////////////////////////////////////////


