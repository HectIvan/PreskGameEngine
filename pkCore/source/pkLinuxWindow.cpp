#ifdef PK_PLATFORM_LINUX

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <gl/GLU.h>
#include "pkWindow.h"
#include "pkWindowDesc.h"

namespace pkEngineSDK
{

void
Window::create(PKWindowDesc& _desc, String _name)
{
  GLuint(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  GluInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(_desc.width, _desc.height);
  glutInitWindowPosition(_desc.posX, _desc.posY);
  glutCreateWindow(_name);
}
}

#endif // PK_PLATFORM_LINUX