/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


VASP_BINARY("vasp.=",copy,false)
VASP_BINARY("vasp.c=",ccopy,false)
//VASP_BINARY("vasp.v=",vcopy)

VASP_BINARY("vasp.+",add,true)
VASP_BINARY("vasp.c+",cadd,true)
//VASP_BINARY("vasp.v+",vadd)

VASP_BINARY("vasp.-",sub,true)
VASP_BINARY("vasp.c-",csub,true)
//VASP_BINARY("vasp.v-",vsub)

VASP_BINARY("vasp.*",mul,true)
VASP_BINARY("vasp.c*",cmul,true)
//VASP_BINARY("vasp.v*",vmul)

VASP_BINARY("vasp./",div,true)
VASP_BINARY("vasp.c/",cdiv,true)
//VASP_BINARY("vasp.v/",vdiv)

VASP_BINARY("vasp.min",min,true)
//VASP_BINARY("vasp.vmin",vmin)
VASP_BINARY("vasp.max",max,true)
//VASP_BINARY("vasp.vmax",vmax)
VASP_UNARY("vasp.minmax",minmax,true) 

VASP_BINARY("vasp.int",int,true) 
VASP_BINARY("vasp.dif",dif,true) 
VASP_BINARY("vasp.peaks",peaks,true) 


VASP_UNARY("vasp.sqr",sqr,true) 
VASP_UNARY("vasp.ssqr",ssqr,true) 
VASP_UNARY("vasp.csqr",csqr,true) 
VASP_UNARY("vasp.sqrt",sqrt,true) 
VASP_UNARY("vasp.ssqrt",ssqrt,true) 
VASP_BINARY("vasp.pow",pow,true) 
VASP_BINARY("vasp.root",root,true) 

VASP_UNARY("vasp.exp",exp,true) 
VASP_UNARY("vasp.log",log,true) 

VASP_UNARY("vasp.inv",inv,true) 
VASP_UNARY("vasp.cinv",cinv,true) 

VASP_UNARY("vasp.abs",abs,true) 
VASP_UNARY("vasp.sign",sign,true) 
VASP_UNARY("vasp.polar",polar,true) 
VASP_UNARY("vasp.cart",cart,true) 

VASP_UNARY("vasp.opt",opt,true)
VASP_UNARY("vasp.copt",copt,true)
VASP_UNARY("vasp.cnorm",cnorm,true)

VASP_UNARY("vasp.cswap",cswap,true)
VASP_UNARY("vasp.cconj",cconj,true)

VASP_BINARY("vasp.shift",shift,true)
VASP_BINARY("vasp.xshift",xshift,true)
VASP_BINARY("vasp.rot",rot,true)
VASP_BINARY("vasp.xrot",xrot,true)
VASP_UNARY("vasp.mirr",mirr,true)
VASP_UNARY("vasp.xmirr",xmirr,true)

VASP_ANYOP("vasp.osc",osc,false)
VASP_ANYOP("vasp.*osc",mosc,true)
VASP_ANYOP("vasp.cosc",cosc,false)
VASP_ANYOP("vasp.*cosc",mcosc,true)
VASP_ANYOP("vasp.phasor",phasor,false)
VASP_ANYOP("vasp.*phasor",mphasor,true)
VASP_UNARY("vasp.noise",noise,false)
VASP_UNARY("vasp.cnoise",cnoise,false)
VASP_UNARY("vasp.bevel",bevelup,false)
VASP_UNARY("vasp.bevel-",beveldn,false)
VASP_UNARY("vasp.*bevel",mbevelup,true)
VASP_UNARY("vasp.*bevel-",mbeveldn,false)
VASP_BINARY("vasp.window",window,false)
VASP_BINARY("vasp.*window",mwindow,true)

VASP_ANYOP("vasp.flp",flp,true)
VASP_ANYOP("vasp.fhp",fhp,true)

VASP_ANYOP("vasp.tilt",tilt,true)
VASP_ANYOP("vasp.xtilt",xtilt,true)

VASP_UNARY("vasp.rfft",rfft,true)
VASP_UNARY("vasp.rfft-",rifft,true)
VASP_UNARY("vasp.cfft",cfft,true)
VASP_UNARY("vasp.cfft-",cifft,true)

