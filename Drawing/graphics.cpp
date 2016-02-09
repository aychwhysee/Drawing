#include "graphics.h"
#include "drawing.h"

#include <GL/glut.h>

#include <cfloat>
#include <cmath>

using namespace std;

const colorType HIGHLIGHT_COLOR = WHITE;

const double TRANSFORM_GISMO_SCALE = 25.0;


bool highlight = false;

bool getHighlight()
{
  return highlight;
}

void setHighlight(bool h)
{
  if (h) setColor(HIGHLIGHT_COLOR);
  highlight = h;
}


colorType intToColor(int item)
{
  switch ( item ) {
    case 0 : return BLACK   ;
    case 1 : return BLUE    ;
    case 2 : return GREEN   ;
    case 3 : return CYAN    ;
    case 4 : return RED     ;
    case 5 : return MAGENTA ;
    case 6 : return YELLOW  ;
    case 7 : return WHITE   ;
    case 8 : return GRAY    ;
   default : return BLACK   ;
  }
}


void setColor( colorType color )
{
  if (!highlight)
    switch ( color )
    {
      case BLACK   : glColor3f( 0.0, 0.0, 0.0 ); break;
      case BLUE    : glColor3f( 0.0, 0.0, 1.0 ); break;
      case GREEN   : glColor3f( 0.0, 1.0, 0.0 ); break;
      case CYAN    : glColor3f( 0.0, 1.0, 1.0 ); break;
      case RED     : glColor3f( 1.0, 0.0, 0.0 ); break;
      case MAGENTA : glColor3f( 1.0, 0.0, 1.0 ); break;
      case YELLOW  : glColor3f( 1.0, 1.0, 0.0 ); break;
      case WHITE   : glColor3f( 1.0, 1.0, 1.0 ); break;
      case GRAY    : glColor3f( 0.5, 0.5, 0.5 ); break;
    }
}



TransformStack transformStack;

TransformStack::TransformStack()
{
	Matrix* idenM = new Matrix();
	stack.push(idenM); //? stack vs transformStack?
	transformStack.push(idenM); //?
}

void TransformStack::push(Matrix* transform)
{
	Matrix* something = transformStack.top();
	Matrix* result = something->multiply(transform);
	stack.push(result);
}

void TransformStack::pop()
{
	stack.pop();
}

Matrix* TransformStack::top()
{
	//return NULL; //should return the top of the stack (a matrix*)
	return stack.top();
	//return transformStack.top();
}


void gPush(Matrix* transform)
{
	transformStack.push(transform);
}

void gPop()
{
	transformStack.pop();
}

void drawLine(double x0, double y0, double x1, double y1)
{
	Vector* p0 = new Vector(x0, y0);
	Vector* p1 = new Vector(x1, y1);
	drawLine(p0, p1);
}

void drawLine(Vector* p0, Vector* p1)
{
	Matrix* transformM = transformStack.top();
	Vector* p00 = 
		transformM->multiply(p0);
	Vector* p11 = 
		transformM->multiply(p1);
	double x00 = p00->operator[](0);
	double y00 = p00->operator[](1);
	double x11 = p11->operator[](0);
	double y11 = p11->operator[](1);
	glBegin(GL_LINES);
	glVertex2d(x00, y00);
	glVertex2d(x11, y11);
	glEnd();
	glFlush();
}

void drawRectangle(double x0, double y0, double x1, double y1)
{
	Vector* p0 = new Vector(x0, y0);
	Vector* p1 = new Vector(x1, y0);
	Vector* p2 = new Vector(x1, y1);
	Vector* p3 = new Vector(x0, y1);
	drawLine(p0, p1);
	drawLine(p1, p2);
	drawLine(p2, p3);
	drawLine(p3, p0);
}

void drawCircle(double x0, double y0, double x1, double y1)
{
	double ccX = x0;
	double ccY = y0;
	double cradius = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	drawCircle(ccX, ccY, cradius);
}

void drawCircle(double cX, double cY, double radius)
{
	double theta, delta;
	delta = TWO_PI / CIRCLE_SECTIONS;
	for (theta = 0; theta < 2 * 3.14159; theta += delta)
	{
		Vector* p0 = new Vector(cX + radius*cos(theta), cY + radius*sin(theta));
		Vector* p1 = new Vector(cX + radius*cos(theta + delta), cY + radius*sin(theta + delta));
		drawLine(p0, p1);
	}
}

void drawPolygon(const list<Vector*>& vertices, bool close)
{
	//?? what
	//for (list<Vector*>::const_iterator iter = vertices.cbegin();
	//iter != vertices.cend(); //cend() minus one though?
	//	++iter)
	//{
	//	Vector* p0 = (*iter);
	//	Vector* p1 = (*iter + 1);
	//	drawLine(p0, p1);
	//}
	int lengthy = vertices.size();
	list<Vector*>::const_iterator iter = vertices.cbegin();
	for (int i = 0; i < lengthy; ++i)
	{
		Vector* p0 = *iter;
		iter++;
		Vector* p1 = *iter;
		drawLine(p0, p1);
	}
}

void drawTransformGismo()
{
  double halfSize = TRANSFORM_GISMO_SCALE/2.0;
  drawRectangle(-halfSize,-halfSize,halfSize,halfSize);
}
