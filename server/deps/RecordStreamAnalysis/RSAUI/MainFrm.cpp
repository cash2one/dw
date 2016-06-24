// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "RSAUI.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "RSAUIView.h"
#include "ProtocolView.h"
#include "CommandView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnViewStyle)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CMainFrame::OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMainFrame::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, &CMainFrame::OnUpdateAppAbout)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CMainFrame::OnUpdateFileSaveAs)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_IDLE,           // 状态行指示器
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
:m_inited(FALSE)
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	ShowStatus(ID_IDLE);

	return 0;
}

void CMainFrame::ShowStatus(DWORD dwState)
{	
	CRect rect;
	GetClientRect(&rect);
	m_current_status_txt = dwState;
	m_wndStatusBar.SetPaneInfo(0, dwState, 0, rect.Width());
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// 创建拆分窗口
	if (!m_wndSplitter.CreateStatic(this, 1, 3))
		return FALSE;

	m_wndSplitter.CreateView(0, 0, pContext->m_pNewViewClass, CSize(200, 50), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CRSAUIView), CSize(100, 0), pContext);

	m_commandSplitter.CreateStatic(&m_wndSplitter, 2, 1, WS_VISIBLE | WS_CHILD, 
		m_wndSplitter.IdFromRowCol(0, 2));

	m_commandSplitter.CreateView(0, 0, RUNTIME_CLASS(CCommandView), CSize(150, 210), pContext);
	m_commandSplitter.CreateView(1, 0, RUNTIME_CLASS(CProtocolView), CSize(150, 0), pContext);


	ResizeSplitter();

	m_inited = true;

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序


CRSAUIView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CRSAUIView* pView = DYNAMIC_DOWNCAST(CRSAUIView, pWnd);
	return pView;
}

void CMainFrame::ResizeSplitter()
{
	CRect rect;
	GetClientRect(&rect);
	m_wndSplitter.SetColumnInfo(0, rect.Width() * 4 / 20, 100);
	m_wndSplitter.SetColumnInfo(1, rect.Width() * 10 / 20, 100);
	m_wndSplitter.SetColumnInfo(2, rect.Width() * 6 / 20, 150);
	m_wndSplitter.RecalcLayout();
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	if (!pCmdUI)
		return;

	// TODO: 自定义或扩展此代码以处理“视图”菜单中的选项

	CRSAUIView* pView = GetRightPane(); 

	// 如果右窗格尚未创建或者不是视图，
	// 则在范围内禁用命令

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// 如果命令是 ID_VIEW_LINEUP，则只有在处于
		// LVS_ICON 或 LVS_SMALLICON 模式时才启用命令

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// 否则，使用点线来反映视图的样式
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{	

	return;

	// TODO: 自定义或扩展此代码以处理“视图”菜单中的选项
	CRSAUIView* pView = GetRightPane();

	// 如果右窗格已创建而且是 CRSAUIView，
	// 则处理菜单命令...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// 要求列表控件与网格对齐
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// 其他命令更改列表控件上的样式
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// 更改样式；窗口将自动重新绘制
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if(m_inited)
	{
		ResizeSplitter();
		ShowStatus(m_current_status_txt);
	}

	
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类

	if(UM_LOADFINISH == message)
	{
		ShowStatus(ID_LOAD_END);
	}
	else if( UM_LOADSTART == message)
	{
		ShowStatus(ID_LOAD_START);
	}

	return CFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateAppAbout(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}
