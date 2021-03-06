
// AlkkagiView.h: CAlkkagiView 클래스의 인터페이스
//

#pragma once


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

	Player player[2];		// 1p이용시 0은 컴퓨터, 1은 사용자.
	Circle*circle;				// 1p이용시 앞에 절반은 컴퓨터, 뒤에 절반은 사용자
	int circleNum;			// 처음시작할 때의 알의 개수
	int whichTurn;			// 1:사용자 턴, 2:컴퓨터 턴

// 작업입니다.
public:
	BOOL SetDevicePixelFormat(HDC hdc);
	void InitGL(GLvoid);
	void drawBackground(void);
	void ReSizeGLScene(GLsizei width, GLsizei height);
	void DrawGLScene(void);
	void computerModule(void);
	bool circleBoundCheck(Circle&circle);
	void drawAllCircle();
	bool isSetMoveValue;
	bool moveCircle();
	bool collisionCheck(float x1, float y1, float x2, float y2);
	bool collision();
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
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void fiveCircle();
	afx_msg void SixCircle();
	afx_msg void sevenCircle();
};

#ifndef _DEBUG  // AlkkagiView.cpp의 디버그 버전
inline CAlkkagiDoc* CAlkkagiView::GetDocument() const
   { return reinterpret_cast<CAlkkagiDoc*>(m_pDocument); }
#endif

