// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_IDLE,           // ״̬��ָʾ��
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
:m_inited(FALSE)
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��������ͣ������ɾ��������
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
	// ������ִ���
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

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


// CMainFrame ��Ϣ�������


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

	// TODO: �Զ������չ�˴����Դ�����ͼ���˵��е�ѡ��

	CRSAUIView* pView = GetRightPane(); 

	// ����Ҵ�����δ�������߲�����ͼ��
	// ���ڷ�Χ�ڽ�������

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// ��������� ID_VIEW_LINEUP����ֻ���ڴ���
		// LVS_ICON �� LVS_SMALLICON ģʽʱ����������

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// ����ʹ�õ�������ӳ��ͼ����ʽ
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

	// TODO: �Զ������չ�˴����Դ�����ͼ���˵��е�ѡ��
	CRSAUIView* pView = GetRightPane();

	// ����Ҵ����Ѵ��������� CRSAUIView��
	// ����˵�����...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// Ҫ���б�ؼ����������
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// ������������б�ؼ��ϵ���ʽ
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

		// ������ʽ�����ڽ��Զ����»���
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	if(m_inited)
	{
		ResizeSplitter();
		ShowStatus(m_current_status_txt);
	}

	
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���

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
