#pragma once
class Circle
{
public:
	char color;				// 0 : white, 1 : black
	float y, x;					// 중심의 y좌표, x좌표
	bool isIn;					// 바둑알이 바둑판 안에 포함되는가?
	float momentum;		// 운동량
	float vy, vx;				// 운동중일때 방향벡터(크기는 항상1이다.)
public:
	Circle(char color);
	void move(float dy, float dx);										// dy,dx만큼 중심을 이동(원을 이동)
	void setMoveValue(float momentum, float vy, float vx);		// 운동량 및 운동방향을 설정
	void drawCircle();														// 바둑알을 그린다
	void setPoint(float y, float x);										// 중심좌표를 설정
	Circle();
	~Circle();
};

