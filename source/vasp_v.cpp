/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file vasp_v.cpp
	\brief Definitions of basic vasp objects.
*/


#include "main.h"

/*! \class vasp_v 
	\remark \b vasp
	\brief Stores vasp messages.
	\since 0.0.1
	\param cmdln.* vasp - to be stored
	\param inlet vasp - is stored and output
	\param inlet list - if possible list is converted to vasp format
	\param inlet bang - triggers stored Vasp output
	\param inlet set - Vasp is stored (and not immediately output)
	\retval outlet vasp
*/
class vasp_v:
	public vasp_tx
{
	FLEXT_HEADER(vasp_v,vasp_tx)

public:

	vasp_v(I argc,t_atom *argv)
	{
		m_set(argc,argv);

		AddInAnything();
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD_(0,"list",m_vasp);
	}

	virtual Vasp *x_work() { return new Vasp(ref); }

	virtual V m_help() { post("%s - Store and output a vasp",thisName()); }
};

FLEXT_LIB_G("vasp",vasp_v)



/*! \class vasp_m 
	\remark \b vasp.m
	\brief Outputs multiple (identical) vasps.
	\since 0.0.1
	\param cmdln.1 int - number of vasp outlets
	\param inlet vasp - is stored and output
	\param inlet bang - triggers stored Vasp output
	\param inlet set - vasp to be stored (and not immediately output)
	\retval outlet.* vasp

	\note Outputs in right to left order.
*/
class vasp_m:
	public vasp_op
{
	FLEXT_HEADER(vasp_m,vasp_op)

public:

	vasp_m(I n)
	{
		AddInAnything();
		AddOutAnything(n);
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		Vasp r(ref); 
		for(I i = CntOut()-1; i >= 0; --i) ToOutVasp(i,r);
	}

	virtual V m_help() { post("%s - Output a vasp multiply",thisName()); }
};

FLEXT_LIB_1("vasp.m",vasp_m,I)



/*! \class vasp_update
	\remark \b vasp.update
	\brief Refreshes buffer graphics for a vasp.
	\since 0.0.1
	\param inlet vasp - is stored and output
	\param inlet bang - triggers stored vasp output
	\param inlet set - vasp to be stored (and not immediately output)
	\retval outlet vasp

	\note In MaxMSP only necessary when buffer is in another window.
*/
class vasp_update:
	public vasp_tx
{
	FLEXT_HEADER(vasp_update,vasp_tx)

public:
	vasp_update()
	{
		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual Vasp *x_work() { ref.Refresh(); return new Vasp(ref); }

	virtual V m_help() { post("%s - Update graphics of a vasp",thisName()); }
};

FLEXT_LIB("vasp.update",vasp_update)



/*! \class vasp_chk
	\remark \b vasp.chk
	\brief Check vasp dimensions.
	\since 0.0.1
	\param inlet vasp - is stored and output
	\param inlet bang - triggers stored vasp output
	\param inlet set - vasp to be stored (and not immediately output)
	\retval outlet vasp

	\remark checks and corrects frame count
	\remark checks channel index... no correction, no output on error!
*/
class vasp_chk:
	public vasp_tx
{
	FLEXT_HEADER(vasp_chk,vasp_tx)

public:
	vasp_chk()
	{
		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		I fr = ret->ChkFrames(); // maximum common frame length
		ret->Frames(fr);

		BL chok = true;

		for(I i = 0; i < ret->Vectors(); ++i) {
			VBuffer *buf = ret->Buffer(i);
			chok = chok && buf->Ok() && buf->Channel() == ret->Vector(i).Channel();
			delete buf;
		}

		if(chok) 
			return ret;
		else {
			delete ret;
			return NULL;
		}
	}

	virtual V m_help() { post("%s - Check vasp dimensions",thisName()); }
};

FLEXT_LIB("vasp.chk",vasp_chk)



/*! \class vasp_n
	\remark \b vasp.n
	\brief Gets indexed vector of a vasp.
	\since 0.0.1
	\param cmdln.1 int - index of vasp vector
	\param inlet vasp - is stored and indexed vasp vector output
	\param inlet bang - triggers indexed vasp vector output
	\param inlet set - vasp to be stored (and not immediately output)
	\retval outlet.1 vasp - single indexed vector of vasp
	\retval outlet.2 vasp - remainder of vasp

	\note Outputs only on valid index
	\todo Output remainder as vasp.
*/
class vasp_n:
	public vasp_tx
{
	FLEXT_HEADER(vasp_n,vasp_tx)

public:
	vasp_n(I argc,t_atom *argv):
		ix(0)
	{
		if(argc >= 1 && CanbeInt(argv[0]))
			ix = GetAInt(argv[0]);
		else if(argc)
			post("%s - Index argument invalid -> set to 0",thisName());

		AddInAnything(2);
		AddOutAnything(2);
		SetupInOut();

		FLEXT_ADDMETHOD(1,m_ix);
	}

	V m_ix(I i) { ix = i; }

	virtual Vasp *x_work() { return ix < ref.Vectors()?new Vasp(ref.Frames(),ref.Vector(ix)):NULL; }

	virtual V m_help() { post("%s - Get one vector of a vasp",thisName()); }

protected:
	I ix;

private:
	FLEXT_CALLBACK_I(m_ix);
};

FLEXT_LIB_G("vasp.n",vasp_n)



/*! \class vasp_qn 
	\remark \b vasp.n?
	\brief Gets number of vector of a vasp.
	\since 0.0.1
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet int - number of vectors in stored vasp

	\note Outputs 0 if vasp is undefined or invalid.

	\todo Should we disable output with invalid vasp?
*/
class vasp_qn:
	public vasp_op
{
	FLEXT_HEADER(vasp_qn,vasp_op)

public:
	vasp_qn()
	{
		AddInAnything();
		AddOutInt();
		SetupInOut();
	}

	virtual V m_bang() { ToOutInt(0,ref.Ok()?ref.Vectors():0); }

	virtual V m_help() { post("%s - Get number of vectors of a vasp",thisName()); }
};

FLEXT_LIB("vasp.n?",vasp_qn)




/*! \class vasp_sync
	\remark \b vasp.sync
	\brief Waits for all inlets to be hit (by vasps/anything) to trigger output.
	\since 0.0.1
	\param cmdln.1 int - number of sync inlets
	\param inlet.1 vasp - is stored 
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
	\param inlet.1 reset - clear all hit flags
	\param inlet.+n vasp/anything - sets hit flag
	\retval outlet vasp - stored vasp 

	\todo Message for selection if only vasp input triggers (or any one).
	\todo Message for selection of manual or auto reset upon trigger
*/
class vasp_sync:
	public vasp_op
{
	FLEXT_HEADER(vasp_sync,vasp_op)

public:
	vasp_sync(I n):
		flags(new BL[n]),
		autoreset(true),vasponly(false)
	{
		AddInAnything(n);
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD_(0,"reset",m_reset);

		m_reset();
	}

	~vasp_sync()	{ if(flags) delete[] flags; }

	V chkbang(I n) 
	{
		BL f = flags[n]; 
		flags[n] = true; 
		if(!f) { // flags have changed
			
			BL all = true;
			for(I i = 0; i < CntIn(); ++i) all = all && flags[i];

			if(all) {
				if(ref.Ok()) ToOutVasp(0,ref);
				else ToOutBang(0);

				if(autoreset) m_reset();
			}
		}
	}

	virtual V m_bang() { chkbang(0); }

	V m_reset() 
	{ 
		for(I i = 0; i < CntIn(); ++i) flags[i] = false;
	}

	virtual V m_method_(I inlet,const t_symbol *s,I argc,t_atom *argv)
	{
		if(inlet > 0 && (!vasponly || s == sym_vasp)) chkbang(inlet);
		else
			m_method_(inlet,s,argc,argv);
	}

	virtual V m_help() { post("%s - Synchronize a number of vasps",thisName()); }
private:
	BL autoreset,vasponly;
	BL *flags;

	FLEXT_CALLBACK(m_reset)
};

FLEXT_LIB_1("vasp.sync",vasp_sync,I)



/*! \class vasp_split
	\remark \b vasp.split
	\brief Splits a vasp into a number of vectors and the remainder.
	\since 0.0.1
	\param cmdln.1 int - number of vectors to split vasp into (excl. one for the remainder vectors)
	\param inlet.1 vasp - is stored and triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
	\retval outlet.n vasp - vector of stored vasp
	\retval outlet.+ vasp - remainder of stored vasp

	\note if there is no remainder outputs a bang
*/
class vasp_split:
	public vasp_op
{
	FLEXT_HEADER(vasp_split,vasp_op)

public:
	vasp_split(I n)
	{
		AddInAnything();
		AddOutAnything(n+1);
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		I outs = CntOut()-1,rem = ref.Vectors()-outs;
		for(I i = min(outs,ref.Vectors())-1; i >= 0; --i) {
			Vasp v(ref.Frames(),ref.Vector(i));
			ToOutVasp(i,v);
		}
		if(rem > 0) {
			Vasp v(ref.Frames(),ref.Vector(outs));
			for(I i = 1; i < rem; ++i) v += ref.Vector(outs+i);
			ToOutVasp(outs,v);
		}
		else
			ToOutBang(outs);
	}

	virtual V m_help() { post("%s - Split a vasp into its vectors",thisName()); }
};

FLEXT_LIB_1("vasp.split",vasp_split,I)


/*! \class vasp_join
	\remark \b vasp.join
	\brief Joins several vasps into one.
	\since 0.0.1
	\param cmdln.1 int - number of vasp slots
	\param inlet.1 vasp - is stored and output triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
	\param inlet.1 reset - clears slots
	\param inlet.+n vasp - is stored in this slot 
	\retval outlet vasp - joined vasp

	The several vectors of the several vasps are all joined into one vasp.

	\note On different vasp frame count the minmum frame count is taken.
	\note The latest vector input to a slot is taken for the resulting vasp
*/
class vasp_join:
	public vasp_tx
{
	FLEXT_HEADER(vasp_join,vasp_tx)

public:
	vasp_join(I n):
		cnt(n),vi(new Vasp *[n-1])
	{
		for(I i = 0; i < cnt-1; ++i) vi[i] = NULL;

		AddInAnything(n);
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD_(0,"reset",m_reset);
	}

	~vasp_join()	{ if(vi) delete[] vi; }

	virtual Vasp *x_work() { 
		Vasp *ret = new Vasp(ref);
		for(I i = 0; i < cnt-1; ++i) if(vi[i]) *ret += *vi[i];
		return ret;
	}

	V m_reset() 
	{ 
		ref.Clear();
		for(I i = 0; i < cnt-1; ++i) if(vi[i]) { delete vi[i]; vi[i] = NULL; }
	}

	virtual V m_method_(I inlet,const t_symbol *s,I argc,t_atom *argv)
	{
		if(inlet > 0 && s == sym_vasp) {
			if(vi[inlet-1]) delete vi[inlet-1];
			vi[inlet-1] = new Vasp(argc,argv);
		}
		else
			m_method_(inlet,s,argc,argv);
	}

	virtual V m_help() { post("%s - Join several vasps into one",thisName()); }
private:
	I cnt;
	Vasp **vi;

	FLEXT_CALLBACK(m_reset)
};

FLEXT_LIB_1("vasp.join",vasp_join,I)



/*! \class vasp_spit
	\remark \b vasp.spit
	\brief Spit out vectors of a vasp consecutively.
	\since 0.0.1
	\param inlet.1 vasp - is stored and triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
	\retval outlet.n vasp - vectors of stored vasp
	\retval outlet.+ bang - triggered after last spit 
*/
class vasp_spit:
	public vasp_op
{
	FLEXT_HEADER(vasp_spit,vasp_op)

public:
	vasp_spit(I argc,t_atom *argv)
	{
		I n = 1;
		if(argc >= 1) n = GetAInt(argv[0]);
		if(n < 1) {
			post("%s - illegal outlet count (%i) -> set to 1",thisName(),n);
			n = 1;
		}

		AddInAnything();
		AddOutAnything(n);
		AddOutBang();
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		I outs = CntOut()-1,rem = ref.Vectors();
		for(I vi = 0; rem;) {
			I r = min(rem,outs);
			for(I i = 0; i < r; ++i) {
				Vasp v(ref.Frames(),ref.Vector(vi+i));
				ToOutVasp(r-1-i,v);
			}
			vi += r;
			rem -= r;
		}
		ToOutBang(outs);
	}

	virtual V m_help() { post("%s - Spit out vectors of a vasp",thisName()); }
};

FLEXT_LIB_G("vasp.spit",vasp_spit)


/*! \class vasp_gather
	\remark \b vasp.gather
	\brief Gathers several consecutive vasps into one.
	\since 0.0.1
	\param cmdln.1 int - number of vasp slots
	\param inlet.1 vasp - is stored and output triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - sets result vasp 
	\param inlet.1 reset - clears result
	\param inlet.2 vasp - add to result vasp
	\retval outlet vasp - gathered vasp

	The several incoming vectors are all gathered into one vasp.

	\note On different vasp frame count the minimum frame count is taken.
*/
class vasp_gather:
	public vasp_tx
{
	FLEXT_HEADER(vasp_gather,vasp_tx)

public:
	vasp_gather(I argc,t_atom *argv)
	{
		cnt = 0;
		if(argc >= 1) cnt = GetAInt(argv[0]);
		if(cnt < 0) {
			post("%s - illegal count (%i) -> set to 0 (triggered mode)",thisName(),cnt);
			cnt = 0;
		}
		rem = cnt;

		AddInAnything(2);
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD_(0,"reset",m_reset);
		FLEXT_ADDMETHOD_(1,"vasp",m_add);
	}

	~vasp_gather()	{ }

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		*ret += dst;
		m_reset(); 
		return ret; 
	}

	V m_reset() { ref.Clear(); dst.Clear(); rem = cnt; }

	virtual I m_set(I argc,t_atom *argv) { rem = cnt; return vasp_tx::m_set(argc,argv); }

	V m_add(I argc,t_atom *argv) 
	{ 
		dst += Vasp(argc,argv);
		if(cnt && !--rem) m_bang();
	}

	virtual V m_help() { post("%s - Gather several vasps into one",thisName()); }
private:
	I cnt,rem;

	FLEXT_CALLBACK(m_reset)
	FLEXT_CALLBACK_G(m_add)
};

FLEXT_LIB_G("vasp.gather",vasp_gather)




/*! \class vasp_part
	\remark \b vasp.part
	\brief Gets parts of vasp vectors.
	\since 0.0.1
	\param cmdln.1 list - list of part lengts
	\param inlet.1 vasp - is stored and output triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
    \param inlet.2 list - list of part lengts
 	\retval outlet.1 vasp - consecutive vasp parts
 	\retval outlet.2 vasp - remainder

	\todo Implement unit processing.
	\remarks Output zero length vasps?
*/
class vasp_part:
	public vasp_op
{
	FLEXT_HEADER(vasp_part,vasp_op)

public:
	vasp_part(I argc,t_atom *argv):
		part(NULL),parts(0)
	{
		m_part(argc,argv);

		AddInAnything(2);
		AddOutAnything(2);
		SetupInOut();

		FLEXT_ADDMETHOD_(1,"list",m_part);
	}

	~vasp_part() { if(part) delete[] part; }

	V m_part(I argc,t_atom *argv) 
	{ 
		if(part) delete[] part; parts = 0;
		part = new I[argc]; 
		for(I i = 0; i < argc; ++i) {
			BL warn = false;
			I p = GetAFloat(argv[i]); // \todo unit processing
			if(p < 0 && !warn) {
				post("%s - invalid part length(s) -> set to 0",thisName());
				p = 0; warn = true;
			}
			part[i] = p; ++parts;
		}
	}

	virtual V m_bang() 
	{ 
		I fr = ref.Frames(),o = 0,f = 0;
		for(I i = 0; i < parts && (fr < 0 || fr); ++i) {
			I p = part[i];
			if(fr >= 0) { p = min(p,fr); fr -= p; }

			Vasp ret(ref); 
			ret.Frames(p);
			ret.OffsetD(o);
			ToOutVasp(0,ret);

			o += p;
		}

		if(fr) {
			Vasp ret(ref); 
			ret.Frames(fr);
			ret.OffsetD(o);
			ToOutVasp(1,ret);
		}
	}

	virtual V m_help() { post("%s - Set a vasp's offset(s) into the vector buffers",thisName()); }
protected:
	I parts,*part;

	FLEXT_CALLBACK_G(m_part)
};

FLEXT_LIB_G("vasp.part",vasp_part)




/*! \class vasp_offs
	\remark \b vasp.offs
	\brief Sets offset of vasp vectors.
	\since 0.0.1
	\param cmdln.1 [_time=0] - offset into buffer(s)
	\param inlet.1 vasp - is stored and output triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
    \param inlet.2 _time - offset into buffer(s)
	\retval outlet vasp - modified vasp

	\attention Normally vasp vectors have individual offsets - this operations sets all the offsets to equal values.
	\todo Implement unit processing.
*/
class vasp_offs:
	public vasp_tx
{
	FLEXT_HEADER(vasp_offs,vasp_tx)

public:
	vasp_offs(I argc,t_atom *argv):
		offs(0),seto(false)
	{
		if(argc >= 1 && CanbeFloat(argv[0]))
			m_offs(GetAFloat(argv[0]));
		else if(argc)
			post("%s - Offset argument invalid -> ignored",thisName());

		AddInAnything();
		AddInFloat();
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD(1,m_offs);
	}

	V m_offs(F o) 
	{ 
		offs = o; //! \todo unit processing 
		seto = true; 
	}

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		if(seto) ret->Offset(offs);
		return ret;
	}

	virtual V m_help() { post("%s - Set a vasp's offset(s) into the vector buffers",thisName()); }
protected:
	I offs;
	BL seto;

private:
	FLEXT_CALLBACK_F(m_offs);
};

FLEXT_LIB_G("vasp.offs",vasp_offs)




/*! \class vasp_doffs
	\remark \b vasp.offs+
	\brief Sets offset of vasp vectors differentially.
	\since 0.0.1
	\param cmdln.1 [_time=0] - increase offset of into buffer(s)
	\param inlet.1 vasp - is stored and output triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
    \param inlet.2 _time - increase of offset into buffer(s)
	\retval outlet vasp - modified vasp

	\todo Implement unit processing
*/
class vasp_doffs:
	public vasp_offs
{
	FLEXT_HEADER(vasp_doffs,vasp_offs)

public:
	vasp_doffs(I argc,t_atom *argv): vasp_offs(argc,argv) {}

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		if(seto) ret->OffsetD(offs);
		return ret;
	}

	virtual V m_help() { post("%s - Shift a vasp's offset(s) into the vector buffers",thisName()); }
};

FLEXT_LIB_G("vasp.offs+",vasp_doffs)



/*! \class vasp_qoffs
	\remark \b vasp.offs?
	\brief Get offset of singled vector vasp.
	\since 0.0.1
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet _time - offset into vector buffer

	\note Outputs 0 if vasp is undefined or invalid
	\note Only works for a vasp with one vector. No output otherwise.

	\todo Implement unit processing
	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
*/
class vasp_qoffs:
	public vasp_op
{
	FLEXT_HEADER(vasp_qoffs,vasp_op)

public:

	vasp_qoffs()
	{
		AddInAnything();
		AddOutFloat();
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		if(ref.Vectors() > 1) 
			post("%s - More than one vector in vasp!",thisName());
		else {
			I o = 0;
			if(ref.Vectors() == 1) {
				o = ref.Vector(0).Offset();
				if(o < 0) o = 0;
				else {
					VBuffer *buf = ref.Buffer(0);
					if(buf) {
						I f = buf->Length();
						if(o > f) o = f;
						delete buf;
					}
				}
			}
			//! \todo unit processing
			ToOutFloat(0,o);
		}
	}

	virtual V m_help() { post("%s - Get a single vectored vasp's offset into the buffer",thisName()); }
};

FLEXT_LIB("vasp.offs?",vasp_qoffs)



/*! \class vasp_frames
	\remark \b vasp.frames
	\brief Sets frame count of vasp.
	\since 0.0.1
	\param cmdln.1 [_time=0] - frame count in time units
	\param inlet.1 vasp - is stored and output triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
    \param inlet.2 _time - frame count in time units
	\retval outlet vasp - modified vasp

	\todo Implement unit processing.
*/
class vasp_frames:
	public vasp_tx
{
	FLEXT_HEADER(vasp_frames,vasp_tx)

public:
	vasp_frames(I argc,t_atom *argv):
		frms(0),setf(false)
	{
		if(argc >= 1 && CanbeInt(argv[0])) 
			m_frms(GetAInt(argv[0]));
		else if(argc)
			post("%s - Frame count argument invalid -> ignored",thisName());

		AddInAnything();
		AddInFloat();
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD(1,m_frms);
	}

	V m_frms(F f) 
	{ 
		frms = f; //! \todo unit processing
		setf = true; 
	}

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		if(setf) ret->Frames(frms);
		return ret;
	}

	virtual V m_help() { post("%s - Set a vasp's frame count",thisName()); }
protected:
	I frms;
	BL setf;

private:
	FLEXT_CALLBACK_F(m_frms);
};

FLEXT_LIB_G("vasp.frames",vasp_frames)




/*! \class vasp_dframes
	\remark \b vasp.frames+
	\brief Sets frame count of vasp differentially.
	\since 0.0.1
	\param cmdln.1 [_time=0] - increase of frame count in time units
	\param inlet.1 vasp - is stored and output triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
    \param inlet.2 _time - increase of frame count in time units
	\retval outlet vasp - modified vasp

	\todo Implement unit processing.
*/
class vasp_dframes:
	public vasp_frames
{
	FLEXT_HEADER(vasp_dframes,vasp_frames)

public:
	vasp_dframes(I argc,t_atom *argv): vasp_frames(argc,argv) {}

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		if(setf) ret->FramesD(frms);
		return ret;
	}

	virtual V m_help() { post("%s - Raise/lower a vasp's frame count",thisName()); }
};

FLEXT_LIB_G("vasp.frames+",vasp_dframes)



/*! \class vasp_qframes
	\remark \b vasp.frames?
	\brief Get frame count in time units
	\since 0.0.1
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet _time - frame count of vasp in time units

	\note Outputs 0 if vasp is undefined or invalid

	\todo Implement unit processing
	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
*/
class vasp_qframes:
	public vasp_op
{
	FLEXT_HEADER(vasp_qframes,vasp_op)

public:

	vasp_qframes()
	{
		AddInAnything();
		AddOutInt();
		SetupInOut();
	}

	virtual V m_bang() { ToOutInt(0,ref.ChkFrames()); }	//! \todo unit processing

	virtual V m_help() { post("%s - Get a vasp's frame count",thisName()); }
};

FLEXT_LIB("vasp.frames?",vasp_qframes)



/*! \class vasp_q
	\remark \b vasp.?
	\brief Get samples of a single vasp vector.
	\since 0.0.1
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet vector - vasp samples

	\note Outputs 0 if vasp is undefined or invalid
	\note Only works for a vasp with one vector. No output otherwise.
*/
class vasp_q:
	public vasp_op
{
	FLEXT_HEADER(vasp_q,vasp_op)

public:

	vasp_q()
	{
		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		if(ref.Vectors() > 1) 
			post("%s - More than one vector in vasp!",thisName());
		else {
			VBuffer *buf = ref.Buffer(0);
			I cnt = buf->Length();
			t_atom *lst = new t_atom[cnt];
			for(I i = 0; i < cnt; ++i) SetFloat(lst[i],buf->Data()[i]);
			ToOutAnything(0,sym_vector,cnt,lst);
			delete[] lst;
		}
	}

	virtual V m_help() { post("%s - Get samples of a vasp vector",thisName()); }
};

FLEXT_LIB("vasp.?",vasp_q)



/*! \class vasp_i
	\remark \b vasp.!
	\brief Get vasp immediate.
	\since 0.0.1
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet vasp! - vasp immediate

*/
class vasp_i:
	public vasp_op
{
	FLEXT_HEADER(vasp_i,vasp_op)

public:

	vasp_i()
	{
		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		post("%s - Sorry, not implemented yet",thisName());
	}

	virtual V m_help() { post("%s - Get immediate vasp vectors",thisName()); }
};

FLEXT_LIB("vasp.!",vasp_i)



