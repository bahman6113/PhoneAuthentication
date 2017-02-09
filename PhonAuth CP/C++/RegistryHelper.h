#pragma once
#define MAX_USER_LEN	256
#define MAX_PASS_LEN	256
#define MAX_DOMAIN_LEN	256
#include <atlstr.h>

class RegistryHelper
{
public:
	static int GetRegCredential(CString &strUser, CString &strDomain, CString &strEncPass); //this method retrieves stored credentials from registry
	static int SetRegCredential(CString strUser, CString strDomain, CString strEncPass); //this method stores credentials into registry
};

