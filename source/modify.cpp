#include "main.h"

FLEXT_GIMME("vasp.tx",vasp_tx)

V vasp_tx::cb_setup(t_class *c)
{
	FLEXT_ADDMETHOD_G(c,"set",m_set);

	// assignment functions
	FLEXT_ADDMETHOD_G(c,"copy",m_copy);
	FLEXT_ADDMETHOD_G(c,"ccopy",m_ccopy);
	FLEXT_ADDMETHOD_G(c,"mcopy",m_mcopy);
//	FLEXT_ADDMETHOD_G(c,"mix",m_mix);
//	FLEXT_ADDMETHOD_G(c,"cmix",m_cmix);
//	FLEXT_ADDMETHOD_G(c,"mmix",m_mmix);

	// binary functions
	FLEXT_ADDMETHOD_G(c,"add",m_add);
	FLEXT_ADDMETHOD_G(c,"cadd",m_cadd);
	FLEXT_ADDMETHOD_G(c,"madd",m_madd);
	FLEXT_ADDMETHOD_G(c,"sub",m_sub);
	FLEXT_ADDMETHOD_G(c,"csub",m_csub);
	FLEXT_ADDMETHOD_G(c,"msub",m_msub);
	FLEXT_ADDMETHOD_G(c,"mul",m_mul);
	FLEXT_ADDMETHOD_G(c,"cmul",m_cmul);
	FLEXT_ADDMETHOD_G(c,"mmul",m_mmul);
	FLEXT_ADDMETHOD_G(c,"div",m_div);
	FLEXT_ADDMETHOD_G(c,"cdiv",m_cdiv);
	FLEXT_ADDMETHOD_G(c,"mdiv",m_mdiv);

	FLEXT_ADDMETHOD_G(c,"min",m_min);
//	FLEXT_ADDMETHOD_G(c,"cmin",m_cmin);
	FLEXT_ADDMETHOD_G(c,"mmin",m_mmin);
	FLEXT_ADDMETHOD_G(c,"max",m_max);
//	FLEXT_ADDMETHOD_G(c,"cmax",m_cmax);
	FLEXT_ADDMETHOD_G(c,"mmax",m_mmax);

	// unary functions
	FLEXT_ADDMETHOD_1(c,"pow",m_pow,F);
//	FLEXT_ADDMETHOD_G(c,"cpow",m_cpow);
	FLEXT_ADDMETHOD(c,"sqr",m_sqr);
	FLEXT_ADDMETHOD(c,"csqr",m_csqr);
	FLEXT_ADDMETHOD_1(c,"root",m_root,F);
	FLEXT_ADDMETHOD(c,"sqrt",m_sqrt);

	FLEXT_ADDMETHOD_G(c,"inv",m_inv);
	FLEXT_ADDMETHOD_G(c,"cinv",m_cinv);
	FLEXT_ADDMETHOD(c,"abs",m_abs);
	FLEXT_ADDMETHOD(c,"polar",m_polar);
	FLEXT_ADDMETHOD(c,"cart",m_cart);

	FLEXT_ADDMETHOD(c,"norm",m_norm);
	FLEXT_ADDMETHOD(c,"cnorm",m_cnorm);
//	FLEXT_ADDMETHOD(c,"opt",m_norm);
//	FLEXT_ADDMETHOD(c,"copt",m_cnorm);

	FLEXT_ADDMETHOD(c,"cswap",m_cswap);
	FLEXT_ADDMETHOD(c,"cconj",m_cconj);

	// Buffer rearrange functions
	FLEXT_ADDMETHOD_1(c,"shift",m_shift,F);
	FLEXT_ADDMETHOD_1(c,"xshift",m_xshift,F);
	FLEXT_ADDMETHOD_1(c,"rot",m_rot,F);
	FLEXT_ADDMETHOD_1(c,"xrot",m_xrot,F);
	FLEXT_ADDMETHOD(c,"mirr",m_mirr);
	FLEXT_ADDMETHOD(c,"xmirr",m_xmirr);
/*
	// Generator functions
	FLEXT_ADDMETHOD_G(c,"osc",m_osc);
	FLEXT_ADDMETHOD_G(c,"cosc",m_cosc);
	FLEXT_ADDMETHOD_G(c,"noise",m_noise);
	FLEXT_ADDMETHOD_G(c,"cnoise",m_cnoise);

	// Fourier transforms
	// real
	FLEXT_ADDMETHOD(c,"rfft",m_rfft);
	FLEXT_ADDMETHOD(c,"rifft",m_rifft);
//	FLEXT_ADDMETHOD(c,"rfft-",m_rifft);
	// complex
	FLEXT_ADDMETHOD(c,"cfft",m_cfft);
	FLEXT_ADDMETHOD(c,"cifft",m_cifft);
//	FLEXT_ADDMETHOD(c,"cfft-",m_cifft);
*/
}


vasp_tx::vasp_tx(I argc,t_atom *argv)
{
	m_set(argc,argv);

	add_in_def();
	add_out_anything();
	setup_inout();
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

