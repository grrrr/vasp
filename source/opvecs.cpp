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


/*! \brief Make real vector block for unary operations.

	\param op operation name
	\param src source vasp
	\param dst optional destination vasp
	\return struct with vector data 

	\todo Support dst vasp
*/
RVecBlock *VaspOp::GetRVecs(const C *op,Vasp &src,Vasp *dst)
{
	if(dst && dst->Ok()) {
		error("%s - sorry, out-of-place operation not supported yet",op);
		return NULL;
	}

	RVecBlock *ret = new RVecBlock(src.Vectors());

	BL ok = true,frlim = false;
	I tfrms = -1;

	for(I ci = 0; ok && ci < src.Vectors(); ++ci) {
		VBuffer *bref = src.Buffer(ci);		
		if(!bref->Data()) {
			post("%s - arg vector (%s) is invalid",op,bref->Name());
			ok = false; // really return?
		}
		else {
			ret->Src(ci,bref);
			I frms = bref->Length();

			if(tfrms < 0) tfrms = frms;
			else if(frms < tfrms) frlim = true,tfrms = frms;
		}
	}

	if(frlim) post("%s - vector length has been limited to maximum common length",op);

	ret->Frames(tfrms < 0?0:tfrms);

	if(ok) return ret;
	else { delete ret; return NULL;	}
}

/*! \brief Make real vector block for unary operations.

	\param op operation name
	\param src source vasp
	\param dst optional destination vasp
	\param full true if imaginary part is compulsory
	\return struct with vector data 

	\todo Support dst vasp
*/
CVecBlock *VaspOp::GetCVecs(const C *op,Vasp &src,Vasp *dst,BL full)
{
	if(dst && dst->Ok()) {
		error("%s - sorry, out-of-place operation not supported yet",op);
		return NULL;
	}

	I pairs = src.Vectors()/2;

	if(src.Vectors() != pairs*2) 
		if(full) {
			post("%s - number of src vectors is odd - not allowed",op);
			return NULL;
		}
		else {
			post("%s - number of src vectors is odd - omitting last vector",op);
		}

	CVecBlock *ret = new CVecBlock(pairs);
	BL ok = true,frlim = false;
	I tfrms = -1;

	for(I ci = 0; ci < pairs; ++ci) {
		VBuffer *bre = src.Buffer(ci*2),*bim = src.Buffer(ci*2+1); // complex channels

		I frms = bre->Length();
		if(!bre->Data()) {
			post("%s - real arg vector (%s) is invalid",op,bre->Name());
			ok = false;
		}
		if(!bim->Data()) {
			post("%s - imag arg vector (%s) is invalid",op,bim->Name());
			ok = false;
		}
		else {
			if(frms != bim->Length()) {
				post("%s - real/imag arg vector length is not equal - using minimum",op);
				frms = min(frms,bim->Length());
			}
		}

		if(tfrms < 0) tfrms = frms;
		else if(frms < tfrms) frlim = true,tfrms = frms;

		ret->Src(ci,bre,bim);
	}

	if(frlim) post("%s - vector length has been limited to maximum common length",op);

	ret->Frames(tfrms < 0?0:tfrms);

	if(ok) return ret;
	else { delete ret; return NULL;	}
}


/*! \brief Make real vector block for binary operations.

	\param op operation name
	\param src source vasp
	\param arg argument vasp
	\param dst optional destination vasp
	\param multi 0 off/1 on/-1 auto... controls whether argument vector is single- or multi-vectored
	\return struct with vector data 

	\todo Support dst vasp
*/
RVecBlock *VaspOp::GetRVecs(const C *op,Vasp &src,const Vasp &arg,Vasp *dst,I multi)
{
	if(dst && dst->Ok()) {
		error("%s - sorry, out-of-place operation not supported yet",op);
		return NULL;
	}

	if(!arg.Ok()) {
		post("%s - invalid argument vasp detected and ignored",op);
		return NULL;
	}

	I nvecs = src.Vectors();
	RVecBlock *ret;

	if(multi < 0) { // auto mode
		multi = arg.Vectors() > 1;
	}

	if(multi) {
		if(arg.Vectors() < nvecs) {
			nvecs = arg.Vectors();
			post("%s - too few arg vectors, operating on only first %i vectors",op,nvecs);
		}
		ret = new RVecBlock(nvecs,nvecs);
		for(I i = 0; i < nvecs; ++i) ret->Arg(i,arg.Buffer(i));
	}
	else {
		if(arg.Vectors() > 1) {
			post("%s - using only first arg vector for all operations",op);
		}
		ret = new RVecBlock(nvecs,1);
		ret->Arg(0,arg.Buffer(0));
	}

	BL ok = true,frlim = false;
	I tfrms = -1;

	for(I ci = 0; ok && ci < nvecs; ++ci) {
		VBuffer *bref = src.Buffer(ci);	
		VBuffer *barg = ret->Arg(multi?ci:0);
		I frms = barg->Frames();

		if((multi || ci == 0) && !barg->Data()) {
			post("%s - src vector (%s) is invalid",op,barg->Name());
			ok = false; // really return?
		}
		else if(!bref->Data()) {
			post("%s - arg vector (%s) is invalid",op,bref->Name());
			ok = false; // really break?
		}
		else {
			if(frms != bref->Length()) {
				post("%s - src/arg vector length not equal - using minimum",op);
				frms = min(frms,bref->Length());
			}
		}

		if(tfrms < 0) tfrms = frms;
		else if(frms < tfrms) frlim = true,tfrms = frms;

		ret->Src(ci,bref);
	}

	if(frlim) post("%s - vector length has been limited to maximum common length",op);

	ret->Frames(tfrms < 0?0:tfrms);

	if(ok) return ret;
	else { delete ret; return NULL;	}
}


/*! \brief Make real complex block for binary operations.

	\param op operation name
	\param src source vasp
	\param arg argument vasp
	\param dst optional destination vasp
	\param multi 0 off/1 on/-1 auto... controls whether argument vector is single- or multi-vectored
	\param full true if imaginary part is compulsory
	\return struct with vector data 

	\todo Support dst vasp
*/
CVecBlock *VaspOp::GetCVecs(const C *op,Vasp &src,const Vasp &arg,Vasp *dst,I multi,BL full)
{
	if(dst && dst->Ok()) {
		error("%s - sorry, out-of-place operation not supported yet",op);
		return NULL;
	}

	if(!arg.Ok()) {
		post("%s - invalid argument vasp detected and ignored",op);
		return NULL;
	}

	I pairs = src.Vectors()/2;
	CVecBlock *ret;

	if(multi < 0) { // auto mode
		multi = arg.Vectors() > 2; // more than one argument pair -> multi
	}

	if(multi) {
		I apairs = arg.Vectors()/2;
		if(arg.Vectors() != apairs*2) 
			if(full) {
				post("%s - number of arg vectors is odd - not allowed",op);
				return NULL;
			}
			else {
				post("%s - number of arg vectors is odd - assuming complex part as 0",op);
				++apairs;
			}

		if(apairs < pairs) {
			pairs = apairs;
			post("%s - too few arg vectors, operating on only first %i vector pairs",op,pairs);
		}
		ret = new CVecBlock(pairs,pairs);
		for(I i = 0; i < pairs; ++i) ret->Arg(i,arg.Buffer(i*2),arg.Buffer(i*2+1));
	}
	else {
		if(arg.Vectors() > 2) {
			post("%s - using only first arg vector pair for all operations",op);
		}
		ret = new CVecBlock(pairs,1);
		ret->Arg(0,arg.Buffer(0),arg.Buffer(1));
	}

	BL ok = true,frlim = false;
	I tfrms = -1;
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
			post("%s - number of src vectors is odd - omitting last vector",op);
			// clear superfluous vector?
		}

		for(I ci = 0; ok && ci < pairs; ++ci) {
			VBuffer *brarg = ret->ReArg(ci),*biarg = ret->ImArg(ci);

			I frms = brarg?brarg->Length():0;

			if(multi || ci == 0) {
				if(!brarg->Data()) {
					post("%s - real arg vector (%s) is invalid",op,brarg->Name());
					ok = false;
				}
				else if(biarg && !biarg->Data()) {
					post("%s - imag arg vector (%s) is invalid",op,biarg->Name());
					ok = false;
				}
			}

			if(ok && biarg && frms != biarg->Length()) {
				if(multi || ci == 0) post("%s - real/imag arg vector length is not equal - using minimum",op);
				frms = min(frms,biarg->Length());
			}

			VBuffer *brref = src.Buffer(ci*2),*biref = src.Buffer(ci*2+1);		

			if(ok) {
				if(!brref->Data()) {
					post("%s - real src vector (%s) is invalid",op,brref->Name());
					ok = false; // really break?
				}
				else if(biref && !biref->Data()) {
					post("%s - imag src vector (%s) is invalid",op,biref->Name());
					ok = false; // really break?
				}
				else {
					if(frms != brref->Length() || frms != biref->Length()) {
						post("%s - src/arg vector length not equal - using minimum",op);
						frms = min(frms,min(biref->Length(),brref->Length()));

						// clear rest?
					}
				}
			}
			if(tfrms < 0) tfrms = frms;
			else if(frms < tfrms) frlim = true,tfrms = frms;

			ret->Src(ci,brref,biref);
		}
	}

	if(frlim) post("%s - vector length has been limited to maximum common length",op);

	ret->Frames(tfrms < 0?0:tfrms);

	if(ok) return ret;
	else { delete ret; return NULL;	}
}


/*! \brief Run the operation on the various real vectors.

	\param vecs src/arg/dst vector block
	\param fun operative function
	\param p parameter block for operative function
	\return normalized vasp or NULL on error

	\todo set overlap flag

	\remark operative function must be capable of handling reversed direction
*/
Vasp *VaspOp::DoOp(RVecBlock *vecs,VecOp::opfun *fun,OpParam &p,BL symm)
{
	BL ok = true;
	const I scnt = symm?2:1;
	for(I i = 0; ok && i < vecs->Vecs(); ++i) 
		for(I si = 0; ok && si < scnt; ++si) {
			p.frames = vecs->Frames();

			VBuffer *s = vecs->Src(i),*d = vecs->Dst(i),*a = vecs->Arg(i);
			p.rsdt = s->Pointer(),p.rss = s->Channels();
		
			if(d) p.rddt = d->Pointer(),p.rds = d->Channels();
			else p.rddt = p.rsdt,p.rds = p.rss;
		
			if(a) p.radt = a->Pointer(),p.ras = a->Channels();
			else p.radt = NULL; //,p.ras = 0;

			if(!symm) 
				p.symm = -1;
			else {
				const I hcnt = p.frames/2;
				if((p.symm = si) == 0) {
					p.frames = hcnt; 
				}
				else {
					const I r = p.frames-hcnt;
					p.frames = hcnt;
					p.rsdt += r*p.rss,p.rddt += r*p.rds;
					if(p.radt) p.radt += r*p.ras;
				}
			}

			{	// ---- Check out and try to resolve overlap situation ------------

				BL sovr = p.SR_In(); // check whether dst is before src 
				if(p.HasArg()) { 
					// has argument
					if(sovr) {
						// src/dst needs reversal -> check if ok for arg/dst
						if(p.AR_Can()) 
							p.R_Rev(); // Revert vectors
						else {
							post("%s - vector overlap situation can't be resolved",p.opname);
							ok = false;
						}
					}
					else if(p.AR_In()) { 
						// arg/dst needs reversal -> check if ok for src/dst

						if(p.SR_Can()) 
							p.R_Rev(); // Revert vectors
						else {
							post("%s - vector overlap situation can't be resolved",p.opname);
							ok = false;
						}
					}
				}
				else { // No arg
					if(sovr) p.R_Rev(); // if overlapping revert vectors
				}
			}

			ok = fun(p);
		}
	return ok?vecs->ResVasp():NULL;
}


/*! \brief Run the operation on the various complex vector pairs.

	\param vecs src/arg/dst vector block
	\param fun operative function
	\param p parameter block for operative function
	\return normalized vasp or NULL on error

	\todo set overlap flag

	\remark operative function must be capable of handling reversed direction
*/
Vasp *VaspOp::DoOp(CVecBlock *vecs,VecOp::opfun *fun,OpParam &p,BL symm)
{
	BL ok = true;
	const I scnt = symm?2:1;
	for(I i = 0; ok && i < vecs->Pairs(); ++i) 
		for(I si = 0; ok && si < scnt; ++si) {
			p.frames = vecs->Frames();
		
			VBuffer *rsv = vecs->ReSrc(i),*isv = vecs->ImSrc(i);
			p.rsdt = rsv->Pointer(),p.rss = rsv->Channels();
			p.isdt = isv->Pointer(),p.iss = isv->Channels();

			VBuffer *rdv = vecs->ReDst(i),*idv = vecs->ImDst(i);
			if(rdv) {
				p.rddt = rdv->Pointer(),p.rds = rdv->Channels();
				if(idv) p.iddt = idv->Pointer(),p.ids = idv->Channels();
				else p.iddt = NULL; //,p.ids = 0; // Can that be NULL??
			}
			else { 
				p.rddt = p.rsdt,p.rds = p.rss,p.iddt = p.isdt,p.ids = p.iss;
			}
			
			VBuffer *rav = vecs->ReArg(i),*iav = vecs->ImArg(i);
			if(rav) {
				p.radt = rav->Pointer(),p.rds = rav->Channels();
				if(iav) p.iadt = iav->Pointer(),p.ias = iav->Channels();
				else p.iadt = NULL; //,p.ias = 0;
			}
			else { 
				p.radt = NULL,p.iadt = NULL;
			}

			if(!symm) 
				p.symm = -1;
			else {
				const I hcnt = p.frames/2;
				if((p.symm = si) == 0) {
					p.frames = hcnt; 
				}
				else {
					const I r = p.frames-hcnt;
					p.frames = hcnt;
					p.rsdt += r*p.rss,p.isdt += r*p.iss;
					p.rddt += r*p.rds;
					if(p.iddt) p.iddt += r*p.ids; // Can that be NULL??
					if(p.radt) p.radt += r*p.ras;
					if(p.iadt) p.iadt += r*p.ias;
				}
			}

			{	// ---- Check out and try to resolve overlap situation ------------

				BL sovr = p.SR_In(); // check whether dst is before src 
				if(sovr && !p.SI_Can()) {
					post("%s - src/dst overlap of re/im vectors not resolvable",p.opname);			
					ok = false;
				}

				if(ok && p.HasArg()) { 
					// has argument
					if(sovr) {
						// src/dst needs reversal -> check if ok for arg/dst

						if(p.AR_Can() && p.AI_Can()) 
							p.C_Rev(); // Revert vectors
						else {
							post("%s - vector overlap situation can't be resolved",p.opname);
							ok = false;
						}
					}
					else if(p.AR_In() || p.AI_In()) { 
						// arg/dst needs reversal -> check if ok for src/dst

						if(p.AR_Can() && p.AI_Can() && p.SR_Can() && p.SI_Can()) 
							p.C_Rev(); // Revert vectors
						else {
							post("%s - vector overlap situation can't be resolved",p.opname);
							ok = false;
						}
					}
				}
				else { // No arg
					if(sovr) p.C_Rev(); // if overlapping revert vectors
				}
			}

			ok = fun(p);
		}
	return ok?vecs->ResVasp():NULL;
}



