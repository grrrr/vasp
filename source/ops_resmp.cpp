/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


// --- resample ---------------------

static BL d_tilt(I cnt,F *dt,I str,Argument &a) 
{ 
	F factor = a.GetFloat();
	F center = a.Next(1).GetAFloat();
	I icenter = (I)center;

	if(cnt <= 1 || factor == 1) return true;

	if(factor == 0) {
		post("tilt: invalid factor value");
		return false;
	}

	if(center != icenter)
		post("tilt: center position truncated to integer sample");

	if(icenter < 0) {
		post("tilt: center position < 0 -> set to 0");
		icenter = 0;
	}
	else if(icenter >= cnt) {
		post("tilt: center position >= buffer length -> set to max. position");
		icenter = cnt;
	}

	// !lacking interpolation
	if(factor > 1) {
		I i;
//		I pl = (I)(icenter/factor),ql = (I)((cnt-icenter)/factor);
		for(i = 1; i <= icenter; ++i) {
			F sp = icenter-i*factor;
			dt[(icenter-i)*str] = sp >= 0 && sp < cnt?dt[(I)sp*str]:0;
		}
		for(i = 1; i < cnt-icenter; ++i) {
			F sp = icenter+i*factor;
			dt[(icenter+i)*str] = sp >= 0 && sp < cnt?dt[(I)sp*str]:0;
		}
	}
	else {
		I i;
		for(i = icenter; i > 0; --i) {
			F sp = icenter-i*factor;
			dt[(icenter-i)*str] = dt[(I)sp*str];
		}
		for(i = cnt-1-icenter; i > 0; --i) {
			F sp = icenter+i*factor;
			dt[(icenter+i)*str] = dt[(I)sp*str];
		}
	}

	return true;
}

static BL d_xtilt(I cnt,F *dt,I str,Argument &a) 
{ 
	F factor = a.GetFloat();
	F center = a.Next(1).GetAFloat();

	BL ok = true;
	{
		Argument params;
		params.Set(factor).Add(center);
		ok = ok && d_tilt(cnt/2,dt,str,params);
	}
	if(ok) {
		Argument params;
		params.Set(factor).Add(cnt/2-1-center);
		ok = ok && d_tilt(cnt/2,dt+(cnt-cnt/2)*str,str,params);
	}
	return ok;
}



Vasp *Vasp::m_tilt(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):1);
		return fr_prm("tilt",d_tilt,params); 
	}
	else return NULL;
}

Vasp *Vasp::m_xtilt(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):1);
		return fr_prm("xtilt",d_xtilt,params); 
	}
	else return NULL;
}

