#include "main.h"

FLEXT_GIMME("vasp.=",vasp_tx_copy)

/*
V vasp_tx_copy::cb_setup(t_class *c)
{
	// assignment functions
//	FLEXT_ADDMETHOD_G(c,"copy",m_copy);
}
*/

vasp_tx_copy::vasp_tx_copy(I argc,t_atom *argv)
{
	post("%s - copy setup called",thisName());

//	m_set(argc,argv);

	add_in_anything(4);
	add_out_anything();
	setup_inout();

	FLEXT_ADDMETHOD_(0,"copy",m_copy);
}

vasp_tx_copy::~vasp_tx_copy()
{
}

V vasp_tx_copy::m_copy(I argc,t_atom *argv) 
{ 
	post("%s - copy method called",thisName());
}






FLEXT_GIMME("vasp.tx",vasp_tx)


vasp_tx::vasp_tx(I argc,t_atom *argv)
{
	m_set(argc,argv);

	add_in_anything();
	add_out_anything();
	setup_inout();


//	FLEXT_ADDMETHOD_G(c,"set",m_set);

	// assignment functions
	FLEXT_ADDMETHOD_(0,"copy",m_copy);
	FLEXT_ADDMETHOD_(0,"ccopy",m_ccopy);
	FLEXT_ADDMETHOD_(0,"mcopy",m_mcopy);
//	FLEXT_ADDMETHOD_(0,"mix",m_mix);
//	FLEXT_ADDMETHOD_(0,"cmix",m_cmix);
//	FLEXT_ADDMETHOD_(0,"mmix",m_mmix);

	// binary functions
	FLEXT_ADDMETHOD_(0,"add",m_add);
	FLEXT_ADDMETHOD_(0,"cadd",m_cadd);
	FLEXT_ADDMETHOD_(0,"madd",m_madd);
	FLEXT_ADDMETHOD_(0,"sub",m_sub);
	FLEXT_ADDMETHOD_(0,"csub",m_csub);
	FLEXT_ADDMETHOD_(0,"msub",m_msub);
	FLEXT_ADDMETHOD_(0,"mul",m_mul);
	FLEXT_ADDMETHOD_(0,"cmul",m_cmul);
	FLEXT_ADDMETHOD_(0,"mmul",m_mmul);
	FLEXT_ADDMETHOD_(0,"div",m_div);
	FLEXT_ADDMETHOD_(0,"cdiv",m_cdiv);
	FLEXT_ADDMETHOD_(0,"mdiv",m_mdiv);

	FLEXT_ADDMETHOD_(0,"min",m_min);
//	FLEXT_ADDMETHOD_(0,"cmin",m_cmin);
	FLEXT_ADDMETHOD_(0,"mmin",m_mmin);
	FLEXT_ADDMETHOD_(0,"max",m_max);
//	FLEXT_ADDMETHOD_(0,"cmax",m_cmax);
	FLEXT_ADDMETHOD_(0,"mmax",m_mmax);

	// unary functions
	FLEXT_ADDMETHOD_1(0,"pow",m_pow,F);
//	FLEXT_ADDMETHOD_(0,"cpow",m_cpow);
	FLEXT_ADDMETHOD_(0,"sqr",m_sqr);
	FLEXT_ADDMETHOD_(0,"csqr",m_csqr);
	FLEXT_ADDMETHOD_1(0,"root",m_root,F);
	FLEXT_ADDMETHOD_(0,"sqrt",m_sqrt);

	FLEXT_ADDMETHOD_(0,"inv",m_inv);
	FLEXT_ADDMETHOD_(0,"cinv",m_cinv);
	FLEXT_ADDMETHOD_(0,"abs",m_abs);
	FLEXT_ADDMETHOD_(0,"polar",m_polar);
	FLEXT_ADDMETHOD_(0,"cart",m_cart);

	FLEXT_ADDMETHOD_(0,"norm",m_norm);
	FLEXT_ADDMETHOD_(0,"cnorm",m_cnorm);
//	FLEXT_ADDMETHOD_(0,"opt",m_norm);
//	FLEXT_ADDMETHOD_(0,"copt",m_cnorm);

	FLEXT_ADDMETHOD_(0,"cswap",m_cswap);
	FLEXT_ADDMETHOD_(0,"cconj",m_cconj);

	// Buffer rearrange functions
	FLEXT_ADDMETHOD_1(0,"shift",m_shift,F);
	FLEXT_ADDMETHOD_1(0,"xshift",m_xshift,F);
	FLEXT_ADDMETHOD_1(0,"rot",m_rot,F);
	FLEXT_ADDMETHOD_1(0,"xrot",m_xrot,F);
	FLEXT_ADDMETHOD_(0,"mirr",m_mirr);
	FLEXT_ADDMETHOD_(0,"xmirr",m_xmirr);
/*
	// Generator functions
	FLEXT_ADDMETHOD_(0,"osc",m_osc);
	FLEXT_ADDMETHOD_(0,"cosc",m_cosc);
	FLEXT_ADDMETHOD_(0,"noise",m_noise);
	FLEXT_ADDMETHOD_(0,"cnoise",m_cnoise);

	// Fourier transforms
	// real
	FLEXT_ADDMETHOD_(0,"rfft",m_rfft);
	FLEXT_ADDMETHOD_(0,"rifft",m_rifft);
//	FLEXT_ADDMETHOD_(0,"rfft-",m_rifft);
	// complex
	FLEXT_ADDMETHOD_(0,"cfft",m_cfft);
	FLEXT_ADDMETHOD_(0,"cifft",m_cifft);
//	FLEXT_ADDMETHOD_(0,"cfft-",m_cifft);
*/
}

vasp_tx::~vasp_tx()
{
}


I vasp_tx::m_set(I argc,t_atom *argv)
{
	vasp arg(argc,argv);

	if(!arg.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
	}
	else 
		ref = arg;

	return 0; 
}

