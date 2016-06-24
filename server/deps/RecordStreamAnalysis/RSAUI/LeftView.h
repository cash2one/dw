// LeftView.h : CLeftView 类的接口
//


#pragma once

class CRSAUIDoc;

class CLeftView : public CTreeView
{
protected: // 仅从序列化创建
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// 属性
public:
	CRSAUIDoc* GetDocument();

// 操作
public:

// 重写
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HTREEITEM	m_root;

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMKillfocus(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // LeftView.cpp 中的调试版本
inline CRSAUIDoc* CLeftView::GetDocument()
   { return reinterpret_cast<CRSAUIDoc*>(m_pDocument); }
#endif

