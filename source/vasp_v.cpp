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
};

FLEXT_LIB_G("vasp",vasp_v)




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
};

FLEXT_LIB("vasp.update",vasp_update)



class vasp_sync:
	public vasp_op
{
	FLEXT_HEADER(vasp_sync,vasp_op)

public:
	vasp_sync(FI n):
		cnt(n),flags(new BL[n])
	{
		AddInAnything(n);
		AddOutAnything();
		SetupInOut();
	}

	vasp_sync::~vasp_sync()	{ if(flags) delete[] flags; }

	V chkbang(I n) {
		BL f = flags[n]; 
		flags[n] = true; 
		if(f) { // flags have changed
			
			BL all = true;
			for(I i = 0; i < cnt; ++i) all = all && flags[i];

			if(all) {
				if(ref.Ok()) ToOutVasp(0,ref);
				else ToOutBang(0);
			}
		}
	}

	virtual V m_bang() { chkbang(0); }

	virtual V m_clear() 
	{ 
		for(I i = 0; i < cnt; ++i) flags[i] = false;
	}

	virtual V m_method_(I inlet,const t_symbol *s,I argc,t_atom *argv)
	{
		if(inlet > 0 && s == sym_vasp) chkbang(inlet);
		else
			m_method_(inlet,s,argc,argv);
	}

private:
	I cnt;
	BL *flags;
};

FLEXT_LIB_1("vasp.sync",vasp_sync,FI)



class vasp_split:
	public vasp_op
{
	FLEXT_HEADER(vasp_split,vasp_op)

public:
	vasp_split(FI n)
	{
		AddInAnything();
		AddOutAnything(n);
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		for(I i = 0; i < ref.Vectors(); ++i) {
			Vasp *v = new Vasp(ref.Frames(),ref.Vector(i));
			AtomList *lst = v->MakeList(false);
			if(lst) {
				ToOutAnything(0,sym_vasp,lst->Count(),lst->Atoms());
				delete lst;
			}
			delete v;
		}
	}
};

FLEXT_LIB_1("vasp.split",vasp_split,FI)


class vasp_join:
	public vasp_tx
{
	FLEXT_HEADER(vasp_join,vasp_tx)

public:
	vasp_join(FI n):
		cnt(n),vi(new Vasp *[n-1])
	{
		AddInAnything(n);
		AddOutAnything();
		SetupInOut();
	}

	vasp_join::~vasp_join()	{ if(vi) delete[] vi; }

	virtual Vasp *x_work() { 
		Vasp *ret = new Vasp(ref);
		for(I i = 0; i < cnt-1; ++i) if(vi[i]) *ret += *vi[i];
		return ret;
	}

	virtual V m_clear() 
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

private:
	I cnt;
	Vasp **vi;
};

FLEXT_LIB_1("vasp.join",vasp_join,FI)




class vasp_offs:
	public vasp_tx
{
	FLEXT_HEADER(vasp_offs,vasp_tx)

public:
	vasp_offs(I argc,t_atom *argv):
		offs(0)
	{
		if(argc >= 1 && CanbeInt(argv[0]))
			offs = GetAInt(argv[0]);
		else
			post("%s - Offset argument invalid -> set to 0",thisName());

		AddInAnything();
		AddInFlint();
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD(1,m_offs);
	}

	V m_offs(I o) { offs = o; }

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		for(I i = 0; i < ret->Vectors(); ++i)
			ret->Vector(i).Offset(offs);
		return ret;
	}
protected:
	I offs;

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
		for(I i = 0; i < ret->Vectors(); ++i)
			ret->Vector(i).OffsetD(offs);
		return ret;
	}
};

FLEXT_LIB_G("vasp.offs+",vasp_doffs)


class vasp_qoffs:
	public vasp_op
{
	FLEXT_HEADER(vasp_qoffs,vasp_op)

public:

	vasp_qoffs(I argc,t_atom *argv)
	{
		AddInAnything();
		AddOutFlint();
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		if(ref.Vectors() == 1) {
			Vasp::Ref &r = ref.Vector(0);
			ToOutFlint(0,r.Offset());
		}
		else if(ref.Vectors() == 0) ToOutFlint(0,0);
		else
			post("%s - more than one vector in vasp!",thisName());
	}
};

FLEXT_LIB_G("vasp.offs?",vasp_qoffs)



class vasp_frames:
	public vasp_tx
{
	FLEXT_HEADER(vasp_frames,vasp_tx)

public:
	vasp_frames(I argc,t_atom *argv):
		frms(0)
	{
		if(argc >= 1 && CanbeInt(argv[0]))
			frms = GetAInt(argv[0]);
		else
			post("%s - Frame count argument invalid -> set to 0",thisName());

		AddInAnything();
		AddInFlint();
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD(1,m_frms);
	}

	V m_frms(I f) { frms = f; }

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		ret->Frames(frms);
		return ret;
	}
protected:
	I frms;

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
		ret->FramesD(frms);
		return ret;
	}
};

FLEXT_LIB_G("vasp.frames+",vasp_dframes)


class vasp_qframes:
	public vasp_op
{
	FLEXT_HEADER(vasp_qframes,vasp_op)

public:

	vasp_qframes(I argc,t_atom *argv)
	{
		AddInAnything();
		AddOutFlint();
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		ToOutFlint(0,ref.Vectors() == 0?0:ref.Frames());
	}
};

FLEXT_LIB_G("vasp.frames?",vasp_qframes)

