#include "main.h"

AtomList::AtomList(I argc,t_atom *argv):
	cnt(argc)
{
	if(argv) {
		lst = new t_atom[cnt];

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
	else
		lst = NULL;
}

AtomList::~AtomList()
{
	if(lst) delete[] lst;
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