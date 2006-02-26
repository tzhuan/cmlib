#include <stdexcept>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <cmath>

#include "gil/io/hdr.h"

using namespace std;
using namespace gil;

namespace {
    // some ugly definition
    const int RED = 0;
    const int GRN = 1;
    const int BLU = 2;
    const int EXP = 3;
    const int COLXS = 128;

    const size_t MIN_ENC_LENGTH = 8;
    const size_t MAX_ENC_LENGTH = 32767; // 0x7FFF
    const size_t MINRUN = 4;


    // this part imported from original header.c in io_utils
    
    /* Copyright (c) 1994 Regents of the University of California */

    /*
     *  header.c - routines for reading and writing information headers.
     *
     *	8/19/88
     *
     *  newheader(t,fp)	start new information header identified by string t
     *  isheadid(s)		returns true if s is a header id line
     *  headidval(r,s)	copy header identifier value in s to r
     *  printargs(ac,av,fp) print an argument list to fp, followed by '\n'
     *  isformat(s)		returns true if s is of the form "FORMAT=*"
     *  formatval(r,s)	copy the format value in s to r
     *  fputformat(s,fp)	write "FORMAT=%s" to fp
     *  getheader(fp,f,p)	read header from fp, calling f(s,p) on each line
     *  checkheader(i,p,o)	check header format from i against p and copy to o
     *
     *  To copy header from input to output, use getheader(fin, fputs, fout)
     */

    const size_t MAXLINE = 512;
    
    // information header magic number
    const char HDRSTR[] = "#?";

    const char COLRFMT[] = "32-bit_rle_rgbe";
    
    // format identifier
    char FMTSTR[] = "FORMAT=";	

    // identifying line of information header
    void newheader(char *s, FILE *fp) 
    {
	    fputs(HDRSTR, fp);
	    fputs(s, fp);
	    putc('\n', fp);
    }

    // get header id (return true if is id)
    int headidval(char *r, char *s)
    {
	    const char  *cp = HDRSTR;

	    while (*cp) if (*cp++ != *s++) return(0);
	    if (r == NULL) return(1);
	    while (*s) *r++ = *s++;
	    *r = '\0';
	    return(1);
    }

    // check to see if line is header id
    int isheadid(char *s)
    {
	    return(headidval(NULL, s));
    }

    // print arguments to a file
    void printargs(int ac, char **av, FILE *fp)
    {
	int  quote;

	while (ac-- > 0) {
	    if (strchr(*av, ' ') != NULL) { // quote it
		if (strchr(*av, '\'') != NULL)
		    quote = '"';
		else
		    quote = '\'';
		putc(quote, fp);
		fputs(*av++, fp);
		putc(quote, fp);
	    } else
		fputs(*av++, fp);
	    putc(' ', fp);
	}
	putc('\n', fp);
    }

    // get format value (return true if format)
    int formatval(char *r, const char *s)
    {
	char  *cp = FMTSTR;

	while (*cp) if (*cp++ != *s++) return(0);
	while (isspace(*s)) s++;
	if (!*s) return(0);
	if (r == NULL) return(1);
	while(*s) *r++ = *s++;
	while (isspace(r[-1])) r--;
	*r = '\0';
	return(1);
    }

    // is line a format line?
    int isformat(char *s)
    {
	return(formatval(NULL, s));
    }

    // put out a format value
    void fputformat(const char *s, FILE *fp)
    {
	fputs(FMTSTR, fp);
	fputs(s, fp);
	putc('\n', fp);
    }

    struct check {
	FILE	*fp;
	char	fs[64];
    };
    
    // get header from file
    int getheader(FILE  *fp, void (*f)(const char *, check *), check *p)
    {
	char  buf[MAXLINE];

	for ( ; ; ) {
	    buf[MAXLINE-2] = '\n';
	    if (fgets(buf, MAXLINE, fp) == NULL)
		return(-1);
	    if (buf[0] == '\n')
		return(0);
	    
#ifdef WIN32
	    // ``newline'' on windows has two bytes.
	    if (buf[0] == '\r' && buf[1] == '\n')
		return(0);
#endif

	    if (buf[MAXLINE-2] != '\n') {
		ungetc(buf[MAXLINE-2], fp);	// prevent false end
		buf[MAXLINE-2] = '\0';
	    }
	    if (f != NULL)
		(*f)(buf, p);
	}
    }

    // check a header line for format info.
    static void mycheck(const char  *s, check  *cp)
    {
	if (!formatval(cp->fs, s) && cp->fp != NULL)
	    fputs(s, cp->fp);
    }


    /*
     * Copymatch(pat,str) checks pat for wildcards, and
     * copies str into pat if there is a match (returning true).
     */

#ifdef COPYMATCH
    int copymatch(char *pat, char *str)
    {
	int	docopy = 0;
	register char	*p = pat, *s = str;

	do {
	    switch (*p) {
		case '?':			/* match any character */
		    if (!*s++)
			return(0);
		    docopy++;
		    break;
		case '*':			/* match any string */
		    while (p[1] == '*') p++;
		    do
			if ( (p[1]=='?' || p[1]==*s)
				&& copymatch(p+1,s) ) {
			    strcpy(pat, str);
			    return(1);
			}
		    while (*s++);
		    return(0);
		case '\\':			/* literal next */
		    p++;
		    /* fall through */
		default:			/* normal character */
		    if (*p != *s)
			return(0);
		    s++;
		    break;
	    }
	} while (*p++);
	if (docopy)
	    strcpy(pat, str);
	return(1);
    }
#else
#define copymatch(pat, s)	(!strcmp(pat, s))
#endif


    /*
     * Checkheader(fin,fmt,fout) returns a value of 1 if the input format
     * matches the specification in fmt, 0 if no input format was found,
     * and -1 if the input format does not match or there is an
     * error reading the header.  If fmt is empty, then -1 is returned
     * if any input format is found (or there is an error), and 0 otherwise.
     * If fmt contains any '*' or '?' characters, then checkheader
     * does wildcard expansion and copies a matching result into fmt.
     * Be sure that fmt is big enough to hold the match in such cases!
     * The input header (minus any format lines) is copied to fout
     * if fout is not NULL.
     */

    int checkheader(FILE *fin, const char *fmt, FILE *fout)
    {
	check cdat;

	cdat.fp = fout;
	cdat.fs[0] = '\0';
	if (getheader(fin, mycheck, &cdat) < 0)
	    return(-1);
	if (cdat.fs[0] != '\0')
	    return(copymatch(fmt, cdat.fs) ? 1 : -1);
	return(0);
    }

    // the following part is imported from resolu.c
    
    /*
     * Definitions for resolution line in image file.
     *
     * True image orientation is defined by an xy coordinate system
     * whose origin is at the lower left corner of the image, with
     * x increasing to the right and y increasing in the upward direction.
     * This true orientation is independent of how the pixels are actually
     * ordered in the file, which is indicated by the resolution line.
     * This line is of the form "{+-}{XY} xyres {+-}{YX} yxres\n".
     * A typical line for a 1024x600 image might be "-Y 600 +X 1024\n",
     * indicating that the scanlines are in English text order (PIXSTANDARD).
     */

    /* flags for scanline ordering */
#define  XDECR			1
#define  YDECR			2
#define  YMAJOR			4

    /* standard scanline ordering */
#define  PIXSTANDARD		(YMAJOR|YDECR)
#define  PIXSTDFMT		"-Y %d +X %d\n"

    /* structure for image dimensions */
    struct RESOLU{
	int	orientation;	/* orientation (from flags above) */
	int	xr, yr;		/* x and y resolution */
    };

    /* macros to get scanline length and number */
#define  scanlen(rs)		((rs)->or & YMAJOR ? (rs)->xr : (rs)->yr)
#define  numscans(rs)		((rs)->or & YMAJOR ? (rs)->yr : (rs)->xr)

    /* resolution string buffer and its size */
#define  RESOLU_BUFLEN		32

    /* macros for reading/writing resolution struct */
#define  fputsresolu(rs,fp)	fputs(resolu2str(resolu_buf,rs),fp)
#define  fgetsresolu(rs,fp)	str2resolu(rs, \
	fgets(resolu_buf,RESOLU_BUFLEN,fp))

    /* reading/writing of standard ordering */
#define  fprtresolu(sl,ns,fp)	fprintf(fp,PIXSTDFMT,ns,sl)
#define  fscnresolu(sl,ns,fp)	(fscanf(fp,PIXSTDFMT,ns,sl)==2)

    // resolution line buffer
    char  resolu_buf[RESOLU_BUFLEN];
    
    // convert resolution struct to line
    char *resolu2str(char *buf, RESOLU *rp)
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
    
    // convert resolution line to struct
    int str2resolu(RESOLU *rp, char *buf)
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

    void realpixels_2_rgb(const vector<Byte4>& scanin, vector<Float3>& realscan)
    {       
	size_t x;
	float v;
	for (x = 0; x < scanin.size(); x++) {
	    if ( scanin[x][EXP] == 0) {
		realscan[x][0] = 0; 
		realscan[x][1] = 0;
		realscan[x][2] = 0;                       
	    }                                              
	    else {
		v = ldexp(1.0/256, scanin[x][EXP]-128);
		realscan[x][0] = (scanin[x][RED] + 0.5)*v;
		realscan[x][1] = (scanin[x][GRN] + 0.5)*v;
		realscan[x][2] = (scanin[x][BLU] + 0.5)*v;
	    }
	}
    }

    void rgb_2_realpixels(const vector<Float3>& realscan, vector<Byte4>& scanout)
    {
	size_t x;
	float r, g, b, max;
	float factor;
	int exp;

	for (x = 0; x < realscan.size(); x++) {
	    r = realscan[x][RED];
	    g = realscan[x][GRN];
	    b = realscan[x][BLU];

	    /* find max */
	    max = r;
	    if (max < g) max = g;
	    if (max < b) max = b;

	    /* find normalize factor */
	    factor=1;
	    exp=0;

	    if (max>0) {
		while ((max > 1.0) || (max < 0.5)) {
		    if (max>1.0) {
			max *= 0.5;	
			factor *= 0.5;
			exp++;
		    } else {
			max *= 2;
			factor *= 2;
			exp--;
		    }
		}

		r *= factor;
		g *= factor;
		b *= factor;

		scanout[x][RED] = (unsigned char)(r*255+0.5);
		scanout[x][GRN] = (unsigned char)(g*255+0.5);
		scanout[x][BLU] = (unsigned char)(b*255+0.5);
		scanout[x][EXP] = COLXS + exp;
	    } else if (max==0) {
		scanout[x][RED] = 0;
		scanout[x][GRN] = 0;
		scanout[x][BLU] = 0;
		scanout[x][EXP] = 0;	
	    } else {
		// it is an error
	    }
	}
    }

} // end of anonymous namespace

void HdrReader::init(FILE* f, size_t& w, size_t& h)
{
    my_file = f;
    // don't read resolution into width and height directly
    // or it will failed on 64bit machines.
    int x, y;
    if( checkheader(my_file, COLRFMT, NULL) < 0)
	throw runtime_error("invalid HDR format in HdrReader::init()");
	    
    if(fgetresolu(&x, &y, my_file) < 0)
	throw runtime_error("invalid HDR format in HdrReader::init()");
    
    w = x;
    h = y;
    my_enc_buffer.resize(w);
}

void HdrReader::read_scanline(vector<Float3>& buf)
{
    read_encoded();
    // now the encoded (RGBE) pixels are stored in my_enc_buffer
    realpixels_2_rgb(my_enc_buffer, buf);
}

void HdrReader::read_encoded()
{
    size_t  i, j;
    int  code, val;

    size_t len = my_enc_buffer.size();
    
    // determine scanline type
    if (len < MIN_ENC_LENGTH or len > MAX_ENC_LENGTH){
	read_encoded_old();
	return;
    }
    
    if ((code = getc(my_file)) == EOF)
	throw runtime_error("unexpected eod of file");
    
    if (code != 2) {
	ungetc(code, my_file);
	read_encoded_old();
	return;
    }
    
    my_enc_buffer[0][GRN] = getc(my_file);
    my_enc_buffer[0][BLU] = getc(my_file);
    
    if ((code = getc(my_file)) == EOF)
	throw runtime_error("unexpected eod of file");
    
    if (my_enc_buffer[0][GRN] != 2 || my_enc_buffer[0][BLU] & 128) {
	    my_enc_buffer[0][RED] = 2;
	    my_enc_buffer[0][EXP] = code;
	    read_encoded_old(1);
	    return;
    }
    
    if ( static_cast<size_t>(my_enc_buffer[0][BLU]<<8 | code) != len )
	    throw runtime_error("length mismatch in HdrReader::read_encoded()");
    
    // read each component
    for (i = 0; i < 4; i++){
	for (j = 0; j < len; ) {
	    if ((code = getc(my_file)) == EOF)
		throw runtime_error("unexpected eod of file");
		
	    if (code > 128) {	
		// run
		code &= 127;
		val = getc(my_file);
		while (code--)
		    my_enc_buffer[j++][i] = val;
	    } else {
		// non-run
		while (code--)
		    my_enc_buffer[j++][i] = getc(my_file);
	    }
	}
    }
}

void HdrReader::read_encoded_old(size_t pos)
{
    int  rshift;
    register int  i;

    rshift = 0;
    size_t len = my_enc_buffer.size() - pos;
    vector<Byte4>::iterator p = my_enc_buffer.begin() + pos;

    while (len > 0) {
	(*p)[RED] = getc(my_file);
	(*p)[GRN] = getc(my_file);
	(*p)[BLU] = getc(my_file);
	(*p)[EXP] = getc(my_file);
	if(feof(my_file))
	    throw runtime_error("unexpected eod of file");
	if(ferror(my_file))
	    throw runtime_error("error reading file");
	    
	if ( (*p)[RED] == 1 and (*p)[GRN] == 1 and (*p)[BLU] == 1) {
	    for (i = (*p)[EXP] << rshift; i > 0; i--) {
		*p = *(p-1);
		++p;
		--len;
	    }
	    rshift += 8;
	} else {
	    ++p;
	    --len;
	    rshift = 0;
	}
    }
}

void HdrReader::finish()
{
    // no need to close the file.
    my_file = NULL;
    my_enc_buffer.resize(0);
}

void HdrWriter::init(FILE* f, size_t w, size_t h)
{
    my_file = f;
    char* argv = new char[128];
    strcpy(argv, "hdrio");
    int x = w, y = h;

    newheader("RADIANCE", my_file);
    printargs(1, &argv, my_file);
    delete [] argv;

    fputformat(COLRFMT, my_file);
    fprintf(my_file, "\n");
    fprtresolu(x, y, my_file);
    
    my_enc_buffer.resize(w);
}

void HdrWriter::write_scanline(const vector<Float3>& buf)
{
    rgb_2_realpixels(buf, my_enc_buffer);
    write_encoded();
}

void HdrWriter::write_encoded()
{
    size_t len = my_enc_buffer.size();
    Byte4* scanline = &my_enc_buffer[0];
    size_t i, j, beg, cnt=1;
    size_t c2;

    if (len < MIN_ENC_LENGTH or len > MAX_ENC_LENGTH){
	// OOBs, write out flat
	fwrite(scanline, sizeof(Byte4), len, my_file);
	return;
    }
    /* put magic header */
    putc(2, my_file);
    putc(2, my_file);
    putc(len>>8, my_file);
    putc(len&255, my_file);
    /* put components seperately */
    for (i = 0; i < 4; i++) {
	for (j = 0; j < len; j += cnt) {	/* find next run */
	    for (beg = j; beg < len; beg += cnt) {
		for (cnt = 1; cnt < 127; cnt++){
		    if(beg+cnt >= len)
			break;
		    if(scanline[beg+cnt][i] != scanline[beg][i])
			break;
		}

		if (cnt >= MINRUN)
		    break;			/* long enough */
	    }
	    if (beg-j > 1 && beg-j < MINRUN) {
		c2 = j+1;
		while (scanline[c2++][i] == scanline[j][i])
		    if (c2 == beg) {	/* short run */
			putc(128+beg-j, my_file);
			putc(scanline[j][i], my_file);
			j = beg;
			break;
		    }
	    }
	    while (j < beg) {		/* write out non-run */
		if ((c2 = beg-j) > 128) c2 = 128;
		putc(c2, my_file);
		while (c2--)
		    putc(scanline[j++][i], my_file);
	    }
	    if (cnt >= MINRUN) {		/* write out run */
		putc(128+cnt, my_file);
		putc(scanline[beg][i], my_file);
	    } else
		cnt = 0;
	}
    }
}

void HdrWriter::finish()
{
    my_file = NULL;
    my_enc_buffer.resize(0);
}
