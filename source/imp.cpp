/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

VASP_BINARY("vasp.=",copy)
VASP_BINARY("vasp.c=",ccopy)
VASP_BINARY("vasp.m=",mcopy)

VASP_BINARY("vasp.+",add)
VASP_BINARY("vasp.c+",cadd)
VASP_BINARY("vasp.m+",madd)

VASP_BINARY("vasp.-",sub)
VASP_BINARY("vasp.c-",csub)
VASP_BINARY("vasp.m-",msub)

VASP_BINARY("vasp.*",mul)
VASP_BINARY("vasp.c*",cmul)
VASP_BINARY("vasp.m*",mmul)

VASP_BINARY("vasp./",div)
VASP_BINARY("vasp.c/",cdiv)
VASP_BINARY("vasp.m/",mdiv)

VASP_UNARY("vasp.abs",abs) 
VASP_UNARY("vasp.sign",sign) 
VASP_UNARY("vasp.polar",polar) 
VASP_UNARY("vasp.cart",cart) 

VASP_UNARY("vasp.norm",norm)
VASP_UNARY("vasp.cnorm",cnorm)

VASP_UNARY("vasp.cswap",cswap)
VASP_UNARY("vasp.cconj",cconj)


