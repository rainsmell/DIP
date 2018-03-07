
// DIPDoc.h : CDIPDoc 类的接口
//


#pragma once
#include "Img.h"
#include "ImgProcess.h"

class CDIPDoc : public CDocument
{
protected: // 仅从序列化创建
	CDIPDoc();
	DECLARE_DYNCREATE(CDIPDoc)

// 特性
public:

// 操作
public:
	CSize GetDocSize() const
	{
		return m_sizeDoc;
	}

	CPalette* GetDocPalette() const
	{
		return m_pPalette;
	}

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	CImgProcess m_Image;
	CImgProcess m_OImage;

	CPalette *m_pPalette;

	void Init();

	virtual ~CDIPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

protected:
	CSize m_sizeDoc;
};
