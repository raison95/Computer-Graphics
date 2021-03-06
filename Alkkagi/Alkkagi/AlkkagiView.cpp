
// AlkkagiView.cpp: CAlkkagiView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Alkkagi.h"
#endif

#include "AlkkagiDoc.h"
#include "AlkkagiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAlkkagiView

IMPLEMENT_DYNCREATE(CAlkkagiView, CView)

BEGIN_MESSAGE_MAP(CAlkkagiView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAlkkagiView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_32771, &CAlkkagiView::fiveCircle)
	ON_COMMAND(ID_32772, &CAlkkagiView::SixCircle)
	ON_COMMAND(ID_32773, &CAlkkagiView::sevenCircle)
END_MESSAGE_MAP()

// CAlkkagiView 생성/소멸

CAlkkagiView::CAlkkagiView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	whichTurn = 2;
	srand(time(NULL));
	isSetMoveValue = true;
}

CAlkkagiView::~CAlkkagiView()
{
}

BOOL CAlkkagiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CAlkkagiView 그리기

void CAlkkagiView::OnDraw(CDC* /*pDC*/)
{
	CAlkkagiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	DrawGLScene();
	Invalidate(FALSE);
}

// CAlkkagiView 인쇄


void CAlkkagiView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CAlkkagiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CAlkkagiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CAlkkagiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CAlkkagiView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAlkkagiView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CAlkkagiView 진단

#ifdef _DEBUG
void CAlkkagiView::AssertValid() const
{
	CView::AssertValid();
}

void CAlkkagiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAlkkagiDoc* CAlkkagiView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAlkkagiDoc)));
	return (CAlkkagiDoc*)m_pDocument;
}
#endif //_DEBUG


// CAlkkagiView 메시지 처리기

BOOL CAlkkagiView::SetDevicePixelFormat(HDC hdc) {
	int pixelformat;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_GENERIC_FORMAT |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		8,
		0,
		8,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	//choose best matching pixel format
	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE) {
		MessageBox(LPCTSTR("ChoosePixelFormat failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}

	// set pixel format to device context
	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) {
		MessageBox(LPCTSTR("SetPixelFormat failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}
	return TRUE;
}

int CAlkkagiView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_hDC = GetDC()->m_hDC;

	if (!SetDevicePixelFormat(m_hDC))
		return -1;

	m_hglRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hglRC);

	InitGL();

	return 0;
}


void CAlkkagiView::OnDestroy()
{
	CView::OnDestroy();

	wglMakeCurrent(m_hDC, NULL);
	wglDeleteContext(m_hglRC);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CAlkkagiView::InitGL(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


void CAlkkagiView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ReSizeGLScene(cx, cy);
}

void CAlkkagiView::ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;

	//glutInitWindowSize(500, 500);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool cntPicked = false;

float getSize(float x, float X, float y, float Y) {
	return sqrt(pow(x - X, 2) + pow(y - Y, 2));
}

void CAlkkagiView::computerModule(void) {
	Sleep(300);
	int start, dest;
	bool found = false;
	for (int i = 0; i<circleNum; ++i)
		if (circle[i].isIn) {
			start = i;
			found = true;
			break;
		}
	if (!found)
		return;
	found = false;
	for (int i = circleNum; i<circleNum * 2; ++i)
		if (circle[i].isIn) {
			dest = i;
			found = true;
			break;
		}
	if (!found)
		return;

	float fromX = circle[start].x, fromY = circle[start].y;

	//int momentumPower = rand() % 1000 + 1;
	//float newMomentum = (float)momentumPower / 10000;
	float newMomentum = rand() % 18 + 1;

	float angle = ((float)(rand() % 361)*3.1416) / 180;
	float toX = circle[dest].x + cos(angle);
	float toY = circle[dest].y + sin(angle);

	toX -= fromX,toY -= fromY;

	float vectorSize = getSize(toX, fromX, toY, fromY);

	circle[start].setMoveValue(newMomentum, toY / vectorSize, toX / vectorSize);

	isSetMoveValue = false;
}

bool CAlkkagiView::moveCircle() {
	bool ret = false;
	for (int i = 0; i < circleNum * 2; ++i) {
		int y = circle[i].y, x = circle[i].x;
		if (!(-9 <= y && y <= 9 && -9 <= x && x<= 9))
			circle[i].isIn = false;

		if (circle[i].momentum < 0)
			circle[i].momentum = 0;

		if (circle[i].momentum > 0) {
			ret = true;
			circle[i].move(circle[i].vy*0.7, circle[i].vx*0.7);
			circle[i].momentum -= 1;
		}
	}
	return ret;
}

void CAlkkagiView::DrawGLScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.f, 0.f, 27.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

	if (cntPicked) {
		player[0].playerNum = 0;
		player[1].playerNum = 1;

		drawBackground();

		bool isMoving = moveCircle();
		if (!isMoving) {
			if (whichTurn == 1) {
				whichTurn = 2;
				isSetMoveValue = true;
			}
			else if (whichTurn == 2) {
				whichTurn = 1;
			}
		}

		if (cntPicked)
			drawAllCircle();

		if (whichTurn == 2) {
			if (isSetMoveValue) {
				computerModule();
			}
		}
		collision();
	}
	SwapBuffers(m_hDC);
}

bool CAlkkagiView::collision()
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < circleNum * 2; i++) {
		if (circle[i].momentum > 0) {												// i가 운동량이 있는 경우
			for (int j = i + 1; j < circleNum * 2; j++) {							// j는 체크의 대상(i를 제외한 모든 알)
				if (circle[j].momentum==0) {										// i가 운동량이 존재하고 j는 정지한 상태일때
					if (collisionCheck(circle[i].x, circle[i].y, circle[j].x, circle[j].y)) {			// 충돌이 일어난경우(바둑알과 바둑알의 교점이 존재할때)
						float v2_x = circle[j].x - circle[i].x;
						float v2_y = circle[j].y - circle[i].y;

						float v_size = getSize(0, v2_x, 0, v2_y);						// 벡터를 단위벡터로
						v2_x /= v_size;
						v2_y /= v_size;
						
						float theta = acos(													// theta는 (1)충돌직전 움직임이 있던 바둑알의 운동방향벡터와
							(circle[i].vx*v2_x + circle[i].vy*v2_y)						// (2)충돌시 두 바둑알의 중심을 이은 벡터(정지해 있던 물체가 나아갈 운동방향벡터)
							/ getSize(0, circle[i].vx, 0, circle[i].vy)						// (1)과 (2) 두 벡터간의 각도
							/ getSize(0, v2_x, 0, v2_y)
						);

						float temp_momentum = circle[i].momentum;             // 운동량 임시저장.
						circle[i].momentum = temp_momentum * sin(theta);		// 운동량의 분배
						circle[j].momentum = temp_momentum * cos(theta);	//			〃

						float temp_x = circle[i].vx, temp_y = circle[i].vy;            // 방향단위벡터 임시저장.

						circle[j].vx = cos(theta)*v2_x;									// 벡터의 분해시 직각의 성질을 이용하기 위해 임시적으로 cos을 곱함.
						circle[j].vy = cos(theta)*v2_y;					
						circle[i].vx = (temp_x - circle[j].vx) / getSize(temp_x, circle[j].vx, temp_y, circle[j].vy);
						circle[i].vy = (temp_y - circle[j].vy) / getSize(temp_x, circle[j].vx, temp_y, circle[j].vy);
						circle[j].vx /= cos(theta);											// 벡터의 크기를 다시 1로 만들어줌.
						circle[j].vy /= cos(theta);
					}
				}
			}
		}
	}
	return false;
}

bool CAlkkagiView::collisionCheck(float x1, float y1, float x2, float y2)
{
	// TODO: 여기에 구현 코드 추가.
	if (getSize(x1, x2, y1, y2) <= 1) return true;
	return false;
}

bool CAlkkagiView::circleBoundCheck(Circle&circle) {
	float y = circle.y, x = circle.x;
	bool ret = -9 <= y && y <= 9 && -9 <= x && x <= 9;
	if (!ret)
		circle.isIn = false;
	return ret;
}

void CAlkkagiView::drawBackground(void) {
	glBegin(GL_QUADS);
	glColor3f(0.6, 0.45, 0.12);
	glVertex3f(-10.0, -10.0, 0.0);
	glVertex3f(-10.0, 10.0, 0.0);
	glVertex3f(10.0, 10.0, 0.0);
	glVertex3f(10.0, -10.0, 0.0);
	glEnd();
	
	glLineWidth(2.0f);
	glColor3f(0, 0, 0);

	//horizontal
	for (int i = 0,line=-9; i < 20; ++i,++line) {
		glBegin(GL_LINES);
		glVertex3f(-9, line, 0);
		glVertex3f(9, line, 0);
		glEnd();
	}

	//vertical
	for (int i = 0,line=-9; i < 20; ++i,++line) {
		glBegin(GL_LINES);
		glVertex3f(line, -9, 0);
		glVertex3f(line, 9, 0);
		glEnd();
	}
}

void CAlkkagiView::drawAllCircle() {
	for (int i = 0; i < circleNum * 2; ++i)
		if (circle[i].isIn)
			circle[i].drawCircle();
}

void CAlkkagiView::fiveCircle()
{
	cntPicked = true;
	circle = new Circle[10];
	circleNum = 5;
	player[0].leftCircleNum = player[1].leftCircleNum = 5;
	const int dy[5] = { 7,3,0,-3,-7 }, dx[5] = { 7,4,7,4,7 };
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	drawBackground();
	
	for (int i = 0; i < 5; ++i) {
		circle[i].color = 0;
		circle[i + 5].color = 1;
		circle[i].setPoint(dy[i], dx[i]);
		circle[i + 5].setPoint(dy[i], -dx[i]);
	}
	
	SwapBuffers(m_hDC);
}


void CAlkkagiView::SixCircle()
{
	cntPicked = true;
	circle = new Circle[12];
	circleNum = 6;
	player[0].leftCircleNum = player[1].leftCircleNum = 6;
	const int dy[6] = { 7,3,0,-3,-7,0 }, dx[6] = { 7,4,7,4,7,2 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBackground();

	for (int i = 0; i < 6; ++i) {
		circle[i].color = 0;
		circle[i + 6].color = 1;
		circle[i].setPoint(dy[i], dx[i]);
		circle[i + 6].setPoint(dy[i], -dx[i]);
	}

	SwapBuffers(m_hDC);
}


void CAlkkagiView::sevenCircle()
{
	cntPicked = true;
	circle = new Circle[14];
	circleNum = 7;
	player[0].leftCircleNum = player[1].leftCircleNum = 7;
	const int dy[7] = { 7,4,0,-4,-7,-2 ,2 }, dx[7] = { 7,5,7,5,7,2 ,2 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBackground();

	for (int i = 0; i < 7; ++i) {
		circle[i].color = 0;
		circle[i + 7].color = 1;
		circle[i].setPoint(dy[i], dx[i]);
		circle[i + 7].setPoint(dy[i], -dx[i]);
	}

	SwapBuffers(m_hDC);
}
