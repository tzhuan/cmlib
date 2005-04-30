//*****************************************************************
//
// ppm.cpp : routines for ppm/pgm I/O
//
//*****************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char* readPGM(const char* filename, int &width, int &height, int &depth)
{
     char buffer[80];
     int phase = 0;
     int type, maxval;
     int j = 0;
     unsigned char* data;
     FILE* f;

     if ( strcmp( filename, "-" ) == 0 )
     {
		f = stdin;
     }
     else
     {
		f = fopen( filename, "rb" );
		if ( f == NULL )
			return NULL;
     }
     
     while ( phase < 4 )
     {
		fgets( buffer, 80, f );
		if ( buffer[0] == '#' ) continue;
		switch( phase )
		{
			case 0: j = sscanf( buffer, "P%d %d %d %d\n", &type, &width, &height, &maxval ); break;
			case 1: j = sscanf( buffer, "%d %d %d\n", &width, &height, &maxval ); break;
			case 2: j = sscanf( buffer, "%d %d\n", &height, &maxval ); break;
			case 3: j = sscanf( buffer, "%d\n", &maxval ); break;
			case 4: j = 0; break;
		}
		if (j>0)
			phase += j;
     }

     if ( type != 5 )
     {
		fprintf( stderr, "bad input image format.\n" );
		return NULL;
     }

     data = new unsigned char[width*height];
     depth = 1;
     fread( data, width * height, 1, f );

     fclose( f );

     return data;
}

bool writePGM(const char* filename, unsigned char* data, int width, int height, int depth)
{
     if(depth != 1)
	 return false;
     FILE* f=fopen(filename, "wb");
     fprintf( f, "P5\n%d %d\n%d\n", width, height, 255 );
     fwrite( data, width*height, 1, f );
     fclose( f );

	 return true;
}

unsigned char* readPPM(const char* filename, int &width, int &height, int& depth)
{
	char buffer[80];
	int phase = 0;
	int type, maxval;
	int j = 0;
	unsigned char* data;
	FILE* f;

	if ( strcmp( filename, "-" ) == 0 ) 
	{
		f = stdin;
	}
	else 
	{
		f = fopen( filename, "rb" );
		if ( f == NULL )
			return NULL;
	}
     
	while ( phase < 4 ) 
	{
		fgets( buffer, 80, f );
		if ( buffer[0] == '#' ) continue;
		switch ( phase )
		{
			case 0: j = sscanf( buffer, "P%d %d %d %d\n", &type, &width, &height, &maxval ); break;
			case 1: j = sscanf( buffer, "%d %d %d\n", &width, &height, &maxval ); break;
			case 2: j = sscanf( buffer, "%d %d\n", &height, &maxval ); break;
			case 3: j = sscanf( buffer, "%d\n", &maxval ); break;
			case 4: j = 0; break;
		}
		phase += j;
	}

	if ( type != 6 ) {
		fprintf( stderr, "bad input image format.\n" );
		return NULL;
	}

	data = new unsigned char[width * height * 3];
	depth = 3;
	fread( data, width * height, 3, f );

	fclose( f );

	return data;
}

bool writePPM(const char *filename, unsigned char* data, int width, int height, int depth)
{
	if(depth != 3)
	    return false;
	FILE* f=fopen(filename, "wb");
	fprintf( f, "P6\n%d %d\n%d\n", width, height, 255 );
	fwrite( data, width*height, 3, f );
	fclose( f );

	return true;
}
