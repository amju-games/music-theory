// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <gtest/gtest.h>

int main(int argc, char * argv[])
{
  ::testing::InitGoogleTest(&argc, argv);

  //// Set up GL context so we can do tests involving OpenGL
  //glutInit(&argc, argv);
  //glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
  //glutInitWindowSize(100, 100);
  //glutCreateWindow("Hello tests");

  return RUN_ALL_TESTS();
}


