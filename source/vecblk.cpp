/*

VASP modular - vector assembling signal processor / objects for Max and Pure Data

Copyright (c)2002-2015 Thomas Grill (gr@grrrr.org)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include "vecblk.h"


///////////////////////////////////////////////////////////////////////////
// VecBlock class
///////////////////////////////////////////////////////////////////////////

VecBlock::VecBlock(BL cx,I msrc,I mdst,I marg,I blarg):
	cplx(cx),asrc(msrc),barg(blarg),aarg(marg*blarg),adst(mdst)
{
	I i,all = asrc+aarg*blarg+adst;
	vecs = new VBuffer *[all];
	for(i = 0; i < all; ++i) vecs[i] = NULL;
}

VecBlock::~VecBlock()
{
	if(vecs) {
		I all = asrc+aarg*barg+adst;
		for(I i = 0; i < all; ++i) 
			if(vecs[i]) delete vecs[i];
		delete[] vecs;
	}
}

Vasp *VecBlock::_DstVasp(I n)
{
	Vasp *ret = new Vasp;
	ret->Frames(Frames());
	for(I i = 0; i < n; ++i) ret->AddVector(Vasp::Ref(*_Dst(i)));
	return ret;
}

Vasp *VecBlock::_SrcVasp(I n)
{
	Vasp *ret = new Vasp;
	ret->Frames(Frames());
	for(I i = 0; i < n; ++i) ret->AddVector(Vasp::Ref(*_Src(i)));
	return ret;
}

Vasp *VecBlock::_ResVasp(I n)
{
	return _Dst(0)?_DstVasp(n):_SrcVasp(n);
}



