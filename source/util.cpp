#include "util.h"
#include <math.h>

R arg(R re,R im)
{
	if(re) 
		return atan(im/re)+(re < 0?PI:0);
	else
		if(im || re) return im > 0?PI/2:-PI/2;
		else return 0;
}

