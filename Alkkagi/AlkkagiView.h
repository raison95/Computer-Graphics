
// AlkkagiView.h: CAlkkagiView 클래스의 인터페이스
//

#pragma once

#define ONE_PLAYER_MODE 1
#define TWO_PLAYER_MODE 2

class CAlkkagiView : public CView
{
protected: // serialization에서만 만들어집니다.
	CAlkkagiView();
	DECLARE_DYNCREATE(CAlkkagiView)

// 특성입니다.
public:
	HDC m_hDC;
	HGLRC m_hglRC;
	CAlkkagiDoc* GetDocument() const;

	
	int whichTurn;			// 1p이용시 0:컴퓨터 턴, 1:사용자 턴
	Circle*circle;				// 1p이용시 앞에 절반은 컴퓨터, 뒤에 절반은 사용자
	int circleNum;			// 처음시작할 때의 바둑알의 개수

	float point_x, point_y;	// 드래그할때 시작점(바둑알의 중심)을 저장하기 위한 변수
	float moving_x, moving_y;// 드래그할때의 x,y값을 저장하기 위한 변수
	bool is_circle_pick;// 날릴 바둑알을 정했는가?
	bool is_draging;// 지금 드래그 중인가?(알을 집어서 바둑알을 날리기위한 준비중인가?)
	int picked_circle_num;// pick한 바둑알의 인덱스(1~10)
	float w_width, w_height;// window의 width와 height를 저장
	int switch_cnt;
	int gamemode;// 몇인용 게임인지?
	bool cntPicked;// 몇개의 바둑알로 할것인지 선택여부.
	bool is_mode_picked;// 몇인용게임인지 선택여부.
	int start, dest;

	
// 작업입니다.
public:
	BOOL SetDevicePixelFormat(HDC hdc);
	void InitGL(GLvoid);
	void drawBackground(void);
	void ReSizeGLScene(GLsizei width, GLsizei height);
	void DrawGLScene(void);
	void computerModule(void);
	void computerModule2(void);
	void drawAllCircle();
	bool isSetMoveValue;
	bool moveCircle();
	void collision();
	bool collisionCheck(float x1, float y1, float x2, float y2);
	void drawLight(void);//빛그려주는함수
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CAlkkagiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void fiveCircle();
	afx_msg void SixCircle();
	afx_msg void sevenCircle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void On32774();		// 1p 이용
	afx_msg void On32776();		// 2p 이용
};

#ifndef _DEBUG  // AlkkagiView.cpp의 디버그 버전
inline CAlkkagiDoc* CAlkkagiView::GetDocument() const
   { return reinterpret_cast<CAlkkagiDoc*>(m_pDocument); }
#endif

