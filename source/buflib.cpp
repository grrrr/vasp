/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "buflib.h"
#include <stdio.h>

#define LIBTICK 100
#define LIBTOL 5

#define MAGIC 12349876L

static BufEntry *libhead = NULL,*libtail = NULL;
static I libcnt = 0,libtick = 0;
static t_clock *libclk = NULL;

#ifdef FLEXT_THREADS
static flext_base::ThrMutex libmtx;
#endif

BufEntry::BufEntry(t_symbol *s,I fr): 
	sym(s),magic(MAGIC),
	len(fr),data(new S[fr]),
	refcnt(0),nxt(NULL) 
{
	ASSERT(!sym->s_thing);
	sym->s_thing = (t_class **)this;
}

BufEntry::~BufEntry()
{
	if(sym) sym->s_thing = NULL;
	if(data) delete[] data;
}

V BufEntry::IncRef() { ++refcnt; }
V BufEntry::DecRef() { --refcnt; tick = libtick; }


static BufEntry *FindInLib(t_symbol *s) 
{
	BufEntry *e = (BufEntry *)s->s_thing;
	return e && e->magic == MAGIC?e:NULL;
}

VBuffer *BufLib::Get(t_symbol *s,I chn,I len,I offs)
{
	BufEntry *e = FindInLib(s);
	if(e) 
		return new ImmBuf(e,len,offs);
	else
		return new SysBuf(s,chn,len,offs);
}

V BufLib::IncRef(t_symbol *s) 
{ 
	if(s) {
		BufEntry *e = (BufEntry *)s->s_thing;
		if(e && e->magic == MAGIC) e->IncRef();
	}
}

V BufLib::DecRef(t_symbol *s)
{ 
	if(s) {
		BufEntry *e = (BufEntry *)s->s_thing;
		if(e && e->magic == MAGIC) e->DecRef();
	}
}



static t_symbol *GetLibSym()
{
	char tmp[20];
	sprintf(tmp,"vasp!%04i",libcnt); // what if libcnt has > 4 digits?
	libcnt++;
	return gensym(tmp);
}

static V LibTick(V *)
{
	libmtx.Lock();

	// collect garbage
	BufEntry *p = NULL;
	for(BufEntry *e = libhead; e; ) {
//		post("heap: sym:%s ref:%i",flext_base::GetString(e->sym),e->refcnt);
		if(e->refcnt <= 0 && e->tick+LIBTOL < libtick) {
			ASSERT(e->refcnt == 0);

			BufEntry *n = e->nxt;

			if(p) p->nxt = n;
			else libhead = n;

			if(!n) libtail = p;
			else e->nxt = NULL;

			delete e;

			e = n;
		}
		else
			p = e,e = e->nxt;
	}

//	post("");

	++libtick;
	clock_delay(libclk,LIBTICK);

	libmtx.Unlock();
}

ImmBuf *BufLib::NewImm(I fr)
{
	if(!libclk) {
		libclk = clock_new(NULL,(t_method)LibTick);
		clock_delay(libclk,LIBTICK);
	}

	t_symbol *s = NULL;
	for(;;) {
		s = GetLibSym();
		if(!s->s_thing) break;
	}

	BufEntry *entry = new BufEntry(s,fr);

	ImmBuf *buf = new ImmBuf(entry);

	libmtx.Lock();

	if(libtail) libtail->nxt = entry; 
	else libhead = entry;
	libtail = entry;

	libmtx.Unlock();

	return buf;
}




ImmBuf::ImmBuf(BufEntry *e,I len,I offs): 
	VBuffer(0,len,offs),entry(e) 
{ 
	if(entry) entry->IncRef(); 
}

ImmBuf::~ImmBuf() 
{ 
	if(entry) entry->DecRef(); 
}

/*
V ImmBuf::IncRef() {}
V ImmBuf::DecRef() {}
*/

t_symbol *ImmBuf::Symbol() const { return entry->sym; }

I ImmBuf::Frames() const { return entry->len; }

V ImmBuf::Frames(I fr,BL keep) { post("vasp immbuf - sorry not implemented!"); }

S *ImmBuf::Data() { return entry->data; }



