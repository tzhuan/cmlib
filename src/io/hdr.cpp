#include <stdexcept>
#include <cstdio>

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
}

void HdrReader::init(FILE* f, size_t& w, size_t& h)
{

}

void HdrReader::read_scanline(vector<Float3>& buf)
{

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


