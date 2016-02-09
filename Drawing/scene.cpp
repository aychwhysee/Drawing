#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include "scene.h"

int TransformNode::count = 0;
map<int, TransformNode*> idNodeTable;

TransformNode::TransformNode(TransformNode* p)
{
	identifier1 = count;
	parent = p;
	shape = NULL;
	transformMatrix = new Matrix();
	selected = false;
	count++;
	idNodeTable[identifier1] = this;
	//children = children;
}

TransformNode::TransformNode(TransformNode* p, ShapeNode* s, Matrix* t)
{
	identifier1 = count;
	parent = p;
	shape = s;
	transformMatrix = t;
	selected = false;
	count++;
	idNodeTable[identifier1] = this;
	//children = children;
}


TransformNode::~TransformNode()
{
	delete shape;
	delete transformMatrix;
	for (list<TransformNode*>::iterator iter = children.begin();
	iter != children.end();
		iter++)
	{
		delete *iter;
	}
}

Matrix* TransformNode::CWT() const
{
	if (getParent() == NULL)
	{
		return getTransform();
	}
	else {
		return parent->CWT()->multiply(transformMatrix);
	}
}

void TransformNode::translate(double deltaX, double deltaY)
{
	Matrix* trM = Matrix::translation(deltaX, deltaY);
	Matrix* thisIn = CWT()->getInverse();
	Matrix* thisIn2 = thisIn->multiply(trM)->multiply(CWT());
	transformMatrix = transformMatrix->multiply(thisIn2);
}

void TransformNode::rotate(double theta)
{
	Matrix* trM = Matrix::rotation(theta);
	Matrix* thisIn = CWT()->getInverse();
	Matrix* thisIn2 = thisIn->multiply(trM)->multiply(CWT());
	transformMatrix = transformMatrix->multiply(thisIn2);
}

void TransformNode::shear(double shearXY, double shearYX)
{
	Matrix* trM = Matrix::shearing(shearXY, shearYX);
	Matrix* thisIn = CWT()->getInverse();
	Matrix* thisIn2 = thisIn->multiply(trM)->multiply(CWT());
	transformMatrix = transformMatrix->multiply(thisIn2);
}

void TransformNode::scale(double scaleX, double scaleY)
{
	Matrix* trM = Matrix::scaling(scaleX, scaleY);
	Matrix* thisIn = CWT()->getInverse();
	Matrix* thisIn2 = thisIn->multiply(trM)->multiply(CWT());
	transformMatrix = transformMatrix->multiply(thisIn2);
}

void TransformNode::draw(bool displayHelpers) const
{
	glPushName(identifier1);
	gPush(transformMatrix);
	bool highlight = getHighlight();
	if (selected)
	{
		setHighlight(true);
	}
	if (shape) shape->draw();
	for (list<TransformNode*>::const_iterator iter = children.cbegin();
		iter != children.cend();
		++iter)
	{
		(*iter)->draw(displayHelpers);
	}
	setHighlight(highlight);
	gPop();
	glPopName();
}

TransformNode* TransformNode::getParent() const 
{ 
	return parent;
}

void TransformNode::setParent(TransformNode* p) 
{ 
	parent = p;
}

void TransformNode::changeParent(TransformNode* newParent)
{
	list<TransformNode*>::const_iterator iter;
	for (iter = children.cbegin();
		iter != children.cend();
		++iter)
	{
		parent->removeChild(*iter);
		//(*iter).transform = newParent->getTransform();
		//need CWT? Use inverse function...?
		//in lecture notes but hard to tell how to actually do it
		newParent->addChild(*iter);
	}
}

void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
	//?????
	TransformNode* something = new TransformNode(NULL);
	set<TransformNode*>::const_iterator iter;
	for (iter = groupMembers.cbegin();
		iter != groupMembers.cend();
		++iter)
	{
		something->addChild(*iter);
	}
}

Matrix* TransformNode::getTransform() const
{
	return transformMatrix;
}

TransformNode* TransformNode::clone() const
{
	TransformNode* clone = new TransformNode(parent, shape, transformMatrix);
	(*clone).parent = NULL;
	//(*clone).shape = shape;
	//(*clone).transformMatrix = transformMatrix;
	//(*clone).children = children;
	//^ is this part even necessary?
	return clone;
}

void TransformNode::addChild(TransformNode* child)
{
	children.push_back(child);
}

void TransformNode::removeChild(TransformNode* child)
{
	children.remove(child);
}

TransformNode* TransformNode::firstChild() const
{
	return children.front();
}

TransformNode* TransformNode::lastChild() const
{
	return children.back();
}

TransformNode* TransformNode::nextChild(TransformNode* child) const
{
	bool isMember = (find(children.cbegin(), children.cend(), child) != children.cend());
	list<TransformNode*>::const_iterator iter;
	if (isMember)
	{
		for (iter = children.cbegin();
			iter != children.cend();
			++iter)
		{
			if (child == *iter)
				break;
		}
			iter++;
	}
	return *iter;
}

TransformNode* TransformNode::previousChild(TransformNode* child) const
{
   bool isMember = (find(children.cbegin(), children.cend(), child) != children.cend());
   list<TransformNode*>::const_iterator iter;
   if (isMember)
   {
	   for (iter = children.cbegin();
	   iter != children.cend();
		   ++iter)
	   {
		   if (child == *iter)
			   break;
	   }
	   iter--;
   }
   return *iter;
}


void TransformNode::select() 
{
	selected = true;
}

void TransformNode::deSelect() 
{
	selected = false;
}

TransformNode* TransformNode::nodeLookup(int identifier)
{
	return idNodeTable[identifier];
}


ShapeNode::ShapeNode(colorType c) : color(c)
{
	transformnode = NULL;
}

void ShapeNode::setTransformNode(TransformNode* tn)
{
	transformnode = tn;
}

TransformNode* ShapeNode::getTransformNode()
{
	return transformnode;
}

Line::Line(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{
}


ShapeNode* Line::clone() const
{
	return NULL;
	//Line* clone = new Line(x0, y0, x1, y1, color);
	//clone->draw();
	//return clone;
}

void Line::draw() const
{
	drawLine(x0, y0, x1, y1);
	setColor(color);
}



Rectangle::Rectangle(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{
}


ShapeNode* Rectangle::clone()  const
{
   return NULL;
   //Rectangle* clone = new Rectangle(x0, y0, x1, y1, color);
   //clone->draw();
   //return clone;
}

void Rectangle::draw() const
{
	drawRectangle(x0, y0, x1, y1);
	setColor(color);
}

Circle::Circle(double ccX, double ccY, double r, colorType c)
	: ShapeNode(c), cX(ccX), cY(ccY), radius(r)
{
}

ShapeNode* Circle::clone() const
{
   return NULL;
   //Circle* clone = new Circle(cX, cY, radius, color);
   //clone->draw();
   //return clone;
}

void Circle::draw() const
{
	drawCircle(cX, cY, radius);
	setColor(color);
}


Polygon::Polygon(const list<Vector*>& vs, colorType c) 
	: ShapeNode(c), vertices(vs)
{
}

Polygon::~Polygon()
{
	//vertices.clear;
	//delete vertices;
}

ShapeNode* Polygon::clone() const
{
   return NULL;
   //Polygon* clone = new Polygon(vertices, closed);
   //clone->draw();
   //return clone;
}

void Polygon::draw() const
{
	drawPolygon(vertices, closed); //where "closed" is a bool, but what?
	setColor(color);
}

