#include <cstdio>
#include <cassert>

#include "gil/Exception.h"
#include "gil/io/crw.h"

extern "C" {
extern int ext_identify(FILE *f);
extern void ext_init(size_t *w, size_t *h, size_t *m);
extern void ext_read(unsigned short(*row_pointers)[4]);
}

using namespace gil;

template<template<typename, typename> class Converter>
void CrwReader<Converter>::check(FILE *f)
{
    assert(f);
    if (!ext_identify(f))
	throw InvalidFormat("unsupported format in CrwReader::check()");
}

template<template<typename, typename> class Converter>
void CrwReader<Converter>::init()
{
    ext_init(&my_width, &my_width, &my_meta_length);
}

template<template<typename, typename> class Converter>
void CrwReader<Converter>::read(unsigned short (*row_pointers)[4])
{
    ext_read(row_pointers);
}

template<template<typename, typename> class Converter>
void CrwReader<Converter>::finish()
{
}
