/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


//VASP_UNARY("vasp.=",copy,false)
//VASP_UNARY("vasp.c=",ccopy,false)
VASP_BINARY("vasp.=",set,false)
VASP_BINARY("vasp.c=",cset,false)

VASP_BINARY("vasp.+",add,true)
VASP_BINARY("vasp.c+",cadd,true)
VASP_BINARY("vasp.-",sub,true)
VASP_BINARY("vasp.c-",csub,true)
VASP_BINARY("vasp.!-",subr,true)
VASP_BINARY("vasp.c!-",csubr,true)
VASP_BINARY("vasp.*",mul,true)
VASP_BINARY("vasp.c*",cmul,true)
VASP_BINARY("vasp./",div,true)
VASP_BINARY("vasp.c/",cdiv,true)
VASP_BINARY("vasp.!/",divr,true)
VASP_BINARY("vasp.c!/",cdivr,true)
VASP_BINARY("vasp.%",mod,true)

VASP_BINARY("vasp.min",min,true)
VASP_BINARY("vasp.rmin",rmin,true)
VASP_BINARY("vasp.max",max,true)
VASP_BINARY("vasp.rmax",rmax,true)

VASP_UNARY("vasp.minmax",minmax,true) 

VASP_BINARY("vasp.<",lwr,true)
VASP_BINARY("vasp.>",gtr,true)
VASP_BINARY("vasp.<=",leq,true)
VASP_BINARY("vasp.>=",geq,true)
VASP_BINARY("vasp.==",equ,true)
VASP_BINARY("vasp.!=",neq,true)

VASP_ANYOP("vasp.int",int,0,true) 
VASP_ANYOP("vasp.dif",dif,0,true) 

VASP_ANYOP("vasp.gate",gate,1,true) 
//VASP_ANYOP("vasp.!gate",igate,1,true) 
VASP_ANYOP("vasp.rgate",rgate,1,true) 
//VASP_ANYOP("vasp.r!gate",rigate,1,true) 

VASP_UNARY("vasp.sqr",sqr,true) 
VASP_UNARY("vasp.ssqr",ssqr,true) 
VASP_UNARY("vasp.csqr",csqr,true) 
VASP_UNARY("vasp.sqrt",sqrt,true) 
VASP_UNARY("vasp.ssqrt",ssqrt,true) 
VASP_BINARY("vasp.pow",pow,true) 
VASP_ANYOP("vasp.cpowi",cpowi,0,true) 
VASP_ANYOP("vasp.rpow",rpow,1,true) 
VASP_ANYOP("vasp.r+",radd,1,true) 

VASP_UNARY("vasp.exp",exp,true) 
VASP_UNARY("vasp.log",log,true) 

VASP_UNARY("vasp.inv",inv,true) 
VASP_UNARY("vasp.cinv",cinv,true) 

VASP_UNARY("vasp.abs",abs,true) 
VASP_UNARY("vasp.sign",sign,true) 
VASP_UNARY("vasp.polar",polar,true) 
VASP_UNARY("vasp.rect",rect,true) 

VASP_UNARY("vasp.cnorm",cnorm,true)
//VASP_UNARY("vasp.cswap",cswap,true)
VASP_UNARY("vasp.cconj",cconj,true)

VASP_ANYOP("vasp.shift",shift,0,true)
VASP_ANYOP("vasp.xshift",xshift,0,true)
VASP_ANYOP("vasp.rot",rot,0,true)
VASP_ANYOP("vasp.xrot",xrot,0,true)
VASP_UNARY("vasp.mirr",mirr,true)
VASP_UNARY("vasp.xmirr",xmirr,true)

VASP_ANYOP("vasp.osc",osc,1,false)
VASP_ANYOP("vasp.*osc",mosc,1,true)
VASP_ANYOP("vasp.cosc",cosc,1,false)
VASP_ANYOP("vasp.*cosc",mcosc,1,true)
VASP_ANYOP("vasp.phasor",phasor,1,false)
VASP_ANYOP("vasp.*phasor",mphasor,1,true)
VASP_UNARY("vasp.noise",noise,false)
VASP_UNARY("vasp.cnoise",cnoise,false)
VASP_UNARY("vasp.bevel",bevelup,false)
VASP_UNARY("vasp.!bevel",beveldn,false)
VASP_UNARY("vasp.*bevel",mbevelup,true)
VASP_UNARY("vasp.*!bevel",mbeveldn,false)
VASP_ANYOP("vasp.window",window,0,false)
VASP_ANYOP("vasp.*window",mwindow,0,true)

VASP_ANYOP("vasp.flp",flp,1,true)
VASP_ANYOP("vasp.fhp",fhp,1,true)

VASP_ANYOP("vasp.tilt",tilt,1,true)
VASP_ANYOP("vasp.xtilt",xtilt,1,true)

VASP_UNARY("vasp.rfft",rfft,true)
VASP_UNARY("vasp.r!fft",rifft,true)
VASP_UNARY("vasp.cfft",cfft,true)
VASP_UNARY("vasp.c!fft",cifft,true)

