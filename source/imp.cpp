/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


VASP_BINARY("vasp.=",copy)
VASP_BINARY("vasp.c=",ccopy)
VASP_BINARY("vasp.v=",vcopy)

VASP_BINARY("vasp.+",add)
VASP_BINARY("vasp.c+",cadd)
VASP_BINARY("vasp.v+",vadd)

VASP_BINARY("vasp.-",sub)
VASP_BINARY("vasp.c-",csub)
VASP_BINARY("vasp.v-",vsub)

VASP_BINARY("vasp.*",mul)
VASP_BINARY("vasp.c*",cmul)
VASP_BINARY("vasp.v*",vmul)

VASP_BINARY("vasp./",div)
VASP_BINARY("vasp.c/",cdiv)
VASP_BINARY("vasp.v/",vdiv)

VASP_BINARY("vasp.min",min)
VASP_BINARY("vasp.vmin",vmin)
VASP_BINARY("vasp.max",max)
VASP_BINARY("vasp.vmax",vmax)
VASP_UNARY("vasp.minmax",minmax) 

VASP_UNARY("vasp.int",int) 
VASP_UNARY("vasp.dif",dif) 
VASP_BINARY("vasp.peaks",peaks) 


VASP_UNARY("vasp.sqr",sqr) 
VASP_UNARY("vasp.ssqr",ssqr) 
VASP_UNARY("vasp.csqr",csqr) 
VASP_UNARY("vasp.sqrt",sqrt) 
VASP_UNARY("vasp.ssqrt",ssqrt) 
VASP_BINARY("vasp.pow",pow) 
VASP_BINARY("vasp.root",root) 

VASP_UNARY("vasp.exp",exp) 
VASP_UNARY("vasp.log",log) 

VASP_UNARY("vasp.inv",inv) 
VASP_UNARY("vasp.cinv",cinv) 

VASP_UNARY("vasp.abs",abs) 
VASP_UNARY("vasp.sign",sign) 
VASP_UNARY("vasp.polar",polar) 
VASP_UNARY("vasp.cart",cart) 

VASP_UNARY("vasp.norm",norm)
VASP_UNARY("vasp.cnorm",cnorm)

VASP_UNARY("vasp.cswap",cswap)
VASP_UNARY("vasp.cconj",cconj)

VASP_BINARY("vasp.shift",shift)
VASP_BINARY("vasp.xshift",xshift)
VASP_BINARY("vasp.rot",rot)
VASP_BINARY("vasp.xrot",xrot)
VASP_UNARY("vasp.mirr",mirr)
VASP_UNARY("vasp.xmirr",xmirr)

VASP_ANYOP("vasp.osc",osc)
VASP_ANYOP("vasp.*osc",mosc)
VASP_ANYOP("vasp.cosc",cosc)
VASP_ANYOP("vasp.*cosc",mcosc)
VASP_ANYOP("vasp.phasor",phasor)
VASP_ANYOP("vasp.*phasor",mphasor)
VASP_UNARY("vasp.noise",noise)
VASP_UNARY("vasp.cnoise",cnoise)
VASP_UNARY("vasp.bevel",bevelup)
VASP_UNARY("vasp.bevel-",beveldn)
VASP_UNARY("vasp.*bevel",mbevelup)
VASP_UNARY("vasp.*bevel-",mbeveldn)
VASP_BINARY("vasp.window",window)
VASP_BINARY("vasp.*window",mwindow)

VASP_ANYOP("vasp.flp",flp)
VASP_ANYOP("vasp.fhp",fhp)

VASP_ANYOP("vasp.tilt",tilt)
VASP_ANYOP("vasp.xtilt",xtilt)

VASP_UNARY("vasp.rfft",rfft)
VASP_UNARY("vasp.rfft-",rifft)
VASP_UNARY("vasp.cfft",cfft)
VASP_UNARY("vasp.cfft-",cifft)

