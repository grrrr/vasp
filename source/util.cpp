#include "main.h"

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
			case A_SYMBOL:
				lst[i].a_w.w_symbol = argv[i].a_w.w_symbol;
				break;
			case A_POINTER:
				lst[i].a_w.w_gpointer = argv[i].a_w.w_gpointer;
				break;
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



Argument::Argument(): tp(tp_none) {}
Argument::~Argument() { Clear(); }


V Argument::Parse(I argc,t_atom *argv)
{
	if(argc == 0)
		Clear();
	else if(argc == 1 && flext_base::IsFloat(argv[0]))
		Set(flext_base::GetFloat(argv[0]));
	else if(argc == 2 && flext_base::IsFloat(argv[0]) && flext_base::IsFloat(argv[1]))
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
}

V Argument::Clear()
{
	switch(tp) {
	case tp_vasp:
		if(dt.v) { delete dt.v; dt.v = NULL; }
		break;
/*
	case tp_list:
		if(dt.atoms) { delete dt.atoms; dt.atoms = NULL; }
		break;
*/
	case tp_vx:
		if(dt.vx) { delete dt.vx; dt.vx = NULL; }
		break;
	case tp_cx:
		if(dt.cx) { delete dt.cx; dt.cx = NULL; }
		break;
	case tp_float:
		break;
	}
	tp = tp_none;
}

V Argument::Set(Vasp *v)
{
	if(tp != tp_none) Clear();
	dt.v = v; tp = tp_vasp;
}

/*
V Argument::Set(I argc,t_atom *argv)
{
	if(tp != tp_none) Clear();
	dt.atoms = new AtomList(argc,argv); tp = tp_list;
}
*/

V Argument::Set(F f)
{
	if(tp != tp_none) Clear();
	dt.f = f; tp = tp_float;
}

V Argument::Set(F re,F im)
{
	if(tp != tp_none) Clear();
	dt.cx = new CX(re,im); tp = tp_cx;
}

V Argument::Set(VX *vec)
{
	if(tp != tp_none) Clear();
	dt.vx = vec; tp = tp_vx;
}




#if 0

class vasp_prepend:
	public flext_base
{
	FLEXT_HEADER(vasp_prepend,flext_base)

public:
	vasp_prepend(I argc,t_atom *argv);

	virtual V m_set(I argc,t_atom *argv);

private:
	virtual V m_method_(I inlet,t_symbol *s,I argc,t_atom *argv);

	FLEXT_CALLBACK_G(m_set)
};

FLEXT_NEW_G("vasp.prepend",vasp_prepend)


vasp_prepend::vasp_prepend(I argc,t_atom *argv)
{
	FLEXT_ADDMETHOD_(0,"set",m_set);
}

V vasp_prepend::m_set(I argc,t_atom *argv)
{
}

V vasp_prepend::m_method_(I inlet,t_symbol *s,I argc,t_atom *argv)
{
	post("%s: inlet=%i symbol=%s argc=%i",inlet,s->s_name,argc);
}


#endif