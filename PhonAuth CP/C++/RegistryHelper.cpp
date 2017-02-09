#include "RegistryHelper.h"

#define KEY_NAME L"SOFTWARE\\PhoneAuthentication"
#define KEY_USER_VAL	"UCred"
#define KEY_PASS_VAL	"EPCred"
#define KEY_DOMAIN_VAL	"DCred"


int RegistryHelper::GetRegCredential(CString &strUser, CString &strDomain, CString &strPass)
{
	CString cSvar = _T("");
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_NAME, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		return 1;

	BYTE szUser[MAX_USER_LEN] = { 0 };
	BYTE szDomain[MAX_DOMAIN_LEN] = { 0 };
	BYTE szPass[MAX_PASS_LEN] = { 0 };

	DWORD dwKeyDataType;
	DWORD dwDataBufSize = MAX_USER_LEN;
	if (RegQueryValueExA(hKey, KEY_USER_VAL, NULL, &dwKeyDataType, szUser, &dwDataBufSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 2;
	}

	strUser = CString(szUser);
	dwDataBufSize = MAX_PASS_LEN;
	if (RegQueryValueExA(hKey, KEY_PASS_VAL, NULL, &dwKeyDataType, szPass, &dwDataBufSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 2;
	}

	dwDataBufSize = MAX_DOMAIN_LEN;
	if (RegQueryValueExA(hKey, KEY_DOMAIN_VAL, NULL, &dwKeyDataType, szDomain, &dwDataBufSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 3;
	}

	RegCloseKey(hKey);
	strPass = CString(szPass);
	strDomain = CString(szDomain);


	return 0;
}

int RegistryHelper::SetRegCredential(CString strUser, CString strDomain, CString strPass)
{
	CString cSvar = _T("");
	HKEY hKey;
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, KEY_NAME, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
		return 1;
	

	BYTE pByteUser[MAX_USER_LEN] = { 0 };
	BYTE pBytePass[MAX_PASS_LEN] = { 0 };
	BYTE pByteDomain[MAX_DOMAIN_LEN] = { 0 };
	size_t dwConverted = 0;

	wcstombs_s(&dwConverted, (CHAR *)pByteUser, MAX_USER_LEN, strUser.GetBuffer(), MAX_USER_LEN);
	if (RegSetValueExA(hKey, KEY_USER_VAL, 0, REG_SZ, (BYTE *)pByteUser, (strUser.GetLength() + 1) * sizeof(CHAR)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 2;
	}

	wcstombs_s(&dwConverted, (CHAR *)pBytePass, MAX_USER_LEN, strPass.GetBuffer(), MAX_PASS_LEN);

	if (RegSetValueExA(hKey, KEY_PASS_VAL, 0, REG_SZ, pBytePass, (strPass.GetLength() + 1) * sizeof(wchar_t)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 2;
	}

	wcstombs_s(&dwConverted, (CHAR *)pByteDomain, MAX_USER_LEN, strDomain.GetBuffer(), MAX_DOMAIN_LEN);

	if (RegSetValueExA(hKey, KEY_DOMAIN_VAL, 0, REG_SZ, pByteDomain, (strDomain.GetLength() + 1) * sizeof(wchar_t)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 2;
	}

	RegCloseKey(hKey);

	return 0;
}
