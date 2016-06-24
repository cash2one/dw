#pragma once
#include "afxwin.h"
#include "ColorStatic.h"

#include "datetimeedit.h"
#include "Spin.h"


// CCommandView 窗体视图

class CCommandView : public CFormView
{
	DECLARE_DYNCREATE(CCommandView)

protected:
	CCommandView();           // 动态创建所使用的受保护的构造函数
	virtual ~CCommandView();

public:
	enum { IDD = IDD_COMMANDVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
private:
	CButton m_query;
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);


private:
	void ShowInfo(UINT color, const TCHAR* info);
	bool CheckValid();

	Date TimeDuration(time_t t);

private:
	CColorStatic m_output;
	CComboBox m_option;
	
	FilterEdit::CDateTimeEdit m_start_time;
	FilterEdit::CSpin m_start_time_spin;

	FilterEdit::CDateTimeEdit m_end_time;
	FilterEdit::CSpin m_end_time_spin;	
	
public:
	virtual void OnInitialUpdate();
	
private:
	CEdit m_guid;
public:
	afx_msg void OnBnClickedQuery();
};


