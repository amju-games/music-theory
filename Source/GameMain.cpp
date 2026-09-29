// * PIANO FEST *
// (c) Copyright 2026 Juliet Colman

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
