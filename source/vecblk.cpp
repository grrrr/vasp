#include "vecblk.h"
//#include <math.h>


///////////////////////////////////////////////////////////////////////////
// VBuffer class
///////////////////////////////////////////////////////////////////////////

VBuffer::VBuffer(t_symbol *s,I c,I l,I o): 
	chn(0),len(0),offs(0) 
{	
	Set(s,c,l,o); 
}

VBuffer::VBuffer(const VBuffer &v):
	chn(0),len(0),offs(0) 
{ 
	operator =(v); 
}


VBuffer::~VBuffer() {}

VBuffer &VBuffer::operator =(const VBuffer &v)
{
	return Set(v.Symbol(),v.Channel(),v.Length(),v.Offset());
}

VBuffer &VBuffer::Set(t_symbol *s,I c,I l,I o)
{
	parent::Set(s);

	chn = c;
	if(chn > Channels()) chn = Channels()-1; // simply correct the channel??
	offs = o;
	if(offs > Frames()) offs = Frames();
	len = l >= 0?l:Frames();
	if(offs+len > Frames()) len = Frames()-offs;

	return *this;
}


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
	for(I i = 0; i < n; ++i) *ret += Vasp::Ref(*_Dst(i));
	return ret;
}

Vasp *VecBlock::_SrcVasp(I n)
{
	Vasp *ret = new Vasp;
	ret->Frames(Frames());
	for(I i = 0; i < n; ++i) *ret += Vasp::Ref(*_Src(i));
	return ret;
}

Vasp *VecBlock::_ResVasp(I n)
{
	return _Dst(0)?_DstVasp(n):_SrcVasp(n);
}



