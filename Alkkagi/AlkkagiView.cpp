
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
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_32771, &CAlkkagiView::fiveCircle)
	ON_COMMAND(ID_32772, &CAlkkagiView::SixCircle)
	ON_COMMAND(ID_32773, &CAlkkagiView::sevenCircle)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_32774, &CAlkkagiView::On32774)
	ON_COMMAND(ID_32776, &CAlkkagiView::On32776)
END_MESSAGE_MAP()

// CAlkkagiView 생성/소멸
bool whowinbull = false;
CAlkkagiView::CAlkkagiView()
{
	
	// TODO: 여기에 생성 코드를 추가합니다.
	whichTurn = 0;
	srand(time(NULL));
	isSetMoveValue = true;
	is_circle_pick = false;
	is_draging = false;
	cntPicked = false;
	is_mode_picked = false;
	switch_cnt = 0;
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

void CAlkkagiView::drawLight(void) {

	glEnable(GL_LIGHTING);
	glTranslatef(0.0f, 0.0f, 27.0f);
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 }; //0이니벡터

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);

	glTranslatef(0.0f, 0.0f, -27.0f);

}

void CAlkkagiView::ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;

	w_width = width, w_height = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

float getSize(float x, float X, float y, float Y) {
	return sqrt(pow(x - X, 2) + pow(y - Y, 2));
}

void CAlkkagiView::computerModule2(void) {
	bool found = false;
	for (int i = 0; i < circleNum; ++i)
		if (circle[i].isIn) {
			start = i;
			found = true;
			break;
		}
	if (!found)
		return;

	found = false;
	float minimum = 987654321;
	for (int i = circleNum; i < circleNum * 2; ++i)
		if (circle[i].isIn) {
			if (getSize(circle[start].x, circle[i].x, circle[start].y, circle[i].y) < minimum) {
				minimum = getSize(circle[start].x, circle[i].x, circle[start].y, circle[i].y);
				dest = i;
				found = true;
			}
		}
	if (!found)
		return;
}

void CAlkkagiView::computerModule(void) {
	if (!circle[start].isIn) return;
	float fromX = circle[start].x, fromY = circle[start].y;	// 컴퓨터가 정한 바둑알의 중심좌표 저장	

	float newMomentum = rand() % 15 + 15;		// 운동량 난수발생

	float angle = ((float)(rand() % 361)*3.1416) / 180;// 목표 바둑알의 방향벡터를 설정하기 위한 각도 난수발생
	float toX = circle[dest].x + cos(angle);
	float toY = circle[dest].y + sin(angle);

	toX -= fromX,toY -= fromY;// toX,toY는 컴퓨터의 방향벡터

	float vectorSize = getSize(toX, fromX, toY, fromY);// toX,toY는 방향벡터의 크기

	circle[start].setMoveValue(newMomentum, toY / vectorSize, toX / vectorSize);
	// 컴퓨터가 정한 바둑알의 운동량, 방향벡터(단위벡터) 설정
	isSetMoveValue = false;// 운동량을 한번만 설정하기위한 플래그(drawglscene의 반복성고려)
}

bool CAlkkagiView::moveCircle() {
	bool ret = false;
	for (int i = 0; i < circleNum * 2; ++i) {
		int y = circle[i].y, x = circle[i].x;
		if (!(-9 <= y && y <= 9 && -9 <= x && x<= 9))   // 매 움직임마다 범위를 체크, 바둑판에서 벗어났으면 플래그처리
			circle[i].isIn = false;

		if (circle[i].momentum < 0)
			circle[i].momentum = 0;

		if (circle[i].momentum > 0) { // 운동량이 양수인경우, 즉 움직임이 예상되는 경우 방향벡터의 실수배만큼 움직이고
			ret = true; // 운동량을 감소시켜 결국에는 0이되면 물체는 정지하게된다.
			if ((gamemode == ONE_PLAYER_MODE) && (i >= circleNum)) switch_cnt = 0; // switch_cnt는 턴이 바뀔때 한번만 바뀌게하는 플래그
			circle[i].move(circle[i].vy*0.08, circle[i].vx*0.08);
			circle[i].momentum -=0.1;
		}
	}
	return ret;
}

int firstmessagebox = 0;
int who_win = 0;



void CAlkkagiView::DrawGLScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.f, 0.f, 27.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);


	
	drawLight();

	if (firstmessagebox == 0) {
		MessageBoxA(NULL, "바둑알의 갯수와 게임모드를 선택하세요", "Input", MB_OK); //변경
		firstmessagebox++;
	}
	if (who_win == 1) {
		MessageBoxA(NULL, "검은돌 이 이겼습니다.", "Input", MB_OK); //변경
		who_win = -1;
		whowinbull = true;
	}
	else if (who_win == 2) {
		MessageBoxA(NULL, "흰돌이 이겼습니다.", "Input", MB_OK); //변경
		who_win = -1;
		whowinbull = true;
	}

	if (cntPicked) {// 바둑알의 개수를 정한경우
		if (is_mode_picked) {	 	// 1인 vs 2인을 정한경우
			drawBackground();
			bool isMoving = moveCircle();// 바둑알을 움직인다.
			if (!isMoving) {		// 움직임이 없는경우
				if (gamemode == ONE_PLAYER_MODE) {// 1p이용시
					if (whichTurn == 0 && (switch_cnt==0)) {// 턴을 넘겨주는 부분
						whichTurn = 1;
						switch_cnt++;
					}
					else if (whichTurn == 1 && (switch_cnt == 0)) {// 턴을 넘겨주는 부분
						Sleep(3000);
						isSetMoveValue = true;
						whichTurn = 0;
					}
				}
				else {
					whichTurn = (whichTurn + 1) % 2;// 턴을 넘겨주는 부분
				}
			}
			if (gamemode == ONE_PLAYER_MODE) {// 1p 이용시
				if (whichTurn == 0) {		// 컴퓨터 차례라면
					computerModule2();// start와 dest를 정하고
					if (isSetMoveValue) {// 한 턴에서 한번만 실행하기위한 플래그
						computerModule();// 컴퓨터 모듈을 실행함.
					}
				}
				else {// 사용자 차례라면
					if (is_circle_pick&&is_draging) {// 드래그를 실시해서 운동량 및 방향벡터를 설정한다.

						GLfloat dragline_mat[] = { 1.0f,1.0f,1.0f,1.0f };
						GLfloat drag_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
						GLfloat drag_shiniess[] = { 128.0f };
						GLfloat drag_specular[] = { 1.0f,1.0f,1.0f,1.0f };

						glMaterialfv(GL_FRONT, GL_AMBIENT, dragline_mat);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, drag_diffuse);
						glMaterialfv(GL_FRONT, GL_SPECULAR, drag_shiniess);
						glMaterialfv(GL_FRONT, GL_SHININESS, drag_specular);

						glLineWidth(2.0f);
						glColor3f(1, 1, 1);
						glBegin(GL_LINES);
						glVertex3f(point_x, point_y, 0);
						glVertex3f(moving_x, moving_y, 0);
						glEnd();
					}
				}
			}
			else {		// 사용자 차례라면
				if (is_circle_pick&&is_draging) {// 드래그를 실시해서 운동


					GLfloat dragline_mat[] = { 1.0f,1.0f,1.0f,1.0f };
					GLfloat drag_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
					GLfloat drag_shiniess[] = { 128.0f };
					GLfloat drag_specular[] = { 1.0f,1.0f,1.0f,1.0f };

					glMaterialfv(GL_FRONT, GL_AMBIENT, dragline_mat);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, drag_diffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, drag_shiniess);
					glMaterialfv(GL_FRONT, GL_SHININESS, drag_specular);

					glLineWidth(2.0f);
					glColor3f(1, 1, 1);
					glBegin(GL_LINES);
					glVertex3f(point_x, point_y, 0);
					glVertex3f(moving_x, moving_y, 0);
					glEnd();
				}
			}
			drawAllCircle();// 바둑알을 그린다.
			collision();// 충돌처리를 하는부분 ( 충돌이 났다면 운동량을 적당히 분배함)
		}
	}
	SwapBuffers(m_hDC);
}

void CAlkkagiView::collision()
{
	// TODO: 여기에 구현 코드 추가.
		for (int i = 0; i < circleNum*2; i++) {
			if (circle[i].momentum > 0) {												// i가 운동량이 있는 경우
				for (int j =0; j < circleNum * 2; j++) {							// j는 체크의 대상(i를 제외한 모든 알)
					if (circle[j].momentum == 0) {										// i가 운동량이 존재하고 j는 정지한 상태일때
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
}

bool CAlkkagiView::collisionCheck(float x1, float y1, float x2, float y2)
{
	// TODO: 여기에 구현 코드 추가.
	if (getSize(x1, x2, y1, y2) <= 1) return true;
	return false;
}

void CAlkkagiView::drawBackground(void) {

	GLfloat no_mat[] = { 0.6f,0.45f,0.12f,1.0f };
	GLfloat mat_diffuse[] = { 0.6f,0.45f,0.12f,1.0f };
	GLfloat low_shiniess[] = { 128.0f };
	GLfloat specular[] = { 1.0f,1.0f,1.0f,1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shiniess); //바둑판 재질


	glBegin(GL_QUADS);
	glColor3f(0.6, 0.45, 0.12);
	glVertex3f(-10.0, -10.0, 0.0);
	glVertex3f(-10.0, 10.0, 0.0);
	glVertex3f(10.0, 10.0, 0.0);
	glVertex3f(10.0, -10.0, 0.0);
	glEnd();
	

	GLfloat line_mat[] = { 0.0f,0.0f,0.0f,1.0f };
	GLfloat line_diffuse[] = { 0.0f,0.0f,0.0f,1.0f };
	GLfloat line_shiniess[] = { 128.0f };
	GLfloat line_specular[] = { 1.0f,1.0f,1.0f,1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, line_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, line_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, line_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, line_shiniess); //선재질

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
		if (circle[i].isIn)						// 바둑알이 유효하다면(바둑판위에 존재한다면) 바둑알을 그린다.
		{

			if (i < circleNum) { //흰색
				GLfloat baduk_mat[] = { 1.0f,1.0f,1.0f,1.0f };
				GLfloat baduk_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
				GLfloat baduk_shiniess[] = { 128.0f };
				GLfloat baduk_specular[] = { 1.0f,1.0f,1.0f,1.0f };

				glMaterialfv(GL_FRONT, GL_AMBIENT, baduk_mat);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, baduk_diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, baduk_shiniess);
				glMaterialfv(GL_FRONT, GL_SHININESS, baduk_specular);

			}
			else {//검은색
				GLfloat baduk_mat[] = { 0.0f,0.0f,0.0f,1.0f };
				GLfloat baduk_diffuse[] = { 0.0f,0.0f,0.0f,1.0f };
				GLfloat baduk_shiniess[] = { 128.0f };
				GLfloat baduk_specular[] = { 1.0f,1.0f,1.0f,1.0f };

				glMaterialfv(GL_FRONT, GL_AMBIENT, baduk_mat);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, baduk_diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, baduk_shiniess);
				glMaterialfv(GL_FRONT, GL_SHININESS, baduk_specular);
			}
			circle[i].drawCircle();

		}

	int count1 = 0;
	int count2 = 0;
	if (!whowinbull) {
		for (int i = 0; i < 2 * circleNum; i++) {
			if (i < circleNum) {
				if (!circle[i].isIn)
					count1++;
			}
			else {
				if (!circle[i].isIn)
					count2++;
			}
		}
		if (count1 == circleNum) {
			who_win = 1;
		}
		else if (count2 == circleNum) {
			who_win = 2;
		}
	}
	
}


void CAlkkagiView::fiveCircle()
{
	// 5개의 바둑알로 시작할 경우
	cntPicked = true;
	circle = new Circle[10];
	circleNum = 5;
	
	const int dy[5] = { 7,3,0,-3,-7 }, dx[5] = { 7,4,7,4,7 };
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	drawBackground();
	// 바둑알의 색깔 및 위치 초기설정
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
	// 6개의 바둑알로 시작할 경우
	cntPicked = true;
	circle = new Circle[12];
	circleNum = 6;
	
	const int dy[6] = { 7,3,0,-3,-7,0 }, dx[6] = { 7,4,7,4,7,2 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBackground();
	// 바둑알의 색깔 및 위치 초기설정
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

	// 7개의 바둑알로 시작할 경우
	cntPicked = true;
	circle = new Circle[14];
	circleNum = 7;
	
	const int dy[7] = { 7,4,0,-4,-7,-2 ,2 }, dx[7] = { 7,5,7,5,7,2 ,2 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBackground();
	// 바둑알의 색깔 및 위치 초기설정
	for (int i = 0; i < 7; ++i) {
		circle[i].color = 0;
		circle[i + 7].color = 1;
		circle[i].setPoint(dy[i], dx[i]);
		circle[i + 7].setPoint(dy[i], -dx[i]);
	}

	SwapBuffers(m_hDC);
}

void CAlkkagiView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnLButtonDown(nFlags, point);
	picked_circle_num = 0;										
	float min_distance = 987654321;
	float x_temp = (point.x - w_width / 2.0)*(18.0 / (w_width / 2.0));
	float y_temp = -(point.y - w_height / 2.0)*(11.0 / (w_height / 2.0));// 마우스의 왼쪽버튼을 눌렀을때의 좌표값

	// 마우스의 왼쪽버튼을 눌렀을때 가장 가까운 바둑알을 고른다.
	for (int i = 0; i < circleNum * 2; i++) {
		if (getSize(circle[i].x, x_temp, circle[i].y, y_temp) < min_distance) {
			min_distance = getSize(circle[i].x, x_temp, circle[i].y, y_temp);
			picked_circle_num = i;
		}
	}
	point_x = circle[picked_circle_num].x, point_y = circle[picked_circle_num].y;
	is_circle_pick = true;
}

void CAlkkagiView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnMouseMove(nFlags, point);
	float x_temp = (point.x - w_width / 2.0)*(18.0 / (w_width / 2.0));
	float y_temp = -(point.y - w_height / 2.0)*(11.0 / (w_height / 2.0));	// 마우스를 움직일때(드래그중일때)의 좌표값

	moving_x = x_temp; moving_y = y_temp;
	is_draging = true;
}

void CAlkkagiView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnLButtonUp(nFlags, point);
	float x_temp = (point.x - w_width / 2.0)*(18.0 / (w_width / 2.0));
	float y_temp = -(point.y - w_height / 2.0)*(11.0 / (w_height / 2.0));	// 마우스의 왼쪽버튼을 떼었을때의 좌표값

	// 바둑알의 중심으로부터 왼쪽버튼을 떼었을때의 좌표값을 이은 벡터의 방향과 
	// 그 벡터의 크기를 기준으로 바둑알을 날리기위한 초기화과정
	float temp_vx = point_x - x_temp, temp_vy = point_y - y_temp;
	float temp_size = getSize(0, temp_vx, 0, temp_vy);
	temp_vx /= temp_size;
	temp_vy /= temp_size;
	circle[picked_circle_num].vx = temp_vx;
	circle[picked_circle_num].vy = temp_vy;
	circle[picked_circle_num].momentum = temp_size*2;
	is_circle_pick = false;
	is_draging = false;
	isSetMoveValue = true;
}

void CAlkkagiView::On32774()
{
	// ONE PLAYER
	gamemode = ONE_PLAYER_MODE;
	is_mode_picked = true;
}

void CAlkkagiView::On32776()
{
	// TWO PLAYER
	gamemode = TWO_PLAYER_MODE;
	is_mode_picked = true;
}
