/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPPERMUTE_H
#define __VASP_OPPERMUTE_H

template <class T,int DIM>
int permutation(OpParam &p,int (*origination)(int ij, int n,OpParam &p))
{
	int di,ij, oij, dij, n_to_do;
	T b[DIM],*sdt[DIM],*ddt[DIM];
	I ss[DIM],ds[DIM];

	sdt[0] = p.rsdt,ddt[0] = p.rddt; ss[0] = p.rss,ds[0] = p.rds;
	if(DIM == 2) {
		sdt[1] = p.isdt,ddt[1] = p.iddt; ss[1] = p.iss,ds[1] = p.ids;
	}

	I n = p.frames;
	n_to_do = n;
	for(ij = 0; ij < n && n_to_do > 0; ij++,n_to_do--) {
		/* Test for previously permuted */
		for (oij = origination(ij,n,p); oij > ij; oij = origination(oij,n,p)) (void)0;

		if (oij < ij) continue;

		/* Chase the cycle */
		dij = ij;
		
		for(di = 0; di < DIM; ++di) b[di] = sdt[di][ij*ss[di]];

		for(oij = origination(dij,n,p); oij != ij; oij = origination(dij,n,p),n_to_do--) {
			for(di = 0; di < DIM; ++di) ddt[di][dij*ds[di]] = sdt[di][oij*ss[di]];
			dij = oij;
		}

		for(di = 0; di < DIM; ++di) ddt[di][dij*ds[di]] = b[di];
	} 
	return 0;
}

#endif

