
// TimedShutdownDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TimedShutdown.h"
#include "TimedShutdownDlg.h"
#include "afxdialogex.h"
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTimedShutdownDlg 对话框



CTimedShutdownDlg::CTimedShutdownDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIMEDSHUTDOWN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimedShutdownDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTimedShutdownDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(hour, &CTimedShutdownDlg::OnEnChangehour)
	ON_EN_CHANGE(minute, &CTimedShutdownDlg::OnEnChangeminute)
	ON_BN_CLICKED(OK, &CTimedShutdownDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTimedShutdownDlg 消息处理程序

BOOL CTimedShutdownDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CFont  timesFont;
	timesFont.CreatePointFont(180, _T("微软雅黑"));
	GetDlgItem(hour)->SetFont(&timesFont);
	GetDlgItem(hour)->SetWindowTextW(_T("12"));
	GetDlgItem(timeDivide)->SetFont(&timesFont);
	GetDlgItem(minute)->SetFont(&timesFont);
	GetDlgItem(minute)->SetWindowTextW(_T("00"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTimedShutdownDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTimedShutdownDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CTimedShutdownDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE://ESC键
			return true;
		case VK_RETURN://Enter键
			return true;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTimedShutdownDlg::OnEnChangehour()
{
	UpdateData(true);
	CString hourString;
	GetDlgItem(hour)->GetWindowTextW(hourString);
	hourString.ReleaseBuffer();
	if (_ttoi(hourString) > 23)
	{
		GetDlgItem(hour)->SetWindowTextW(_T("23"));
	}
}

void CTimedShutdownDlg::OnEnChangeminute()
{
	UpdateData(true);
	CString minuteString;
	GetDlgItem(minute)->GetWindowTextW(minuteString);
	minuteString.ReleaseBuffer();
	if (_ttoi(minuteString) > 59)
	{
		GetDlgItem(minute)->SetWindowTextW(_T("59"));
	}
}

typedef struct Time
{
	bool flag;
	int Hour;
	int Minute;
}Time;

DWORD WINAPI ShutDownThread(LPVOID lpParam);
Time localTime;

void CTimedShutdownDlg::OnBnClickedOk()
{
	CString cmd;
	GetDlgItem(OK)->GetWindowTextW(cmd);
	cmd.ReleaseBuffer();

	LPDWORD shutDownThreadID = NULL;
	if (cmd == L"执行")
	{
		CString hourString;
		GetDlgItem(hour)->GetWindowTextW(hourString);
		hourString.ReleaseBuffer();

		CString minuteString;
		GetDlgItem(minute)->GetWindowTextW(minuteString);
		minuteString.ReleaseBuffer();

		localTime.Hour = _ttoi(hourString);
		localTime.Minute = _ttoi(minuteString);
		localTime.flag = true;
		CreateThread(NULL, 0, &ShutDownThread, &localTime, 0, shutDownThreadID);

		GetDlgItem(hour)->EnableWindow(false);
		GetDlgItem(minute)->EnableWindow(false);
		GetDlgItem(OK)->SetWindowTextW(_T("撤销"));
	}
	else
	{
		localTime.flag = false;
		GetDlgItem(hour)->EnableWindow(true);
		GetDlgItem(minute)->EnableWindow(true);
		GetDlgItem(OK)->SetWindowTextW(_T("执行"));
	}
}

DWORD WINAPI ShutDownThread(LPVOID lpParam)
{
	Time *localTime = (Time *)lpParam;

	while (localTime->flag)
	{
		struct tm t;
		time_t now;
		time(&now);
		localtime_s(&t, &now);
		if (t.tm_hour == localTime->Hour && t.tm_min == localTime->Minute)
		{
			system("shutdown -s -t 10");
			break;
		}

		Sleep(10000);
	}
	return 0;
}