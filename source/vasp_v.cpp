#include "main.h"

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
	}

	virtual Vasp *x_work() { return new Vasp(ref); }

	virtual V m_help() { post("%s - Store and output a vasp",thisName()); }
};

FLEXT_LIB_G("vasp",vasp_v)



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

		AddInAnything();
		AddOutAnything();
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

	virtual V m_bang() { ToOutInt(0,ref.Vectors()); }

	virtual V m_help() { post("%s - Get number of vectors of a vasp",thisName()); }
};

FLEXT_LIB("vasp.n?",vasp_qn)




class vasp_sync:
	public vasp_op
{
	FLEXT_HEADER(vasp_sync,vasp_op)

public:
	vasp_sync(I n):
		flags(new BL[n]),
		autoreset(true)
	{
		AddInAnything(n);
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD_(0,"clear",m_clear);

		m_clear();
	}

	vasp_sync::~vasp_sync()	{ if(flags) delete[] flags; }

	V chkbang(I n) {
		BL f = flags[n]; 
		flags[n] = true; 
		if(!f) { // flags have changed
			
			BL all = true;
			for(I i = 0; i < CntIn(); ++i) all = all && flags[i];

			if(all) {
				if(ref.Ok()) ToOutVasp(0,ref);
				else ToOutBang(0);

				if(autoreset) m_clear();
			}
		}
	}

	virtual V m_bang() { chkbang(0); }

	virtual V m_clear() 
	{ 
		for(I i = 0; i < CntIn(); ++i) flags[i] = false;
	}

	virtual V m_method_(I inlet,const t_symbol *s,I argc,t_atom *argv)
	{
		if(inlet > 0 && s == sym_vasp) chkbang(inlet);
		else
			m_method_(inlet,s,argc,argv);
	}

	virtual V m_help() { post("%s - Synchronize a number of vasps",thisName()); }
private:
	BL autoreset;
	BL *flags;

	FLEXT_CALLBACK(m_clear)
};

FLEXT_LIB_1("vasp.sync",vasp_sync,I)



class vasp_split:
	public vasp_op
{
	FLEXT_HEADER(vasp_split,vasp_op)

public:
	vasp_split(I n)
	{
		AddInAnything();
		AddOutAnything(n);
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		for(I i = ref.Vectors()-1; i >= 0; --i) 
			ToOutVasp(i,Vasp(ref.Frames(),ref.Vector(i)));
	}

	virtual V m_help() { post("%s - Split a vasp into its vectors",thisName()); }
};

FLEXT_LIB_1("vasp.split",vasp_split,I)


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

		FLEXT_ADDMETHOD_(0,"clear",m_clear);
	}

	vasp_join::~vasp_join()	{ if(vi) delete[] vi; }

	virtual Vasp *x_work() { 
		Vasp *ret = new Vasp(ref);
		for(I i = 0; i < cnt-1; ++i) if(vi[i]) *ret += *vi[i];
		return ret;
	}

	V m_clear() 
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

	FLEXT_CALLBACK(m_clear)
};

FLEXT_LIB_1("vasp.join",vasp_join,I)




class vasp_offs:
	public vasp_tx
{
	FLEXT_HEADER(vasp_offs,vasp_tx)

public:
	vasp_offs(I argc,t_atom *argv):
		offs(0),seto(false)
	{
		if(argc >= 1 && CanbeInt(argv[0]))
			m_offs(GetAInt(argv[0]));
		else if(argc)
			post("%s - Offset argument invalid -> ignored",thisName());

		AddInAnything();
		AddInInt();
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD(1,m_offs);
	}

	V m_offs(I o) { offs = o,seto = true; }

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		if(seto) {
			for(I i = 0; i < ret->Vectors(); ++i)
				ret->Vector(i).Offset(offs);
		}
		return ret;
	}

	virtual V m_help() { post("%s - Set a vasp's offset(s) into the vector buffers",thisName()); }
protected:
	I offs;
	BL seto;

private:
	FLEXT_CALLBACK_I(m_offs);
};

FLEXT_LIB_G("vasp.offs",vasp_offs)




class vasp_doffs:
	public vasp_offs
{
	FLEXT_HEADER(vasp_doffs,vasp_offs)

public:
	vasp_doffs(I argc,t_atom *argv): vasp_offs(argc,argv) {}

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		if(seto) {
			for(I i = 0; i < ret->Vectors(); ++i)
				ret->Vector(i).OffsetD(offs);
		}
		return ret;
	}

	virtual V m_help() { post("%s - Shift a vasp's offset(s) into the vector buffers",thisName()); }
};

FLEXT_LIB_G("vasp.offs+",vasp_doffs)



class vasp_qoffs:
	public vasp_op
{
	FLEXT_HEADER(vasp_qoffs,vasp_op)

public:

	vasp_qoffs()
	{
		AddInAnything();
		AddOutInt();
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
			ToOutInt(0,o);
		}
	}

	virtual V m_help() { post("%s - Get a single vectored vasp's offset into the buffer",thisName()); }
};

FLEXT_LIB("vasp.offs?",vasp_qoffs)



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
		AddInInt();
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD(1,m_frms);
	}

	V m_frms(I f) { frms = f,setf = true; }

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
	FLEXT_CALLBACK_I(m_frms);
};

FLEXT_LIB_G("vasp.frames",vasp_frames)




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

	virtual V m_bang() 
	{ 
		if(ref.Vectors() == 0) ToOutInt(0,0);
		else {
			I frms = -1;
			for(I i = 0; i < ref.Vectors(); ++i) {
				VBuffer *buf = ref.Buffer(i);
				if(buf) {
					I f = buf->Length();
					if(frms < 0 || f < frms) frms = f;
					delete buf;
				}
			}
			ToOutInt(0,frms < 0?0:frms);
		}
	}

	virtual V m_help() { post("%s - Get a vasp's frame count",thisName()); }
};

FLEXT_LIB("vasp.frames?",vasp_qframes)

