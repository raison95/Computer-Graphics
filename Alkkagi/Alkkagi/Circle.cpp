#include "stdafx.h"
#include "Circle.h"


Circle::Circle(char color) {
	this->color = color;
	y = x = -100;
	isIn = true;
}

void Circle::setMoveValue(float momentum, float vy, float vx) {			
	this->momentum = momentum;
	this->vx = vx;
	this->vy = vy;
}

void Circle::move(float dy, float dx) {												
	y += dy;
	x += dx;
}

void Circle::setPoint(float y, float x) {											
	this->y = y;
	this->x = x;
}

void Circle::drawCircle() {
	if (this->color == 0)																// color (0 : white, 1 : black)
		glColor3f(1, 1, 1);
	else
		glColor3f(0, 0, 0);

	double rad = 0.5;																	// rad은 반지름

	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		double angle = i * 3.141592 / 180;										// i의 단위는 라디안
		double X = rad * cos(angle);					
		double Y = rad * sin(angle);						
		glVertex2f(X + x, Y + y);														// 중심의 좌표 (x,y)로 부터 (X,Y) 떨어진 점을 그린다 (360번 돌때, 원을 이룬다.)
	}
	glEnd();
}

Circle::Circle()
{
	y = x = -100;
	isIn = true;
}

Circle::~Circle()
{
}
