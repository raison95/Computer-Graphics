#pragma once
class Circle
{
public:
	char color;				// 0 : white, 1 : black
	float y, x;					// �߽��� y��ǥ, x��ǥ
	bool isIn;					// �ٵϾ��� �ٵ��� �ȿ� ���ԵǴ°�?
	float momentum;		// ���
	float vy, vx;				// ����϶� ���⺤��(ũ��� �׻�1�̴�.)
public:
	Circle(char color);
	void move(float dy, float dx);										// dy,dx��ŭ �߽��� �̵�(���� �̵�)
	void setMoveValue(float momentum, float vy, float vx);		// ��� �� ������� ����
	void drawCircle();														// �ٵϾ��� �׸���
	void setPoint(float y, float x);										// �߽���ǥ�� ����
	Circle();
	~Circle();
};

