
// TimedShutdownDlg.h: 头文件
//

#pragma once


// CTimedShutdownDlg 对话框
class CTimedShutdownDlg : public CDialogEx
{
// 构造
public:
	CTimedShutdownDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIMEDSHUTDOWN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChangehour();
	afx_msg void OnEnChangeminute();
	afx_msg void OnBnClickedOk();
};
