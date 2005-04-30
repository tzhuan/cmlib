/* Copyright (c) 1991 Regents of the University of California */

/*
 * Read and write image resolutions.
 */

#include <stdio.h>
#include <stdlib.h>

#include "resolu.h"


char  resolu_buf[RESOLU_BUFLEN];	/* resolution line buffer */


void fputresolu(int ord, int sl, int ns, FILE *fp)		/* put out picture dimensions */
//int  ord;			/* scanline ordering */
//int  sl, ns;			/* scanline length and number */
//FILE  *fp;
{
	RESOLU  rs;

	if ((rs.orientation = ord) & YMAJOR) {
		rs.xr = sl;
		rs.yr = ns;
	} else {
		rs.xr = ns;
		rs.yr = sl;
	}
	fputsresolu(&rs, fp);
}


int fgetresolu(int *sl, int *ns, FILE *fp)			/* get picture dimensions */
//int  *sl, *ns;			/* scanline length and number */
//FILE  *fp;
{
	RESOLU  rs;

	if (!fgetsresolu(&rs, fp))
		return(-1);
	if (rs.orientation & YMAJOR) {
		*sl = rs.xr;
		*ns = rs.yr;
	} else {
		*sl = rs.yr;
		*ns = rs.xr;
	}
	return(rs.orientation);
}


char *resolu2str(char *buf, register RESOLU *rp)		/* convert resolution struct to line */
{
	if (rp->orientation&YMAJOR)
		sprintf(buf, "%cY %d %cX %d\n",
				rp->orientation&YDECR ? '-' : '+', rp->yr,
				rp->orientation&XDECR ? '-' : '+', rp->xr);
	else
		sprintf(buf, "%cX %d %cY %d\n",
				rp->orientation&XDECR ? '-' : '+', rp->xr,
				rp->orientation&YDECR ? '-' : '+', rp->yr);
	return(buf);
}


int str2resolu(register RESOLU *rp, char *buf)		/* convert resolution line to struct */
{
	register char  *xndx, *yndx;
	register char  *cp;

	if (buf == NULL)
		return(0);
	xndx = yndx = NULL;
	for (cp = buf; *cp; cp++)
		if (*cp == 'X')
			xndx = cp;
		else if (*cp == 'Y')
			yndx = cp;
	if (xndx == NULL || yndx == NULL)
		return(0);
	rp->orientation = 0;
	if (xndx > yndx) rp->orientation |= YMAJOR;
	if (xndx[-1] == '-') rp->orientation |= XDECR;
	if (yndx[-1] == '-') rp->orientation |= YDECR;
	if ((rp->xr = atoi(xndx+1)) <= 0)
		return(0);
	if ((rp->yr = atoi(yndx+1)) <= 0)
		return(0);
	return(1);
}
