
// DIPView.h : CDIPView ��Ľӿ�
//

#pragma once


class CDIPView : public CView
{
protected: // �������л�����
	CDIPView();
	DECLARE_DYNCREATE(CDIPView)

// ����
public:
	CDIPDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CDIPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileClean();
	afx_msg void OnViewIntensity();
	afx_msg void OnPointLiner();
	afx_msg void OnPointLog();
	afx_msg void OnPointGamma();
	afx_msg void OnThreshold();
	afx_msg void OnPointParLineTran();
	afx_msg void OnPointHistEq();
	afx_msg void OnPointHistst();
};

#ifndef _DEBUG  // DIPView.cpp �еĵ��԰汾
inline CDIPDoc* CDIPView::GetDocument() const
   { return reinterpret_cast<CDIPDoc*>(m_pDocument); }
#endif

