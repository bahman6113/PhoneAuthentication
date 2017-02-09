
// PhoneAuth ConfiguratorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PhoneAuth Configurator.h"
#include "PhoneAuth ConfiguratorDlg.h"
#include "afxdialogex.h"
#include <DSRole.h>
#include "RegistryHelper.h"
#include "CryptoHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "netapi32.lib")

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPhoneAuthConfiguratorDlg dialog



CPhoneAuthConfiguratorDlg::CPhoneAuthConfiguratorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PHONEAUTHCONFIGURATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhoneAuthConfiguratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USER, m_edUser);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edPass);
	DDX_Control(pDX, IDC_COMBO_DOMAIN, m_cmbDomain);
}

BEGIN_MESSAGE_MAP(CPhoneAuthConfiguratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_STATIC_PASSWORD, &CPhoneAuthConfiguratorDlg::OnStnClickedStaticPassword)
	ON_BN_CLICKED(IDOK, &CPhoneAuthConfiguratorDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPhoneAuthConfiguratorDlg message handlers

BOOL CPhoneAuthConfiguratorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitCommonControls();
	m_cmbDomain.AddString(L"");
	//Checking if there is any domain
	DSROLE_PRIMARY_DOMAIN_INFO_BASIC * info;
	DWORD dw;
	
	dw = DsRoleGetPrimaryDomainInformation(NULL, DsRolePrimaryDomainInfoBasic, (PBYTE *)&info);
	if (dw != ERROR_SUCCESS)
	{
		AfxMessageBox(L"Error in getting domain information!", MB_ICONERROR);
		return TRUE;
	}
	if (info->DomainNameDns)
	{
		m_cmbDomain.AddString(info->DomainNameDns);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPhoneAuthConfiguratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPhoneAuthConfiguratorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPhoneAuthConfiguratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPhoneAuthConfiguratorDlg::OnStnClickedStaticPassword()
{
	// TODO: Add your control notification handler code here
}


void CPhoneAuthConfiguratorDlg::OnBnClickedOk()
{
	CString strUser = L"", strPass = L"", strDomain = L"";
	size_t uiOutSize = 0;
	BYTE *pbEncdata = NULL;
	CHAR *pbMBPass = NULL;
	m_edUser.GetWindowTextW(strUser);
	m_edPass.GetWindowTextW(strPass);

	if(m_cmbDomain.GetCurSel() != CB_ERR)
		m_cmbDomain.GetLBText(m_cmbDomain.GetCurSel(), strDomain);
	
	if (strDomain.IsEmpty())
		strDomain = L".";

	if (strUser.IsEmpty() || strPass.IsEmpty())
	{
		AfxMessageBox(L"Please enter credentials first!", MB_ICONEXCLAMATION);
		return;
	}
	

	HANDLE hUser = NULL;
	if (!LogonUser(strUser, strDomain, strPass, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hUser))
	{
		DWORD dwErr = GetLastError();
		CString strMsg = L"";
		if (dwErr == ERROR_LOGON_FAILURE)
			strMsg = L"Error! Invalid username or password";
		else
			strMsg = "Error in checking user credential";
		
		AfxMessageBox(strMsg, MB_ICONERROR);
		return;
	}

	CloseHandle(hUser);
	
	uiOutSize = CryptoHelper::EncOutputSize(strPass.GetLength());
	try
	{
		pbEncdata = new BYTE[uiOutSize];
		pbMBPass = new CHAR[strPass.GetLength() + 1];
		memset(pbMBPass, 0, strPass.GetLength() + 1);
	}
	catch (...)
	{
		AfxMessageBox(L"Memory Allocation Error!", MB_ICONERROR);
		return;
	}
	
	size_t uiConverted = 0;
	wcstombs_s(&uiConverted, pbMBPass, strPass.GetLength() + 1, strPass.GetBuffer(), strPass.GetLength());
	if (CryptoHelper::Encryptor((unsigned char *) pbMBPass, pbEncdata, strPass.GetLength(), true) != 0)
	{
		AfxMessageBox(L"Encryption Error!", MB_ICONERROR);
		delete[] pbEncdata;
		delete[] pbMBPass;
		return;
	}

	CString strEncodedPass = CString(CryptoHelper::Base64Encode(pbEncdata, uiOutSize));

	if (RegistryHelper::SetRegCredential(strUser, strDomain, strEncodedPass))
	{
		AfxMessageBox(L"Error in writing to registry! Please run the program as administrator", MB_ICONERROR);
		delete[] pbEncdata;
		delete[] pbMBPass;
		return;
	}

	if(pbEncdata)
		delete[] pbEncdata;
	if(pbMBPass)
		delete[] pbMBPass;

	CDialogEx::OnOK();
}
