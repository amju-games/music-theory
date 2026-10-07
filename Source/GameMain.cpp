// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include

#if defined(WIN32) && !defined(LISTEN) && !defined(CATCH)
#define AMJU_USE_OPENGL
//#define AMJU_USE_GLUT
//#include <main.h>
// Use WinMain.cpp
#endif

#if defined(MACOSX) && !defined(LISTEN) && !defined(CATCH)
#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT
#include <main.h>
#endif
