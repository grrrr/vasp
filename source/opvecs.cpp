/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file vasp__ctrl.cpp
	\brief Methods for handling of vector data for real, complex and multi-vector cases.

*/

#include "main.h"


//! return vector block for real data
RVecBlock *Vasp::GetRVecs(const C *op,Vasp &src)
{
	BL ok = true;
	RVecBlock *ret = new RVecBlock(src.Vectors());

	for(I ci = 0; ok && ci < src.Vectors(); ++ci) {
		VBuffer *bref = src.Buffer(ci);		
		if(!bref->Data()) {
			post("%s - dst vector (%s) is invalid",op,bref->Name());
			ok = false; // really return?
		}
		else 
			ret->Src(ci,bref,bref->Length());
	}

	if(ok) return ret;
	else { delete ret; return NULL;	}
}

CVecBlock *Vasp::GetCVecs(const C *op,Vasp &src)
{
	if(src.Vectors()%2 != 0) {
		post("%s - number of src vectors is odd - omitting last vector",op);
		// clear superfluous vector?
	}

	BL ok = true;
	I pairs = src.Vectors()/2;
	CVecBlock *ret = new CVecBlock(pairs);

	for(I ci = 0; ci < pairs; ++ci) {
		VBuffer *bre = src.Buffer(ci*2),*bim = src.Buffer(ci*2+1); // complex channels

		I frms = bre->Length();
		if(!bre->Data()) {
			post("%s - real dst vector (%s) is invalid",op,bre->Name());
			ok = false;
		}
		if(!bim->Data()) {
			post("%s - imag dst vector (%s) is invalid",op,bim->Name());
			ok = false;
		}
		else {
			if(frms != bim->Length()) {
				post("%s - real/imag dst vector length is not equal - using minimum",op);
				frms = min(frms,bim->Length());
			}
		}

		ret->Src(ci,bre,bim,frms);
	}

	if(ok) return ret;
	else { delete ret; return NULL;	}
}

RVecBlock *Vasp::GetRVecs(const C *op,Vasp &src,Vasp &arg,BL multi)
{
	if(!arg.Ok()) {
		post("%s - invalid argument vasp detected and ignored",op);
		return NULL;
	}

	I nvecs = src.Vectors();
	RVecBlock *ret;

	if(multi) {
		if(arg.Vectors() < nvecs) {
			nvecs = arg.Vectors();
			post("%s - too few argument vectors, operating on only first %i vectors",op,nvecs);
		}
		ret = new RVecBlock(nvecs,nvecs);
		for(I i = 0; i < nvecs; ++i) ret->Arg(i,arg.Buffer(i));
	}
	else {
		if(arg.Vectors() > 1) {
			post("%s - using only first argument vector for all operations",op);
		}
		ret = new RVecBlock(nvecs,1);
		ret->Arg(0,arg.Buffer(0));
	}

	BL ok = true;

	for(I ci = 0; ok && ci < nvecs; ++ci) {
		VBuffer *bref = src.Buffer(ci);	
		VBuffer *barg = ret->Arg(multi?ci:0);
		I frms = barg->Frames();

		if((multi || ci == 0) && !barg->Data()) {
			post("%s - src vector (%s) is invalid",op,barg->Name());
			ok = false; // really return?
		}
		else if(!bref->Data()) {
			post("%s - dst vector (%s) is invalid",op,bref->Name());
			ok = false; // really break?
		}
		else {
			if(frms != bref->Length()) {
				post("%s - src/dst vector length not equal - using minimum",op);
				frms = min(frms,bref->Length());
			}
		}
		ret->Src(ci,bref,frms);
	}

	if(ok) return ret;
	else { delete ret; return NULL;	}
}

CVecBlock *Vasp::GetCVecs(const C *op,Vasp &src,Vasp &arg,BL multi)
{
	if(!arg.Ok()) {
		post("%s - invalid argument vasp detected and ignored",op);
		return NULL;
	}

	BL ok = true;
	I pairs = src.Vectors()/2;
	CVecBlock *ret;

	if(multi) {
		I apairs = arg.Vectors()/2;
		if(arg.Vectors() != apairs*2) {
			post("%s - number of arg vectors is odd - assuming complex part as 0",op);
			++apairs;
		}

		if(apairs < pairs) {
			pairs = apairs;
			post("%s - too few argument vectors, operating on only first %i vector pairs",op,pairs);
		}
		ret = new CVecBlock(pairs,pairs);
		for(I i = 0; i < pairs; ++i) ret->Arg(i,arg.Buffer(i*2),arg.Buffer(i*2+1));
	}
	else {
		if(arg.Vectors() > 2) {
			post("%s - using only first argument vector pair for all operations",op);
		}
		ret = new CVecBlock(pairs,1);
		ret->Arg(0,arg.Buffer(0),arg.Buffer(1));
	}

/*
	if(!biarg) {
		post("%s - only one src vector - setting imaginary part to 0",op);					
	}
	else if(arg.Vectors() > 2) {
		post("%s - using only first two src vectors",op);
	}
*/
/*
	if(!brarg->Data()) {
		post("%s - real src vector (%s) is invalid",op,brarg->Name());
		ok = false;
	}
	else if(biarg && !biarg->Data()) {
		post("%s - imag src vector (%s) is invalid",op,biarg->Name());
		ok = false;
	}
	else 
*/
	{
//		I frms = brarg->Length();
/*
		if(biarg && frms != biarg->Length()) {
			post("%s - real/imag src vector length is not equal - using minimum",op);
			frms = min(frms,biarg->Length());
		}
*/
		if(src.Vectors() != pairs*2) {
			post("%s - number of dst vectors is odd - omitting last vector",op);
			// clear superfluous vector?
		}

		for(I ci = 0; ok && ci < pairs; ++ci) {
			VBuffer *brarg = ret->ReArg(ci),*biarg = ret->ImArg(ci);

			I frms = brarg?brarg->Length():0;

			if(multi || ci == 0) {
				if(!brarg->Data()) {
					post("%s - real src vector (%s) is invalid",op,brarg->Name());
					ok = false;
				}
				else if(biarg && !biarg->Data()) {
					post("%s - imag src vector (%s) is invalid",op,biarg->Name());
					ok = false;
				}
			}

			if(ok && biarg && frms != biarg->Length()) {
				if(multi || ci == 0) post("%s - real/imag src vector length is not equal - using minimum",op);
				frms = min(frms,biarg->Length());
			}

			VBuffer *brref = src.Buffer(ci*2),*biref = src.Buffer(ci*2+1);		

			if(ok) {
				if(!brref->Data()) {
					post("%s - real dst vector (%s) is invalid",op,brref->Name());
					ok = false; // really break?
				}
				else if(biref && !biref->Data()) {
					post("%s - imag dst vector (%s) is invalid",op,biref->Name());
					ok = false; // really break?
				}
				else {
					if(frms != brref->Length() || frms != biref->Length()) {
						post("%s - source/target vector length not equal - using minimum",op);
						frms = min(frms1,min(biref->Length(),brref->Length()));

						// clear rest?
					}
				}
			}

			ret->Src(ci,brref,biref,frms);
		}
	}

	if(ok) return ret;
	else { delete ret; return NULL;	}
}







// just transform complex vector pairs
Vasp *Vasp::fc_nop(const C *op,const Argument &arg,const nop_funcs &f)
{
	if(arg.CanbeComplex()) 
		return fc_nop(op,arg.GetAComplex(),f);
	else 
		return arg.IsNone()?new Vasp(*this):NULL;
}


// for real values or single real vector
Vasp *Vasp::fr_arg(const C *op,F v,argfunR f)
{
	BL ok = true;
	for(I ci = 0; ok && ci < this->Vectors(); ++ci) {
		VBuffer *bref = this->Buffer(ci);		
		if(!bref->Data()) {
			post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
			ok = false; // really return?
		}
		else {
			I chns = bref->Channels();
			ok &= f(bref->Length(),bref->Data()+bref->Offset()*chns+bref->Channel(),chns,v);
			bref->Dirty();
		}
		delete bref;
	}

	// output Vasp
	return ok?new Vasp(*this):NULL;
}

// for real values or single real vector (with parameters)
// This is exactly the same code a in the previous function!!!
Vasp *Vasp::fr_prm(const C *op,argfunRA f,Argument &p)
{
	BL ok = true;
	for(I ci = 0; ok && ci < this->Vectors(); ++ci) {
		VBuffer *bref = this->Buffer(ci);		
		if(!bref->Data()) {
			post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
			ok = false; // really return?
		}
		else {
			I chns = bref->Channels();
			ok &= f(bref->Length(),bref->Data()+bref->Offset()*chns+bref->Channel(),chns,p);
			bref->Dirty();
		}
		delete bref;
	}

	// output Vasp
	return ok?new Vasp(*this):NULL;
}

Vasp *Vasp::fr_arg(const C *op,const Vasp &arg,argfunV f)
{
	if(!arg.Ok()) {
		post("%s(%s) - invalid argument vasp detected and ignored",thisName(),op);
		return NULL;
	}

	if(arg.Vectors() > 1) {
		post("%s(%s) - using only first src vector",thisName(),op);
	}

	BL ok = true;

	VBuffer *barg = arg.Buffer(0);
	if(!barg->Data()) {
		post("%s(%s) - src vector (%s) is invalid",thisName(),op,barg->Name());
		ok = false; // really return?
	}
	else {
		I frms = barg->Frames();

		for(I ci = 0; ok && ci < this->Vectors(); ++ci) {
			VBuffer *bref = this->Buffer(ci);		

			if(!bref->Data()) {
				post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
				ok = false; // really break?
			}
			else {
				I frms1 = frms;
				if(frms1 != bref->Length()) {
					post("%s(%s) - src/dst vector length not equal - using minimum",thisName(),op);
					frms1 = min(frms1,bref->Length());

					// clear rest?
				}

				I achns = barg->Channels(),rchns = bref->Channels();
				ok &= f(frms1,bref->Data()+bref->Offset()*rchns+bref->Channel(),rchns,barg->Data()+barg->Offset()*achns+barg->Channel(),achns);
				bref->Dirty();
			}

			delete bref;
		}
	}

	delete barg;

	// output corrected Vasp
	return ok?new Vasp(*this):NULL;
}

// for real values or single real vector
Vasp *Vasp::fr_arg(const C *op,const Argument &arg,const arg_funcs &f)
{
	if(arg.CanbeFloat()) 
		return fr_arg(op,arg.GetAFloat(),f); // real input
	else if(arg.IsVasp()) 
		return fr_arg(op,arg.GetVasp(),f); // single vector source
	else
		return NULL;
}


// for complex values or complex vector pair
Vasp *Vasp::fc_arg(const C *op,const CX &cx,argfunC f)
{
	// complex (or real) input

	if(this->Vectors()%2 != 0) {
		post("%s(%s) - number of src vectors is odd - omitting last vector",thisName(),op);
		// clear superfluous vector?
	}

	BL ok = true;

	for(I ci = 0; ci < this->Vectors()/2; ++ci) {
		VBuffer *bre = this->Buffer(ci*2),*bim = this->Buffer(ci*2+1); // complex channels

		if(!bre->Data()) {
			post("%s(%s) - real dst vector (%s) is invalid",thisName(),op,bre->Name());
			ok = false;
		}
		if(!bim->Data()) {
			post("%s(%s) - imag dst vector (%s) is invalid",thisName(),op,bim->Name());
			ok = false;
		}
		else {
			I frms = bre->Length();
			if(frms != bim->Length()) {
				post("%s(%s) - real/imag dst vector length is not equal - using minimum",thisName(),op);
				frms = min(frms,bim->Length());

				// clear the rest?
			}

			ok &= f(frms,bre->Pointer(),bre->Channels(),bim->Pointer(),bim->Channels(),cx.real,cx.imag);
			bre->Dirty(); bim->Dirty();
		}

		delete bre;
		delete bim;
	}

	// output corrected vasp
	return ok?new Vasp(*this):NULL;
}

// for complex values or complex vector pair
// This is exactly the same code a in the previous function!!!
Vasp *Vasp::fc_prm(const C *op,argfunCA f,Argument &p)
{
	// complex (or real) input

	if(this->Vectors()%2 != 0) {
		post("%s(%s) - number of src vectors is odd - omitting last vector",thisName(),op);
		// clear superfluous vector?
	}

	BL ok = true;

	for(I ci = 0; ci < this->Vectors()/2; ++ci) {
		VBuffer *bre = this->Buffer(ci*2),*bim = this->Buffer(ci*2+1); // complex channels

		if(!bre->Data()) {
			post("%s(%s) - real dst vector (%s) is invalid",thisName(),op,bre->Name());
			ok = false;
		}
		if(!bim->Data()) {
			post("%s(%s) - imag dst vector (%s) is invalid",thisName(),op,bim->Name());
			ok = false;
		}
		else {
			I frms = bre->Length();
			if(frms != bim->Length()) {
				post("%s(%s) - real/imag dst vector length is not equal - using minimum",thisName(),op);
				frms = min(frms,bim->Length());

				// clear the rest?
			}

			I rchns = bre->Channels(),ichns = bim->Channels();
			ok &= f(frms,bre->Data()+bre->Offset()*rchns+bre->Channel(),rchns,bim->Data()+bim->Offset()*ichns+bim->Channel(),ichns,p);
			bre->Dirty(); bim->Dirty();
		}

		delete bre;
		delete bim;
	}

	// output corrected vasp
	return ok?new Vasp(*this):NULL;
}

// for complex values or complex vector pair
Vasp *Vasp::fc_arg(const C *op,const Vasp &arg,argfunCV f)
{
	if(!arg.Ok()) {
		post("%s(%s) - invalid argument vasp detected and ignored",thisName(),op);
		return NULL;
	}

	BL ok = true;
	VBuffer *brarg = arg.Buffer(0),*biarg = arg.Buffer(1);

	if(!biarg) {
		post("%s(%s) - only one src vector - setting imaginary part to 0",thisName(),op);					
	}
	else if(arg.Vectors() > 2) {
		post("%s(%s) - using only first two src vectors",thisName(),op);
	}

	if(!brarg->Data()) {
		post("%s(%s) - real src vector (%s) is invalid",thisName(),op,brarg->Name());
		ok = false;
	}
	else if(biarg && !biarg->Data()) {
		post("%s(%s) - imag src vector (%s) is invalid",thisName(),op,biarg->Name());
		ok = false;
	}
	else {
		I frms = brarg->Length();
		if(biarg && frms != biarg->Length()) {
			post("%s(%s) - real/imag src vector length is not equal - using minimum",thisName(),op);
			frms = min(frms,biarg->Length());
		}

		if(this->Vectors()%2 != 0) {
			post("%s(%s) - number of dst vectors is odd - omitting last vector",thisName(),op);
			// clear superfluous vector?
		}

		for(I ci = 0; ok && ci < this->Vectors()/2; ++ci) {
			VBuffer *brref = this->Buffer(ci*2),*biref = this->Buffer(ci*2+1);		

			if(!brref->Data()) {
				post("%s(%s) - real dst vector (%s) is invalid",thisName(),op,brref->Name());
				ok = false; // really break?
			}
			else if(biref && !biref->Data()) {
				post("%s(%s) - imag dst vector (%s) is invalid",thisName(),op,biref->Name());
				ok = false; // really break?
			}
			else {
				I frms1 = frms;
				if(frms1 != brref->Length() || frms1 != biref->Length()) {
					post("%s(%s) - source/target vector length not equal - using minimum",thisName(),op);
					frms1 = min(frms1,min(biref->Length(),brref->Length()));

					// clear rest?
				}

				I rchns = brref->Channels(),ichns = biref->Channels(),archns = brarg->Channels(),aichns = biarg->Channels();
				ok &= f(frms1,brref->Data()+brref->Offset()*rchns+brref->Channel(),rchns,biref->Data()+biref->Offset()*ichns+biref->Channel(),ichns,brarg->Data()+brarg->Offset()*archns+brarg->Channel(),archns,biarg->Data()+biarg->Offset()*aichns+biarg->Channel(),aichns);
				brref->Dirty(); biref->Dirty();
			}

			delete brref;
			delete biref;
		}
	}

	delete brarg;
	if(biarg) delete biarg;

	// output corrected!! Vasp
	return ok?new Vasp(*this):NULL;
}

Vasp *Vasp::fc_arg(const C *op,const Argument &arg,const arg_funcs &f)
{
	if(arg.CanbeComplex()) 
		return fc_arg(op,arg.GetAComplex(),f);
	else if(arg.IsVasp()) 
		return fc_arg(op,arg.GetVasp(),f); // pairs of vectors
	else
		return NULL;
}

// for multiple vectors
Vasp *Vasp::fv_arg(const C *op,const Vasp &arg,argfunV f)
{
	if(!arg.Ok()) {
		post("%s(%s) - invalid src Vasp",thisName(),op);
		return NULL;
	}

	if(!this->Ok()) {
		post("%s(%s) - invalid dst Vasp",thisName(),op);
		return NULL;
	}

	I vecs = this->Vectors();
	if(vecs != arg.Vectors()) {
		post("%s(%s) - unequal src/dst vector count - using minimum",thisName(),op);

		vecs = min(vecs,arg.Vectors());
		// clear the rest?
	}

	BL ok = true,warned = false;

	// do
	for(I ci = 0; ok && ci < vecs; ++ci) {
		VBuffer *bref = this->Buffer(ci),*barg = arg.Buffer(ci);

		if(!bref->Data()) {
			post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
			ok = false; // really return?
		}
		if(!barg->Data()) {
			post("%s(%s) - src vector (%s) is invalid",thisName(),op,barg->Name());
			ok = false; // really return?
		}
		else {
			I frms = bref->Length();
			if(frms != barg->Length()) {
				if(!warned) { // warn only once (or for each vector?) 
					post("%s(%s) - src/dst vector length not equal - using minimum",thisName(),op);
					warned = true;
				}

				frms = min(frms,barg->Length());
				// clear the rest?
			}

			I rchns = bref->Channels(),achns = barg->Channels();
			ok &= f(frms,bref->Data()+bref->Offset()*rchns+bref->Channel(),rchns,barg->Data()+barg->Offset()*achns+barg->Channel(),achns);
			bref->Dirty();
		}

		delete bref;
		delete barg;
	}

	// output corrected Vasp
	return ok?new Vasp(*this):NULL;
}

// for multiple vectors

Vasp *Vasp::fv_arg(const C *op,const Argument &arg,const arg_funcs &f)
{
	return arg.IsVasp()?fv_arg(op,arg.GetVasp(),f):NULL;
}

