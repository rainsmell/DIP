
// DIPView.h : CDIPView 类的接口
//

#pragma once


class CDIPView : public CView
{
protected: // 仅从序列化创建
	CDIPView();
	DECLARE_DYNCREATE(CDIPView)

// 特性
public:
	CDIPDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDIPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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
	afx_msg void OnGemoTran();
	afx_msg void OnGemoHorMirror();
	afx_msg void OnGemoVerMirror();
	afx_msg void OnGemoTranspose();
	afx_msg void OnGemoScale();
	afx_msg void OnGemoRotate();
	afx_msg void OnEnhaSmooth();
	afx_msg void OnEnhaMedian();
	afx_msg void OnEnhaAdaptiveMeidanFilter();
	afx_msg void OnEnhaGradSobel();
	afx_msg void OnEnhaGradLaplacian();
	afx_msg void OnEnhanceFilter();
	afx_msg void OnFreFilterFFT2();
	afx_msg void OnFreFilterIFFT2();
	afx_msg void OnFreqIdealLPF();
	afx_msg void OnFreqGaussFilter();
	afx_msg void OnFreqGaussHPF();
	afx_msg void OnFreqLaplace();
	afx_msg void OnAddUniform();
	afx_msg void OnAddGaussian();
	afx_msg void OnAddSlatPepper();
	afx_msg void OnAddRayleigh();
	afx_msg void OnInvTuihua();
	afx_msg void OnInvFilter();
	afx_msg void OnFreqWienerFilter();
	afx_msg void OnColorRGB2CMY();
	afx_msg void OnColorRGB2HSI();
	afx_msg void OnColorHSI2RGB();
	afx_msg void OnColorRGB2HSV();
	afx_msg void OnColorHSV2RGB();
	afx_msg void OnColorRGB2YUV();
	afx_msg void OnColorYUV2RGB();
	afx_msg void OnColorRGB2YIQ();
	afx_msg void OnColorYIQ2RGB();
	afx_msg void OnDctAll();
	afx_msg void OnIdctAll();
	afx_msg void OnHuffcode();
};

#ifndef _DEBUG  // DIPView.cpp 中的调试版本
inline CDIPDoc* CDIPView::GetDocument() const
   { return reinterpret_cast<CDIPDoc*>(m_pDocument); }
#endif

