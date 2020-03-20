/*

VASP modular - vector assembling signal processor / objects for Max and Pure Data

Copyright (c)2002-2015 Thomas Grill (gr@grrrr.org)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_CLASSES_H
#define __VASP_CLASSES_H

#include "vasp.h"
#include "arg.h"


class vasp_base:
	public flext_base
{
	FLEXT_HEADER_S(vasp_base,flext_base,Setup)

public:
	enum xs_unit {
		xsu__ = -1,  // don't change
		xsu_sample = 0,xsu_buffer,xsu_ms,xsu_s
	};	

	static const t_symbol *sym_vasp;
	static const t_symbol *sym_env;
	static const t_symbol *sym_double;
	static const t_symbol *sym_complex;
	static const t_symbol *sym_vector;
	static const t_symbol *sym_radio;

protected:
	vasp_base();
	virtual ~vasp_base();

	virtual V m_radio(I argc,const t_atom *argv);  // commands for all

    virtual V m_help() = 0;

/*
	V m_argchk(BL chk);  // precheck argument on arrival
	V m_loglvl(I lvl);  // noise level of log messages
	V m_unit(xs_unit u);  // unit command
*/
	BL refresh;  // immediate graphics refresh?
	BL argchk;   // pre-operation argument feasibility check
    BL deferred;    // queue outgoing vasp messages
	xs_unit unit;  // time units
	I loglvl;	// noise level for log messages

	friend class Vasp;

	BL ToOutVasp(I outlet,Vasp &v);

private:
	static V Setup(t_classid);

	FLEXT_CALLBACK_V(m_radio)
	FLEXT_CALLBACK(m_help)

	FLEXT_ATTRVAR_B(argchk)
	FLEXT_ATTRVAR_B(deferred)
	FLEXT_ATTRVAR_I(loglvl)
	FLEXT_ATTRVAR_E(unit,xs_unit)
};


///////////////////////////////////////////////////////////////////////////
// vasp_op class
///////////////////////////////////////////////////////////////////////////

template<bool withto = false>
class vasp_op:
	public vasp_base
{
	FLEXT_HEADER_S(vasp_op,vasp_base,Setup)

protected:
	vasp_op();

	virtual V m_dobang();						// bang method

	virtual V m_vasp(I argc,const t_atom *argv); // trigger
	virtual I m_set(I argc,const t_atom *argv);  // non trigger
	virtual V m_to(I argc,const t_atom *argv); // set destination
//	V m_detach(BL thr);		// detached thread
//	virtual V m_prior(I dp);  // thread priority +-
	virtual V m_stop();				// stop working

	virtual V m_update(I argc = 0,const t_atom *argv = NULL);  // graphics update

	V m_setupd(const AtomList &l) { m_update(l.Count(),l.Atoms()); }
	V m_getupd(AtomList &l) { l(1); SetBool(l[0],refresh); }

	// destination vasp
	Vasp ref,dst;

	V m_setref(const AtomList &l) { m_set(l.Count(),l.Atoms()); }
	V m_getref(AtomList &l) { ref.MakeList(l); }
	V m_setto(const AtomList &l) { m_to(l.Count(),l.Atoms()); }
	V m_getto(AtomList &l) { dst.MakeList(l); }

	FLEXT_CALLBACK_V(m_to)

	FLEXT_CALLBACK(m_dobang)
#ifdef FLEXT_THREADS
	FLEXT_THREAD(m_bang)

	ThrMutex runmtx;
	V Lock() { runmtx.Lock(); }
	V Unlock() { runmtx.Unlock(); }

//	thrid_t thrid; 
#else
	FLEXT_CALLBACK(m_bang)

	V Lock() {}
	V Unlock() {}
#endif
	BL detach;	// detached operation?
	I prior;  // thread priority

	FLEXT_CALLBACK_V(m_vasp)
	FLEXT_CALLBACK_V(m_set)

	FLEXT_CALLVAR_V(m_getref,m_setref)
	FLEXT_CALLVAR_V(m_getto,m_setto)
	
	FLEXT_CALLBACK(m_stop)

	FLEXT_CALLVAR_V(m_getupd,m_setupd)
	FLEXT_ATTRVAR_B(detach)
	FLEXT_ATTRVAR_I(prior)

private:
	static V Setup(t_classid);

	virtual V m_bang() = 0;						// do! and output current Vasp
};

template<bool withto>
vasp_op<withto>::vasp_op()
:detach(false),prior(-2)
#ifdef FLEXT_THREADS
//	,thrid(0)
#endif
{
}

template<bool withto>
V vasp_op<withto>::Setup(t_classid c)
{
    if(withto) FLEXT_CADDATTR_VAR(c,"to",m_getto,m_setto);
    
    FLEXT_CADDBANG(c,0,m_dobang);
    FLEXT_CADDMETHOD_(c,0,"vasp",m_vasp);
    FLEXT_CADDMETHOD_(c,0,"set",m_set);
    
    FLEXT_CADDATTR_VAR(c,"ref",m_getref,m_setref);
    
    FLEXT_CADDMETHOD_(c,0,"stop",m_stop);
    
    FLEXT_CADDATTR_VAR(c,"update",m_getupd,m_setupd);
    
    FLEXT_CADDATTR_VAR1(c,"detach",detach);
    FLEXT_CADDATTR_VAR1(c,"prior",prior);
}

template<bool withto>
V vasp_op<withto>::m_dobang()
{
#ifdef FLEXT_THREADS
    if(detach)
        FLEXT_CALLMETHOD(m_bang);
    else
#endif
        m_bang();
}

template<bool withto>
I vasp_op<withto>::m_set(I argc,const t_atom *argv)
{
    Vasp arg(argc,argv);
    
    if(argc && !arg.Ok()) {
        ref.Clear();
        post("%s - invalid vasp detected and ignored",thisName());
    }
    else {
        if(arg.Check())
            ref = arg;
        else {
            ref.Clear();
            post("%s - vasp reference is invalid",thisName());
        }
    }
    
    return 0;
}

template<bool withto>
V vasp_op<withto>::m_vasp(I argc,const t_atom *argv)
{
    m_set(argc,argv);
    m_dobang();
}

template<bool withto>
V vasp_op<withto>::m_to(I argc,const t_atom *argv)
{
    Vasp to(argc,argv);
    
    if(argc && !to.Ok()) {
        // empty vasp
        dst.Clear();
    }
    else
        dst = to;
}

template<bool withto>
V vasp_op<withto>::m_update(I argc,const t_atom *argv)
{
    if(argc == 0)
        ref.Refresh();
    else {
        if(CanbeInt(argv[0]))
            refresh = GetAInt(argv[0]) != 0;
        else
            post("%s(update) - argument should be omitted or integer",thisName());
    }
}

template<bool withto>
V vasp_op<withto>::m_stop() {}


///////////////////////////////////////////////////////////////////////////
// vasp_tx class
///////////////////////////////////////////////////////////////////////////

template <bool withto = false>
class vasp_tx:
	public vasp_op<withto>
{
	FLEXT_HEADER(vasp_tx,vasp_op<withto>)

protected:
	vasp_tx();

	virtual V m_bang();						// do! and output current Vasp

	virtual Vasp *x_work() = 0;
};

#define VASP_SETUP(op) FLEXT_SETUP(vasp_##op);  

template<bool withto>
vasp_tx<withto>::vasp_tx() {}

template<bool withto>
V vasp_tx<withto>::m_bang()
{
    // Thread has to wait until previous is finished
    this->Lock();
    
#ifdef FLEXT_THREADS
    if(this->prior && flext::IsThreadRegistered()) this->RelPriority(this->prior);
#endif
    
    if(this->ref.Check())
    {
        Vasp *ret = x_work();
        if(ret) {
            if(!this->ToOutVasp(0,*ret))
                post("%s - empty list",this->thisName());
            if(this->refresh) ret->Refresh();
            delete ret;
        }
        else {
#ifdef FLEXT_DEBUG
            post("%s - no valid return",thisName());
#endif
        }
    }
    else {
        post("%s - no valid vasp to work with",this->thisName());
    }
    
#ifdef FLEXT_THREADS
    //	thrid = 0;
#endif
    
    this->Unlock();
}


///////////////////////////////////////////////////////////////////////////
// vasp_unop class
///////////////////////////////////////////////////////////////////////////

// base class for unary operations
template <bool withto = false>
class vasp_unop:
	public vasp_tx<withto>
{
	FLEXT_HEADER(vasp_unop,vasp_tx<withto>)

protected:
	vasp_unop(UL outcode = 0);

	virtual Vasp *x_work();
	virtual Vasp *tx_work();
};

template<bool withto>
vasp_unop<withto>::vasp_unop(UL outcode)
{
    this->AddInAnything();
    this->AddOutAnything(1);
    this->AddOutlets(outcode);
}

template<bool withto>
Vasp *vasp_unop<withto>::x_work() { return tx_work(); }

template<bool withto>
Vasp *vasp_unop<withto>::tx_work()
{
    error("%s - no work method implemented",this->thisName());
    return NULL;
}


///////////////////////////////////////////////////////////////////////////
// vasp_binop class
///////////////////////////////////////////////////////////////////////////

// base class for binary operations
template <bool withto = false>
class vasp_binop:
	public vasp_tx<withto>
{
	FLEXT_HEADER_S(vasp_binop,vasp_tx<withto>,Setup)

protected:
	vasp_binop(I argc,const t_atom *argv,const Argument &def = Argument(),UL outcode = 0);

	// assignment functions
	virtual V a_list(I argc,const t_atom *argv); 
	/*virtual*/ V a_vasp(I argc,const t_atom *argv);
	/*virtual*/ V a_env(I argc,const t_atom *argv);
	/*virtual*/ V a_float(F f); 
	/*virtual*/ V a_int(I f); 
	/*virtual*/ V a_double(I argc,const t_atom *argv); 
	/*virtual*/ V a_complex(I argc,const t_atom *argv); 
	/*virtual*/ V a_vector(I argc,const t_atom *argv); 

	V a_radio(I,const t_atom *) {}

	virtual Vasp *x_work();
	virtual Vasp *tx_work(const Argument &arg);

	Argument arg;

    V m_setarg(const flext::AtomList &l) { a_list(l.Count(),l.Atoms()); }
	V m_getarg(flext::AtomList &l) { arg.MakeList(l); }

private:
	static V Setup(flext_base::t_classid);

	FLEXT_CALLBACK_V(a_list)
	FLEXT_CALLBACK_V(a_vasp)
	FLEXT_CALLBACK_V(a_env)
	FLEXT_CALLBACK_1(a_float,F)
	FLEXT_CALLBACK_1(a_int,I)
	FLEXT_CALLBACK_V(a_double)
	FLEXT_CALLBACK_V(a_complex)
	FLEXT_CALLBACK_V(a_vector)
	FLEXT_CALLBACK_V(a_radio)

	FLEXT_CALLVAR_V(m_getarg,m_setarg)
};

template<bool withto>
vasp_binop<withto>::vasp_binop(I argc,const t_atom *argv,const Argument &def,UL outcode)
{
    a_list(argc,argv);
    if(arg.IsNone() && !def.IsNone()) arg = def;
    
    this->AddInAnything(2);
    this->AddOutAnything(1);
    this->AddOutlets(outcode);
}

template<bool withto>
V vasp_binop<withto>::Setup(flext_base::t_classid c)
{
    FLEXT_CADDMETHOD(c,1,a_list);
    FLEXT_CADDMETHOD_(c,1,"vasp",a_vasp);
    FLEXT_CADDMETHOD_(c,1,"env",a_env);
    FLEXT_CADDMETHOD_(c,1,"float",a_float);
    FLEXT_CADDMETHOD_(c,1,"double",a_double);
    FLEXT_CADDMETHOD_(c,1,"int",a_int);
    FLEXT_CADDMETHOD_(c,1,"complex",a_complex);
    FLEXT_CADDMETHOD_(c,1,"vector",a_vector);
    FLEXT_CADDMETHOD_(c,1,"radio",a_radio);
    
    FLEXT_CADDATTR_VAR(c,"arg",m_getarg,m_setarg);
}

template<bool withto>
V vasp_binop<withto>::a_list(I argc,const t_atom *argv)
{
    if(argc) {
        arg.Parse(argc,argv);
        if(arg.IsNone())
            post("%s - list argument could not be evaluated (ignored)",this->thisName());
        else if(this->argchk) {
            // check argument feasibility
        }
    }
    else {
        //		post("%s - Empty list argument (ignored)",thisName());
    }
}

template<bool withto>
V vasp_binop<withto>::a_vasp(I argc,const t_atom *argv)
{
    Vasp *v = new Vasp(argc,argv);
    if(v->Ok()) {
        arg.SetVasp(v);
        if(this->argchk) {
            // check argument feasibility
        }
    }
    else {
        post("%s - invalid vasp argument (ignored)",this->thisName());
        delete v;
    }
}

template<bool withto>
V vasp_binop<withto>::a_env(I argc,const t_atom *argv)
{
    Env *bp = new Env(argc,argv);
    if(bp->Ok()) {
        arg.SetEnv(bp);
        if(this->argchk) {
            // check argument feasibility
        }
    }
    else {
        post("%s - invalid env argument (ignored)",this->thisName());
        delete bp;
    }
}

template<bool withto>
V vasp_binop<withto>::a_float(F v) { arg.SetR(v); }

template<bool withto>
V vasp_binop<withto>::a_double(I argc,const t_atom *argv)
{
    if(
       (argc == 1 && this->CanbeFloat(argv[0])) ||
       (argc == 2 && this->CanbeFloat(argv[0]) && this->CanbeFloat(argv[1]))
       ) {
        arg.SetR((D)this->GetAFloat(argv[0])+(D)this->GetAFloat(argv[1]));
        if(this->argchk) {
            // check argument feasibility
        }
    }
    else
        post("%s - invalid double argument (ignored)",this->thisName());
}

template<bool withto>
V vasp_binop<withto>::a_int(I v) { arg.SetI(v); }

template<bool withto>
V vasp_binop<withto>::a_complex(I argc,const t_atom *argv)
{
    if(
       (argc == 1 && this->CanbeFloat(argv[0])) ||
       (argc == 2 && this->CanbeFloat(argv[0]) && this->CanbeFloat(argv[1]))
       ) {
        arg.SetCX(this->GetAFloat(argv[0]),this->GetAFloat(argv[1]));
        if(this->argchk) {
            // check argument feasibility
        }
    }
    else
        post("%s - invalid complex argument (ignored)",this->thisName());
}

template<bool withto>
V vasp_binop<withto>::a_vector(I argc,const t_atom *argv)
{
    error("%s - vector type not implemented",this->thisName());
}


template<bool withto>
Vasp *vasp_binop<withto>::x_work() { return tx_work(arg); }

template<bool withto>
Vasp *vasp_binop<withto>::tx_work(const Argument &)
{
    error("%s - no work method implemented",this->thisName());
    return NULL;
}


///////////////////////////////////////////////////////////////////////////
// vasp_anyop class
///////////////////////////////////////////////////////////////////////////

// base class for non-parsed (list) arguments
template <bool withto = false>
class vasp_anyop:
	public vasp_tx<withto>
{
	FLEXT_HEADER_S(vasp_anyop,vasp_tx<withto>,Setup)

protected:
	vasp_anyop(I argc,const t_atom *argv,const Argument &def = Argument(),UL outcode = 0);

	// assignment functions
	virtual V a_list(I argc,const t_atom *argv); 

	V a_radio(I,const t_atom *) {}

	virtual Vasp *x_work();
	virtual Vasp *tx_work(const Argument &arg);

	Argument arg;

    V m_setarg(const flext::AtomList &l) { a_list(l.Count(),l.Atoms()); }
	V m_getarg(flext::AtomList &l) { arg.MakeList(l); }

private:
	static V Setup(flext_base::t_classid);

	FLEXT_CALLBACK_V(a_list)
	FLEXT_CALLBACK_V(a_radio)

	FLEXT_CALLVAR_V(m_getarg,m_setarg)
};

template<bool withto>
vasp_anyop<withto>::vasp_anyop(I argc,const t_atom *argv,const Argument &def,UL outcode)
{
    a_list(argc,argv);
    if(arg.IsNone() && !def.IsNone()) arg = def;
    
    this->AddInAnything(2);
    this->AddOutAnything(1);
    this->AddOutlets(outcode);
}

template<bool withto>
V vasp_anyop<withto>::Setup(flext_base::t_classid c)
{
    FLEXT_CADDMETHOD(c,1,a_list);
    FLEXT_CADDMETHOD_(c,1,"vasp",a_list);
    FLEXT_CADDMETHOD_(c,1,"radio",a_radio);
    
    FLEXT_CADDATTR_VAR(c,"arg",m_getarg,m_setarg);
}

template<bool withto>
V vasp_anyop<withto>::a_list(I argc,const t_atom *argv)
{ 
    if(argc) {
        arg.SetList(argc,argv);
        if(arg.IsNone()) 
            post("%s - argument could not be evaluated (ignored)",this->thisName());
        else if(this->argchk) {
            // check argument feasibility
        }
    }
    else {
        //		post("%s - Empty list argument (ignored)",thisName());
    }
}

template<bool withto>
Vasp *vasp_anyop<withto>::x_work() { return tx_work(arg); }

template<bool withto>
Vasp *vasp_anyop<withto>::tx_work(const Argument &)
{
    error("%s - no work method implemented",this->thisName());
    return NULL;
}


#define VASP_UNARY(name,op,to,help)												\
class vasp_##op:																\
	public vasp_unop<to>														\
{																				\
	FLEXT_HEADER(vasp_##op,vasp_unop<to>)										\
public:																			\
	vasp_##op(): vasp_unop<to>() {}                                             \
protected:																		\
	virtual Vasp *tx_work()														\
	{																			\
		OpParam p(thisName(),0);												\
		CVasp cdst(dst),cref(ref);												\
		return VaspOp::m_##op(p,cref,&cdst);									\
	}																			\
	virtual V m_help() { post("%s - " help,thisName()); }						\
};																				\
    FLEXT_LIB(name ", vasp_" #op,vasp_##op)												


#define VASP_BINARY(name,op,to,def,help)										\
class vasp_##op:																\
	public vasp_binop<to>														\
{																				\
	FLEXT_HEADER(vasp_##op,vasp_binop<to>)                                      \
public:																			\
	vasp_##op(I argc,const t_atom *argv): vasp_binop<to>(argc,argv,def) {}		\
protected:																		\
	virtual Vasp *tx_work(const Argument &arg)									\
	{																			\
		OpParam p(thisName(),1);												\
		CVasp cdst(dst),cref(ref);												\
		return VaspOp::m_##op(p,cref,arg,&cdst);								\
	}																			\
	virtual V m_help() { post("%s - " help,thisName()); }						\
};																				\
FLEXT_LIB_V(name ", vasp_" #op,vasp_##op)												


#define VASP_ANYOP(name,op,args,to,def,help)									\
class vasp_##op:																\
	public vasp_anyop<to>														\
{																				\
	FLEXT_HEADER(vasp_##op,vasp_anyop<to>)										\
public:																			\
	vasp_##op(I argc,const t_atom *argv): vasp_anyop<to>(argc,argv,def) {}		\
protected:																		\
	virtual Vasp *tx_work(const Argument &arg)									\
	{																			\
		OpParam p(thisName(),args);												\
		CVasp cdst(dst),cref(ref);												\
		return VaspOp::m_##op(p,cref,arg,&cdst);								\
	}																			\
	virtual V m_help() { post("%s - " help,thisName()); }						\
};																				\
FLEXT_LIB_V(name ", vasp_" #op,vasp_##op)												

#define VASP_LIB(name,cl) FLEXT_LIB(name ", " #cl,cl);  
#define VASP_LIB_V(name,cl) FLEXT_LIB_V(name ", " #cl,cl);  


#define VASP__SETUP(op) FLEXT_SETUP(vasp_##op);  

#endif
