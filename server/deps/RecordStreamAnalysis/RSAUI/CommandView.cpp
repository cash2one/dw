// CommandView.cpp : 实现文件
//

#include "stdafx.h"
#include "RSAUI.h"
#include "CommandView.h"
#include "RSAUIDoc.h"

#include <time.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;


#define INFO_START_LOAD		_T("正在加载sqlite文件进cache...")
#define INFO_END_LOAD		_T("sqlite文件加载完成.")
#define INFO_INPUT_INVALID	_T("查询条件不合法.")
#define INFO_START_QUERY	_T("开始查询...")
#define INFO_END_QUERY		_T("查询完成.")

extern UINT WM_XEDIT_KILL_FOCUS;


// CCommandView

IMPLEMENT_DYNCREATE(CCommandView, CFormView)

CCommandView::CCommandView()
	: CFormView(CCommandView::IDD)
{

}

CCommandView::~CCommandView()
{
}

void CCommandView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUERY, m_query);
	DDX_Control(pDX, IDC_OUTPUT, m_output);
	DDX_Control(pDX, IDC_COMBO1, m_option);
	DDX_Control(pDX, IDC_INPUT_GUID, m_guid);
}

BEGIN_MESSAGE_MAP(CCommandView, CFormView)
	ON_BN_CLICKED(IDC_QUERY, &CCommandView::OnBnClickedQuery)
END_MESSAGE_MAP()


// CCommandView 诊断

#ifdef _DEBUG
void CCommandView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCommandView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCommandView 消息处理程序

void CCommandView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: 在此添加专用代码和/或调用基类

}

BOOL CCommandView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(message)
	{
	case UM_LOADSTART:
		m_query.EnableWindow(FALSE);
		ShowInfo(LIGHTRED, INFO_START_LOAD);
		break;

	case UM_LOADFINISH:
		m_query.EnableWindow(TRUE);
		ShowInfo(LIGHTBLUE, INFO_END_LOAD);
		break;

	case UM_QUERYFINISH:
		ShowInfo(LIGHTBLUE, INFO_END_QUERY);
		break;
	}

	return CFormView::OnWndMsg(message, wParam, lParam, pResult);
}

void CCommandView::ShowInfo(UINT color, const TCHAR* info)
{
	m_output.SetTextColor(color);
	m_output.SetWindowText(info);
}

void CCommandView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	m_start_time.SubclassDlgItem(IDC_START_TIME, this);
	m_start_time.SetBackgroundColourFocus (RGB (150, 200, 255));
	m_start_time_spin.SubclassDlgItem(IDC_SPIN_START, this);


	m_end_time.SubclassDlgItem(IDC_END_TIME, this);
	m_end_time.SetBackgroundColourFocus (RGB (150, 200, 255));
	m_end_time_spin.SubclassDlgItem(IDC_SPIN_END, this);

	m_option.AddString(_T("guid"));
	m_option.AddString(_T("owner guid"));
	m_option.SetCurSel(0);
}

bool CCommandView::CheckValid()
{
// 	if( !m_start_time.IsInputValid())
// 		return false;
// 	if( !m_end_time.IsInputValid())
// 		return false;
// 
// 	if( m_start_time.GetDateTime() >= m_end_time.GetDateTime())
// 		return false;
// 	
	CString strguid;
	m_guid.GetWindowText(strguid);
	if( 0 == strguid.GetLength())
		return false;

	Guid guid = _tstoi64(strguid);
	if( 0 == guid)
		return false;


	return true;
}

void CCommandView::OnBnClickedQuery()
{    
    
    CRSAUIDoc* pDoc = (CRSAUIDoc *)GetDocument();
    pDoc->DispatchMsg(WM_XEDIT_KILL_FOCUS, 0, 0);

	if(!CheckValid())
	{	
		ShowInfo(LIGHTRED, INFO_INPUT_INVALID);
		return;
	}

	ShowInfo(LIGHTBLUE, INFO_START_QUERY);	
    pDoc->m_start_time = 0;
    pDoc->m_end_time = 0;
    
    do 
    {
        if( !m_start_time.IsInputValid())
            break;
        if( !m_end_time.IsInputValid())
            break;

        if( m_start_time.GetDateTime() >= m_end_time.GetDateTime())
            break;
        
        SYSTEMTIME st;
        m_start_time.GetDateTime().GetAsSystemTime(st);
        CTime start_t(st);
        pDoc->m_start_time = TimeDuration(start_t.GetTime());
        m_end_time.GetDateTime().GetAsSystemTime(st);
        CTime end_t(st);
        pDoc->m_end_time = TimeDuration(end_t.GetTime());

    } while (false);

	

	if( 0 == m_option.GetCurSel())
		pDoc->m_is_Guid = true;
	else
		pDoc->m_is_Guid = false;
	
	CString strguid;
	m_guid.GetWindowText(strguid);

	pDoc->m_guid = _tstoi64(strguid);	
	
	pDoc->DispatchMsg(UM_QUERY, 0, 0);
}

Date CCommandView::TimeDuration(time_t t)
{
	
	boost::posix_time::ptime time_now = boost::posix_time::from_time_t(t);
	boost::posix_time::ptime epoch(date(1970, Jan, 1));
	boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse;
	time_elapse = time_now - epoch;

	return time_elapse.total_microseconds();
}

