#pragma once
#pragma intrinsic(memcmp, memcpy,strcpy,strcmp,_stricmp,strlen)
#include <windows.h>
#include <process.h>
#include <winternl.h>
#include <imagehlp.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <windns.h>
#include <dbghelp.h>
#include <winldap.h>
#include <winnetwk.h>
#include <wtsapi32.h>
#include <shlwapi.h>

//KERNEL32
#ifdef BOF
#if 0
WINBASEAPI void * WINAPI VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
WINBASEAPI int WINAPI VirtualFree (LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
HLOCAL WINAPI LocalAlloc (UINT, SIZE_T);
HLOCAL WINAPI LocalFree (HLOCAL);
WINBASEAPI void * WINAPI HeapAlloc (HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
WINBASEAPI LPVOID WINAPI HeapReAlloc (HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes);
WINBASEAPI HANDLE WINAPI GetProcessHeap();
WINBASEAPI BOOL WINAPI HeapFree (HANDLE, DWORD, PVOID);
WINBASEAPI DWORD WINAPI FormatMessageA (DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, LPSTR lpBuffer, DWORD nSize, va_list *Arguments);
WINBASEAPI int WINAPI Kernel32$WideCharToMultiByte (UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar);
WINBASEAPI int WINAPI FileTimeToLocalFileTime (CONST FILETIME *lpFileTime, LPFILETIME lpLocalFileTime);
WINBASEAPI int WINAPI FileTimeToSystemTime (CONST FILETIME *lpFileTime, LPSYSTEMTIME lpSystemTime);
WINBASEAPI int WINAPI GetDateFormatW (LCID Locale, DWORD dwFlags, CONST SYSTEMTIME *lpDate, LPCWSTR lpFormat, LPWSTR lpDateStr, int cchDate);
WINBASEAPI VOID WINAPI GetSystemTimeAsFileTime (LPFILETIME lpSystemTimeAsFileTime);
WINBASEAPI VOID WINAPI GetLocalTime (LPSYSTEMTIME lpSystemTime);
WINBASEAPI WINBOOL WINAPI SystemTimeToFileTime (CONST SYSTEMTIME *lpSystemTime, LPFILETIME lpFileTime);
WINBASEAPI WINBOOL WINAPI SystemTimeToTzSpecificLocalTime (CONST TIME_ZONE_INFORMATION *lpTimeZoneInformation, CONST SYSTEMTIME *lpUniversalTime, LPSYSTEMTIME lpLocalTime);
WINBASEAPI WINBOOL WINAPI GlobalMemoryStatusEx (LPMEMORYSTATUSEX lpBuffer);
WINBASEAPI WINBOOL WINAPI GetDiskFreeSpaceExA (LPCSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller, PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes);
WINBASEAPI HANDLE WINAPI GetCurrentProcess (VOID);
DWORD WINAPI GetCurrentProcessId(VOID);
WINBASEAPI DWORD WINAPI GetLastError (VOID);
WINBASEAPI WINBOOL WINAPI CloseHandle (HANDLE hObject);
WINBASEAPI HANDLE WINAPI CreateThread (LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
WINBASEAPI DWORD WINAPI GetTickCount (VOID);
WINBASEAPI ULONGLONG WINAPI GetTickCount64 (VOID);
WINBASEAPI LPVOID WINAPI CreateFiber (SIZE_T dwStackSize, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter);
WINBASEAPI LPVOID WINAPI ConvertThreadToFiber (LPVOID lpParameter);
WINBASEAPI WINBOOL WINAPI ConvertFiberToThread (VOID);
WINBASEAPI VOID WINAPI DeleteFiber (LPVOID lpFiber);
WINBASEAPI VOID WINAPI SwitchToFiber (LPVOID lpFiber);
WINBASEAPI DWORD WINAPI WaitForSingleObject (HANDLE hHandle, DWORD dwMilliseconds);
WINBASEAPI VOID WINAPI Sleep (DWORD dwMilliseconds);
WINBASEAPI WINBOOL WINAPI DeleteFileW (LPCWSTR lpFileName);
WINBASEAPI HANDLE WINAPI CreateFileW (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WINBASEAPI DWORD WINAPI GetFileSize (HANDLE hFile, LPDWORD lpFileSizeHigh);
WINBASEAPI WINBOOL WINAPI ReadFile (HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
WINBASEAPI HANDLE WINAPI OpenProcess (DWORD dwDesiredAccess, WINBOOL bInheritHandle, DWORD dwProcessId);
WINBASEAPI WINBOOL WINAPI GetComputerNameExW (COMPUTER_NAME_FORMAT NameType, LPWSTR lpBuffer, LPDWORD nSize);
WINBASEAPI int WINAPI lstrlenW (LPCWSTR lpString);
WINBASEAPI LPWSTR WINAPI lstrcatW (LPWSTR lpString1, LPCWSTR lpString2);
WINBASEAPI LPWSTR WINAPI lstrcpynW (LPWSTR lpString1, LPCWSTR lpString2, int iMaxLength);
WINBASEAPI DWORD WINAPI GetFullPathNameW (LPCWSTR lpFileName, DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR *lpFilePart);
WINBASEAPI DWORD WINAPI GetFileAttributesW (LPCWSTR lpFileName);
WINBASEAPI DWORD WINAPI GetCurrentDirectoryW (DWORD nBufferLength, LPWSTR lpBuffer);
WINBASEAPI HANDLE WINAPI FindFirstFileW (LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
WINBASEAPI HANDLE WINAPI FindFirstFileA (const char * lpFileName, LPWIN32_FIND_DATA lpFindFileData);
WINBASEAPI WINBOOL WINAPI FindNextFileW (HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);
WINBASEAPI WINBOOL WINAPI FindNextFileA (HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);
WINBASEAPI WINBOOL WINAPI FindClose (HANDLE hFindFile);
WINBASEAPI VOID WINAPI SetLastError (DWORD dwErrCode);
#define intAlloc(size) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)
#define intRealloc(ptr, size) (ptr) ? HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ptr, size) : HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)
#define intFree(addr) HeapFree(GetProcessHeap(), 0, addr)
#define intZeroMemory(addr,size) memset((addr),0,size)
HGLOBAL WINAPI GlobalAlloc(UINT uFlags, SIZE_T dwBytes);
HGLOBAL WINAPI GlobalFree(HGLOBAL hMem);
LPTCH WINAPI GetEnvironmentStrings();
WINBASEAPI BOOL WINAPI FreeEnvironmentStringsA(LPSTR);
WINBASEAPI DWORD WINAPI ExpandEnvironmentStringsW (LPCWSTR lpSrc, LPWSTR lpDst, DWORD nSize);
WINBASEAPI HANDLE WINAPI CreateToolhelp32Snapshot(DWORD dwFlags,DWORD th32ProcessID);
WINBASEAPI WINBOOL WINAPI Process32First(HANDLE hSnapshot,LPPROCESSENTRY32 lppe);
WINBASEAPI WINBOOL WINAPI Process32Next(HANDLE hSnapshot,LPPROCESSENTRY32 lppe);
WINBASEAPI WINBOOL WINAPI Module32First(HANDLE hSnapshot,LPMODULEENTRY32 lpme);
WINBASEAPI WINBOOL WINAPI Module32Next(HANDLE hSnapshot,LPMODULEENTRY32 lpme);
WINBASEAPI HMODULE WINAPI LoadLibraryA (LPCSTR lpLibFileName);
WINBASEAPI FARPROC WINAPI GetProcAddress (HMODULE hModule, LPCSTR lpProcName);
WINBASEAPI WINBOOL WINAPI FreeLibrary (HMODULE hLibModule);
WINBASEAPI int WINAPI lstrlenA(LPCSTR);
int WINAPI GetLocaleInfoEx(LPCWSTR lpLocaleName, LCTYPE LCType, LPWSTR lpLCData, int cchData);
WINBASEAPI int WINAPI GetSystemDefaultLocaleName(LPWSTR lpLocaleName, int cchLocaleName);
LCID WINAPI LocaleNameToLCID(LPCWSTR lpName, DWORD dwFlags);
int WINAPI GetDateFormatEx(LPCWSTR lpLocaleName, DWORD dwFlags, const SYSTEMTIME *lpData, LPCWSTR lpFormat, LPWSTR lpDateStr, int cchDate, LPCWSTR lpCalendar);


//WTSAPI32
DWORD WINAPI WTSAPI32$WTSEnumerateSessionsA(LPVOID, DWORD, DWORD, PWTS_SESSION_INFO*, DWORD*);
DWORD WINAPI WTSAPI32$WTSQuerySessionInformationA(LPVOID, DWORD, WTS_INFO_CLASS , LPSTR*, DWORD*);
DWORD WINAPI WTSAPI32$WTSFreeMemory(PVOID);

//Iphlpapi.lib
//ULONG WINAPI IPHLPAPI$GetAdaptersInfo (PIP_ADAPTER_INFO AdapterInfo, PULONG SizePointer);
DWORD WINAPI IPHLPAPI$GetAdaptersInfo(PIP_ADAPTER_INFO,PULONG);
DWORD WINAPI IPHLPAPI$GetIpForwardTable (PMIB_IPFORWARDTABLE pIpForwardTable, PULONG pdwSize, WINBOOL bOrder);
DWORD WINAPI IPHLPAPI$GetNetworkParams(PFIXED_INFO,PULONG);
ULONG WINAPI IPHLPAPI$GetUdpTable (PMIB_UDPTABLE UdpTable, PULONG SizePointer, WINBOOL Order);
ULONG WINAPI IPHLPAPI$GetTcpTable (PMIB_TCPTABLE TcpTable, PULONG SizePointer, WINBOOL Order);
ULONG WINAPI IPHLPAPI$GetIpNetTable(PMIB_IPNETTABLE IpNetTable,PULONG SizePointer, BOOL Order);

//MSVCRT
WINBASEAPI char *__cdecl _ultoa(unsigned long _Value,char *_Dest,int _Radix);
WINBASEAPI void *__cdecl calloc(size_t _NumOfElements, size_t _SizeOfElements);
WINBASEAPI void *__cdecl memcpy(void * __restrict__ _Dst,const void * __restrict__ _Src,size_t _MaxCount);
WINBASEAPI int __cdecl memcmp(const void *_Buf1,const void *_Buf2,size_t _Size);
WINBASEAPI void *__cdecl realloc(void *_Memory, size_t _NewSize);
WINBASEAPI void __cdecl free(void *_Memory);
/*
WINBASEAPI void __cdecl memset(void *dest, int c, size_t count);
*/
WINBASEAPI int __cdecl sprintf(char *__stream, const char *__format, ...);
WINBASEAPI int __cdecl vsnprintf(char * __restrict__ d,size_t n,const char * __restrict__ format,va_list arg);
WINBASEAPI int __cdecl _snwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,...);
WINBASEAPI errno_t __cdecl wcscpy_s(wchar_t *_Dst, rsize_t _DstSize, const wchar_t *_Src);
WINBASEAPI size_t __cdecl wcslen(const wchar_t *_Str);
WINBASEAPI size_t __cdecl wcstombs(char * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _MaxCount);
/*
WINBASEAPI wchar_t *__cdecl wcscmp(const wchar_t *_lhs,const wchar_t *_rhs);
*/
/*
WINBASEAPI wchar_t *__cdecl wcstok(wchar_t * __restrict__ _Str,const wchar_t * __restrict__ _Delim);
*/
WINBASEAPI wchar_t *__cdecl wcstok_s(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context);
WINBASEAPI wchar_t *__cdecl wcsstr(const wchar_t *_Str,const wchar_t *_SubStr);
WINBASEAPI wchar_t *__cdecl wcscat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source);
WINBASEAPI wchar_t *__cdecl wcsncat(wchar_t * __restrict__ _Dest, const wchar_t * __restrict__ _Source, size_t _Count);
/*
WINBASEAPI wchar_t *__cdecl strncat(char * __restrict__ _Dest,const char * __restrict__ _Source, size_t _Count);
*/
WINBASEAPI wchar_t *__cdecl wcscpy(wchar_t * __restrict__ _Dest, const wchar_t * __restrict__ _Source);
WINBASEAPI int __cdecl _wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2);
WINBASEAPI int __cdecl _wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2, size_t _Count);
WINBASEAPI int __cdecl _strnicmp(const char *_Str1,const char *_Str2, size_t _Count);
WINBASEAPI _CONST_RETURN wchar_t *__cdecl wcschr(const wchar_t *_Str, wchar_t _Ch);

WINBASEAPI wchar_t *__cdecl wcsrchr(const wchar_t *_Str,wchar_t _Ch);
WINBASEAPI wchar_t *__cdecl wcsrchr(const wchar_t *_Str,wchar_t _Ch);
WINBASEAPI unsigned long __cdecl wcstoul(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix);
char * __cdecl strcat(char * __restrict__ _Dest,const char * __restrict__ _Source);
WINBASEAPI size_t __cdecl strnlen(const char *_Str,size_t _MaxCount);
WINBASEAPI size_t __cdecl strlen(const char *_Str);
int __cdecl strcmp(const char *_Str1,const char *_Str2);
int __cdecl _stricmp(const char *string1,const char *string2);
WINBASEAPI int __cdecl strncmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
char * __cdecl strcpy(char * __restrict__ __dst, const char * __restrict__ __src);
PCHAR __cdecl strstr(const char *haystack, const char *needle);
PCHAR __cdecl strchr(const char *haystack, int needle);
char *__cdecl strtok(char * __restrict__ _Str,const char * __restrict__ _Delim);
_CRTIMP char *__cdecl strtok_s(char *_Str,const char *_Delim,char **_Context);
WINBASEAPI unsigned long __cdecl strtoul(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix);
WINBASEAPI size_t __cdecl strftime(char *_DstBuf,size_t _SizeInBytes,const char *_Format,const struct tm *_Tm);
WINBASEAPI struct tm * __cdecl gmtime(const time_t *_Time);
WINBASEAPI wchar_t * __cdecl wcsncat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count);

//DNSAPI
DNS_STATUS WINAPI DNSAPI$DnsQuery_A(PCSTR,WORD,DWORD,PIP4_ARRAY,PDNS_RECORD*,PVOID*);
VOID WINAPI DNSAPI$DnsFree(PVOID pData,DNS_FREE_TYPE FreeType);

//WSOCK32

//NETAPI32
DWORD WINAPI DsGetDcNameA(LPVOID, LPVOID, LPVOID, LPVOID, ULONG, LPVOID);
WINBASEAPI DWORD WINAPI NetUserGetInfo(LPCWSTR servername,LPCWSTR username,DWORD level,LPBYTE *bufptr);
WINBASEAPI DWORD WINAPI NetUserModalsGet(LPCWSTR servername,DWORD level,LPBYTE *bufptr);
WINBASEAPI DWORD WINAPI NetServerEnum(LMCSTR servername,DWORD level,LPBYTE *bufptr,DWORD prefmaxlen,LPDWORD entriesread,LPDWORD totalentries,DWORD servertype,LMCSTR domain,LPDWORD resume_handle);
WINBASEAPI DWORD WINAPI NetUserGetGroups(LPCWSTR servername,LPCWSTR username,DWORD level,LPBYTE *bufptr,DWORD prefmaxlen,LPDWORD entriesread,LPDWORD totalentries);
WINBASEAPI DWORD WINAPI NetUserGetLocalGroups(LPCWSTR servername,LPCWSTR username,DWORD level,DWORD flags,LPBYTE *bufptr,DWORD prefmaxlen,LPDWORD entriesread,LPDWORD totalentries);
WINBASEAPI DWORD WINAPI NetApiBufferFree(LPVOID Buffer);
WINBASEAPI DWORD WINAPI NetGetAnyDCName(LPCWSTR servername,LPCWSTR domainname,LPBYTE *bufptr);
WINBASEAPI DWORD WINAPI NetUserEnum(LPCWSTR servername,DWORD level,DWORD filter,LPBYTE *bufptr,DWORD prefmaxlen,LPDWORD entriesread,LPDWORD totalentries,LPDWORD resume_handle);
WINBASEAPI DWORD WINAPI NetGroupGetUsers(LPCWSTR servername,LPCWSTR groupname,DWORD level,LPBYTE *bufptr,DWORD prefmaxlen,LPDWORD entriesread,LPDWORD totalentries,PDWORD_PTR ResumeHandle);
WINBASEAPI DWORD WINAPI NetQueryDisplayInformation(LPCWSTR ServerName,DWORD Level,DWORD Index,DWORD EntriesRequested,DWORD PreferredMaximumLength,LPDWORD ReturnedEntryCount,PVOID *SortedBuffer);
WINBASEAPI DWORD WINAPI NetLocalGroupEnum(LPCWSTR servername,DWORD level,LPBYTE *bufptr,DWORD prefmaxlen,LPDWORD entriesread,LPDWORD totalentries,PDWORD_PTR resumehandle);
WINBASEAPI DWORD WINAPI NetLocalGroupGetMembers(LPCWSTR servername,LPCWSTR localgroupname,DWORD level,LPBYTE *bufptr,DWORD prefmaxlen,LPDWORD entriesread,LPDWORD totalentries,PDWORD_PTR resumehandle);
WINBASEAPI DWORD WINAPI NetUserSetInfo(LPCWSTR servername,LPCWSTR username,DWORD level,LPBYTE buf,LPDWORD parm_err);
WINBASEAPI DWORD WINAPI NetShareEnum(LMSTR servername,DWORD level,LPBYTE *bufptr,DWORD prefmaxlen,LPDWORD entriesread,LPDWORD totalentries,LPDWORD resume_handle);
WINBASEAPI DWORD WINAPI NetApiBufferFree(LPVOID Buffer);
WINBASEAPI DWORD WINAPI NetSessionEnum(LPCWSTR servername, LPCWSTR UncClientName, LPCWSTR username, DWORD level, LPBYTE* bufptr, DWORD prefmaxlen, LPDWORD entriesread, LPDWORD totalentries, LPDWORD resumehandle);
WINBASEAPI DWORD WINAPI NetWkstaUserEnum(LMSTR servername,DWORD level,LPBYTE *bufptr,DWORD prefmaxlen,LPDWORD entriesread,LPDWORD totalentries,LPDWORD resumehandle);
WINBASEAPI DWORD WINAPI NetWkstaGetInfo(LMSTR servername,DWORD level,LPBYTE *bufptr);
WINBASEAPI DWORD WINAPI NetStatisticsGet(LMSTR server,LMSTR service,DWORD level,DWORD options,LPBYTE *bufptr);
WINBASEAPI DWORD WINAPI NetRemoteTOD(LPCWSTR UncServerName,LPBYTE  *BufferPtr);

//mpr
WINBASEAPI DWORD WINAPI MPR$WNetOpenEnumW(DWORD dwScope, DWORD dwType, DWORD dwUsage, LPNETRESOURCEW lpNetResource, LPHANDLE lphEnum);
WINBASEAPI DWORD WINAPI MPR$WNetEnumResourceW(HANDLE hEnum, LPDWORD lpcCount, LPVOID lpBuffer, LPDWORD lpBufferSize);
WINBASEAPI DWORD WINAPI MPR$WNetCloseEnum(HANDLE hEnum);
WINBASEAPI DWORD WINAPI MPR$WNetGetNetworkInformationW(LPCWSTR lpProvider, LPNETINFOSTRUCT lpNetInfoStruct);
WINBASEAPI DWORD WINAPI MPR$WNetGetConnectionW(LPCWSTR lpLocalName, LPWSTR lpRemoteName, LPDWORD lpnLength);
WINBASEAPI DWORD WINAPI MPR$WNetGetResourceInformationW(LPNETRESOURCEW lpNetResource, LPVOID lpBuffer, LPDWORD lpcbBuffer, LPWSTR *lplpSystem);
WINBASEAPI DWORD WINAPI MPR$WNetGetUserW(LPCWSTR lpName, LPWSTR lpUserName,	LPDWORD lpnLength);
WINBASEAPI DWORD WINAPI MPR$WNetAddConnection2W(LPNETRESOURCEW lpNetResource, LPCWSTR lpPassword, LPCWSTR lpUserName, DWORD dwFlags);
WINBASEAPI DWORD WINAPI MPR$WNetCancelConnection2W(LPCWSTR lpName, DWORD dwFlags, BOOL fForce);

//user32
WINUSERAPI int WINAPI EnumDesktopWindows(HDESK hDesktop,WNDENUMPROC lpfn,LPARAM lParam);
WINUSERAPI int WINAPI IsWindowVisible (HWND hWnd);
WINUSERAPI int WINAPI GetWindowTextA(HWND hWnd,LPSTR lpString,int nMaxCount);
WINUSERAPI int WINAPI GetClassNameA(HWND hWnd,LPSTR lpClassName,int nMaxCount);
WINUSERAPI LPWSTR WINAPI CharPrevW(LPCWSTR lpszStart,LPCWSTR lpszCurrent);
WINUSERAPI HWND WINAPI FindWindowExA (HWND hWndParent, HWND hWndChildAfter, LPCSTR lpszClass, LPCSTR lpszWindow);
WINUSERAPI LRESULT WINAPI SendMessageA (HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WINUSERAPI int WINAPI GetWindowTextA(HWND  hWnd, LPSTR lpString, int nMaxCount);
WINUSERAPI int WINAPI GetClassNameA(HWND hWnd, LPTSTR lpClassName, int nMaxCount);
WINUSERAPI BOOL WINAPI EnumChildWindows(HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam);

//secur32
WINBASEAPI BOOLEAN WINAPI SECUR32$GetUserNameExA (int NameFormat, LPSTR lpNameBuffer, PULONG nSize);

//shlwapi
WINBASEAPI LPSTR WINAPI SHLWAPI$StrStrIA(LPCSTR lpFirst,LPCSTR lpSrch);
WINBASEAPI int WINAPI SHLWAPI$SHFormatDateTimeA(const FILETIME *pft, DWORD *pdwFlags, LPSTR *pszBuf, UINT cchBuf);

//advapi32
WINADVAPI WINBOOL WINAPI OpenProcessToken (HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);
WINADVAPI WINBOOL WINAPI GetTokenInformation (HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength, PDWORD ReturnLength);
WINADVAPI WINBOOL WINAPI ConvertSidToStringSidA(PSID Sid,LPSTR *StringSid);
WINADVAPI WINBOOL WINAPI ConvertStringSecurityDescriptorToSecurityDescriptorW(LPCWSTR StringSecurityDescriptor,DWORD StringSDRevision,PSECURITY_DESCRIPTOR *SecurityDescriptor,PULONG SecurityDescriptorSize);
WINADVAPI WINBOOL WINAPI LookupAccountSidA (LPCSTR lpSystemName, PSID Sid, LPSTR Name, LPDWORD cchName, LPSTR ReferencedDomainName, LPDWORD cchReferencedDomainName, PSID_NAME_USE peUse);
WINADVAPI WINBOOL WINAPI LookupAccountSidW (LPCWSTR lpSystemName, PSID Sid, LPWSTR Name, LPDWORD cchName, LPWSTR ReferencedDomainName, LPDWORD cchReferencedDomainName, PSID_NAME_USE peUse);
WINADVAPI WINBOOL WINAPI LookupPrivilegeNameA (LPCSTR lpSystemName, PLUID lpLuid, LPSTR lpName, LPDWORD cchName);
WINADVAPI WINBOOL WINAPI LookupPrivilegeDisplayNameA (LPCSTR lpSystemName, LPCSTR lpName, LPSTR lpDisplayName, LPDWORD cchDisplayName, LPDWORD lpLanguageId);
WINADVAPI SC_HANDLE WINAPI OpenSCManagerA(LPCSTR lpMachineName,LPCSTR lpDatabaseName,DWORD dwDesiredAccess);
WINADVAPI SC_HANDLE WINAPI OpenServiceA(SC_HANDLE hSCManager,LPCSTR lpServiceName,DWORD dwDesiredAccess);
WINADVAPI WINBOOL WINAPI QueryServiceStatus(SC_HANDLE hService,LPSERVICE_STATUS lpServiceStatus);
WINADVAPI WINBOOL WINAPI QueryServiceConfigA(SC_HANDLE hService,LPQUERY_SERVICE_CONFIGA lpServiceConfig,DWORD cbBufSize,LPDWORD pcbBytesNeeded);
WINADVAPI WINBOOL WINAPI CloseServiceHandle(SC_HANDLE hSCObject);
WINADVAPI WINBOOL WINAPI EnumServicesStatusExA(SC_HANDLE hSCManager,SC_ENUM_TYPE InfoLevel,DWORD dwServiceType,DWORD dwServiceState,LPBYTE lpServices,DWORD cbBufSize,LPDWORD pcbBytesNeeded,LPDWORD lpServicesReturned,LPDWORD lpResumeHandle,LPCSTR pszGroupName);
WINADVAPI WINBOOL WINAPI QueryServiceStatusEx(SC_HANDLE hService,SC_STATUS_TYPE InfoLevel,LPBYTE lpBuffer,DWORD cbBufSize,LPDWORD pcbBytesNeeded);
WINADVAPI WINBOOL WINAPI QueryServiceConfig2A(SC_HANDLE hService,DWORD dwInfoLevel,LPBYTE lpBuffer,DWORD cbBufSize,LPDWORD pcbBytesNeeded);
WINADVAPI WINBOOL WINAPI ChangeServiceConfig2A(SC_HANDLE hService,DWORD dwInfoLevel,LPVOID lpInfo);
WINADVAPI WINBOOL WINAPI ChangeServiceConfigA(SC_HANDLE hService,DWORD dwServiceType,DWORD dwStartType,DWORD dwErrorControl,LPCSTR lpBinaryPathName,LPCSTR lpLoadOrderGroup,LPDWORD lpdwTagId,LPCSTR lpDependencies,LPCSTR lpServiceStartName,LPCSTR lpPassword,LPCSTR lpDisplayName);
WINADVAPI SC_HANDLE WINAPI CreateServiceA(SC_HANDLE hSCManager,LPCSTR lpServiceName,LPCSTR lpDisplayName,DWORD dwDesiredAccess,DWORD dwServiceType,DWORD dwStartType,DWORD dwErrorControl,LPCSTR lpBinaryPathName,LPCSTR lpLoadOrderGroup,LPDWORD lpdwTagId,LPCSTR lpDependencies,LPCSTR lpServiceStartName,LPCSTR lpPassword);
WINADVAPI WINBOOL WINAPI DeleteService(SC_HANDLE hService);
WINADVAPI LONG WINAPI RegOpenKeyExW(HKEY hKey,LPCWSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult);
WINADVAPI WINBOOL WINAPI EnumServicesStatusExW(SC_HANDLE hSCManager,SC_ENUM_TYPE InfoLevel,DWORD dwServiceType,DWORD dwServiceState,LPBYTE lpServices,DWORD cbBufSize,LPDWORD pcbBytesNeeded,LPDWORD lpServicesReturned,LPDWORD lpResumeHandle,LPCWSTR pszGroupName);
WINADVAPI LONG WINAPI RegCreateKeyA(HKEY hKey,LPCSTR lpSubKey,PHKEY phkResult);
WINADVAPI LONG WINAPI RegSetValueExA(HKEY hKey,LPCSTR lpValueName,DWORD Reserved,DWORD dwType,CONST BYTE *lpData,DWORD cbData);
WINADVAPI LONG WINAPI RegOpenKeyExA(HKEY hKey,LPCSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult);
WINADVAPI LONG WINAPI RegConnectRegistryA(LPCSTR lpMachineName,HKEY hKey,PHKEY phkResult);
WINADVAPI LONG WINAPI RegCloseKey(HKEY hKey);
WINADVAPI LONG WINAPI RegOpenKeyA(HKEY hKey,LPCSTR lpSubKey,PHKEY phkResult);
WINADVAPI LONG WINAPI RegCreateKeyExA(HKEY hKey,LPCSTR lpSubKey,DWORD Reserved,LPSTR lpClass,DWORD dwOptions,REGSAM samDesired,LPSECURITY_ATTRIBUTES lpSecurityAttributes,PHKEY phkResult,LPDWORD lpdwDisposition);
WINADVAPI LONG WINAPI RegDeleteKeyExA(HKEY hKey,LPCSTR lpSubKey,REGSAM samDesired,DWORD Reserved);
WINADVAPI LONG WINAPI RegDeleteKeyValueA(HKEY hKey,LPCSTR lpSubKey,LPCSTR lpValueName);
WINADVAPI LONG WINAPI RegQueryValueExA(HKEY hKey,LPCSTR lpValueName,LPDWORD lpReserved,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData);
WINADVAPI LONG WINAPI RegQueryInfoKeyA(HKEY hKey,LPSTR lpClass,LPDWORD lpcchClass,LPDWORD lpReserved,LPDWORD lpcSubKeys,LPDWORD lpcbMaxSubKeyLen,LPDWORD lpcbMaxClassLen,LPDWORD lpcValues,LPDWORD lpcbMaxValueNameLen,LPDWORD lpcbMaxValueLen,LPDWORD lpcbSecurityDescriptor,PFILETIME lpftLastWriteTime);
WINADVAPI LONG WINAPI RegEnumValueA(HKEY hKey,DWORD dwIndex,LPSTR lpValueName,LPDWORD lpcchValueName,LPDWORD lpReserved,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData);
WINADVAPI LONG WINAPI RegEnumKeyExA(HKEY hKey,DWORD dwIndex,LPSTR lpName,LPDWORD lpcchName,LPDWORD lpReserved,LPSTR lpClass,LPDWORD lpcchClass,PFILETIME lpftLastWriteTime);
WINADVAPI LONG WINAPI RegDeleteValueA(HKEY hKey,LPCSTR lpValueName);
WINADVAPI LONG WINAPI RegQueryValueExW(HKEY hKey,LPCWSTR lpValueName,LPDWORD lpReserved,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData);
WINADVAPI LONG WINAPI RegSaveKeyExA(HKEY hKey,LPCSTR lpFile,LPSECURITY_ATTRIBUTES lpSecurityAttributes,DWORD Flags);
WINADVAPI WINBOOL WINAPI GetFileSecurityW (LPCWSTR lpFileName, SECURITY_INFORMATION RequestedInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD nLength, LPDWORD lpnLengthNeeded);
WINADVAPI WINBOOL WINAPI GetSecurityDescriptorOwner (PSECURITY_DESCRIPTOR pSecurityDescriptor, PSID *pOwner, LPBOOL lpbOwnerDefaulted);
WINADVAPI WINBOOL WINAPI GetSecurityDescriptorDacl (PSECURITY_DESCRIPTOR pSecurityDescriptor, LPBOOL lpbDaclPresent, PACL *pDacl, LPBOOL lpbDaclDefaulted);
WINADVAPI WINBOOL WINAPI GetAclInformation (PACL pAcl, LPVOID pAclInformation, DWORD nAclInformationLength, ACL_INFORMATION_CLASS dwAclInformationClass);
WINADVAPI WINBOOL WINAPI GetAce (PACL pAcl, DWORD dwAceIndex, LPVOID *pAce);
WINADVAPI WINBOOL WINAPI LookupAccountSidW (LPCWSTR lpSystemName, PSID Sid, LPWSTR Name, LPDWORD cchName, LPWSTR ReferencedDomainName, LPDWORD cchReferencedDomainName, PSID_NAME_USE peUse);
WINADVAPI WINBOOL WINAPI ConvertSidToStringSidW(PSID Sid,LPWSTR *StringSid);
WINADVAPI VOID WINAPI MapGenericMask (PDWORD AccessMask, PGENERIC_MAPPING GenericMapping);
WINADVAPI WINBOOL WINAPI OpenProcessToken (HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);
WINADVAPI WINBOOL WINAPI GetTokenInformation (HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength, PDWORD ReturnLength);
WINADVAPI WINBOOL WINAPI InitializeSecurityDescriptor (PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD dwRevision);
WINADVAPI WINBOOL WINAPI SetSecurityDescriptorDacl (PSECURITY_DESCRIPTOR pSecurityDescriptor, WINBOOL bDaclPresent, PACL pDacl, WINBOOL bDaclDefaulted);
WINADVAPI WINBOOL WINAPI ConvertSecurityDescriptorToStringSecurityDescriptorW(PSECURITY_DESCRIPTOR SecurityDescriptor,DWORD RequestedStringSDRevision,SECURITY_INFORMATION SecurityInformation,LPWSTR *StringSecurityDescriptor,PULONG StringSecurityDescriptorLen);
WINADVAPI WINBOOL WINAPI StartServiceA(SC_HANDLE hService,DWORD dwNumServiceArgs,LPCSTR *lpServiceArgVectors);
WINADVAPI WINBOOL WINAPI ControlService(SC_HANDLE hService,DWORD dwControl,LPSERVICE_STATUS lpServiceStatus);
WINADVAPI WINBOOL WINAPI EnumDependentServicesA(SC_HANDLE hService,DWORD dwServiceState,LPENUM_SERVICE_STATUSA lpServices,DWORD cbBufSize,LPDWORD pcbBytesNeeded,LPDWORD lpServicesReturned);
WINADVAPI LSTATUS WINAPI RegQueryInfoKeyA(HKEY hKey, LPSTR lpClass, LPDWORD lpcchClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime);

//NTDLL
WINBASEAPI NTSTATUS NTAPI NTDLL$NtCreateFile(PHANDLE FileHandle,ACCESS_MASK DesiredAccess,POBJECT_ATTRIBUTES ObjectAttributes,PIO_STATUS_BLOCK IoStatusBlock,PLARGE_INTEGER AllocationSize,ULONG FileAttributes,ULONG ShareAccess,ULONG CreateDisposition,ULONG CreateOptions,PVOID EaBuffer,ULONG EaLength);
WINBASEAPI NTSTATUS NTAPI NTDLL$NtClose(HANDLE Handle);
WINBASEAPI NTSTATUS NTAPI NTDLL$NtFsControlFile(HANDLE FileHandle,HANDLE Event,PIO_APC_ROUTINE ApcRoutine,PVOID ApcContext,PIO_STATUS_BLOCK IoStatusBlock,ULONG IoControlCode,PVOID InputBuffer,ULONG InputBufferLength,PVOID OutputBuffer,ULONG OutputBufferLength);

//IMAGEHLP
WINBASEAPI WINBOOL IMAGEAPI IMAGEHLP$ImageEnumerateCertificates(HANDLE FileHandle,WORD TypeFilter,PDWORD CertificateCount,PDWORD Indices,DWORD IndexCount);
WINBASEAPI WINBOOL IMAGEAPI IMAGEHLP$ImageGetCertificateHeader(HANDLE FileHandle,DWORD CertificateIndex,LPWIN_CERTIFICATE Certificateheader);
WINBASEAPI WINBOOL IMAGEAPI IMAGEHLP$ImageGetCertificateData(HANDLE FileHandle,DWORD CertificateIndex,LPWIN_CERTIFICATE Certificate,PDWORD RequiredLength);

//crypt32
WINIMPM WINBOOL WINAPI CRYPT32$CryptVerifyMessageSignature (PCRYPT_VERIFY_MESSAGE_PARA pVerifyPara, DWORD dwSignerIndex, const BYTE *pbSignedBlob, DWORD cbSignedBlob, BYTE *pbDecoded, DWORD *pcbDecoded, PCCERT_CONTEXT *ppSignerCert);
WINIMPM DWORD WINAPI CRYPT32$CertGetNameStringW (PCCERT_CONTEXT pCertContext, DWORD dwType, DWORD dwFlags, void *pvTypePara, LPWSTR pszNameString, DWORD cchNameString);
WINIMPM PCCERT_CONTEXT WINAPI CRYPT32$CertCreateCertificateContext (DWORD dwCertEncodingType, const BYTE *pbCertEncoded, DWORD cbCertEncoded);
WINIMPM WINBOOL WINAPI CRYPT32$CertFreeCertificateContext (PCCERT_CONTEXT pCertContext);
WINIMPM WINBOOL WINAPI CRYPT32$CertGetCertificateContextProperty (PCCERT_CONTEXT pCertContext, DWORD dwPropId, void *pvData, DWORD *pcbData);
WINIMPM WINBOOL WINAPI CRYPT32$CertGetCertificateChain (HCERTCHAINENGINE hChainEngine, PCCERT_CONTEXT pCertContext, LPFILETIME pTime, HCERTSTORE hAdditionalStore, PCERT_CHAIN_PARA pChainPara, DWORD dwFlags, LPVOID pvReserved, PCCERT_CHAIN_CONTEXT *ppChainContext);
WINIMPM VOID WINAPI CRYPT32$CertFreeCertificateChain (PCCERT_CHAIN_CONTEXT pChainContext);
WINIMPM PCCRYPT_OID_INFO WINAPI CRYPT32$CryptFindOIDInfo (DWORD dwKeyType, void *pvKey, DWORD dwGroupId);

//WS2_32
// defining this here to avoid including ws2tcpip.h which results in include order warnings when bofs include windows.h before bofdefs.h
typedef struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    size_t ai_addrlen;
    char *ai_canonname;
    struct sockaddr *ai_addr;
    struct addrinfo *ai_next;
} ADDRINFOA,*PADDRINFOA;

//WS2_32
int __stdcall connect(SOCKET sock, const struct sockaddr* name, int namelen);
int __stdcall closesocket(SOCKET sock);
void __stdcall freeaddrinfo(struct addrinfo* ai);
int __stdcall getaddrinfo(char* host, char* port, const struct addrinfo* hints, struct addrinfo** result);
u_long __stdcall htonl(u_long hostlong);
u_short __stdcall htons(u_short hostshort);
char * __stdcall inet_ntoa(struct in_addr in);
int __stdcall ioctlsocket(SOCKET sock, long cmd, u_long* arg);
int __stdcall select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timeval* timeout);
/*
unsigned int __stdcall socket(int af, int type, int protocol);
*/
int __stdcall __WSAFDIsSet(SOCKET sock, struct fd_set* fdset);
int __stdcall WSAGetLastError();
LPCWSTR WINAPI InetNtopW(INT Family, LPCVOID pAddr, LPWSTR pStringBuf, size_t StringBufSIze);
INT WINAPI inet_pton(INT Family, LPCSTR pStringBuf, PVOID pAddr);

//dnsapi
VOID WINAPI DNSAPI$DnsFree(PVOID pData,DNS_FREE_TYPE FreeType);
int WINAPI DNSAPI$DnsGetCacheDataTable(PVOID data);

//OLE32
HRESULT WINAPI CoInitializeEx (LPVOID pvReserved, DWORD dwCoInit);
/*
HRESULT WINAPI CoUninitialize (void);
*/
HRESULT WINAPI CoInitializeSecurity (PSECURITY_DESCRIPTOR pSecDesc, LONG cAuthSvc, SOLE_AUTHENTICATION_SERVICE *asAuthSvc, void *pReserved1, DWORD dwAuthnLevel, DWORD dwImpLevel, void *pAuthList, DWORD dwCapabilities, void *pReserved3);
HRESULT WINAPI CoCreateInstance (REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID *ppv);
HRESULT WINAPI CLSIDFromString (LPCOLESTR lpsz, LPCLSID pclsid);
HRESULT WINAPI IIDFromString (LPCOLESTR lpsz, LPIID lpiid);
int     WINAPI StringFromGUID2 (REFGUID rguid, LPOLESTR lpsz, int cchMax);
DECLSPEC_IMPORT	HRESULT WINAPI CoSetProxyBlanket(IUnknown* pProxy, DWORD dwAuthnSvc, DWORD dwAuthzSvc, OLECHAR* pServerPrincName, DWORD dwAuthnLevel, DWORD dwImpLevel, RPC_AUTH_IDENTITY_HANDLE pAuthInfo, DWORD dwCapabilities);
LPVOID	WINAPI CoTaskMemAlloc(SIZE_T cb);
void	WINAPI CoTaskMemFree(LPVOID pv);

//OLEAUT32
BSTR	WINAPI SysAllocString(const OLECHAR *);
INT		WINAPI SysReAllocString(BSTR *, const OLECHAR *);
void	WINAPI SysFreeString(BSTR);
UINT	WINAPI SysStringLen(BSTR);
void	WINAPI VariantInit(VARIANTARG *pvarg);
/*
void	WINAPI VariantClear(VARIANTARG *pvarg);
*/
HRESULT	WINAPI SysAddRefString(BSTR);
HRESULT	WINAPI VariantChangeType(VARIANTARG *pvargDest, VARIANTARG *pvarSrc, USHORT wFlags, VARTYPE vt);
/*
void	WINAPI VarFormatDateTime(LPVARIANT pvarIn,int iNamedFormat,ULONG dwFlags,BSTR *pbstrOut);
void	WINAPI SafeArrayDestroy(SAFEARRAY *psa);
*/
HRESULT	WINAPI SafeArrayLock(SAFEARRAY *psa);
HRESULT	WINAPI SafeArrayGetLBound(SAFEARRAY *psa, UINT nDim, LONG *plLbound);
HRESULT	WINAPI SafeArrayGetUBound(SAFEARRAY *psa, UINT nDim, LONG *plUbound);
HRESULT	WINAPI SafeArrayGetElement(SAFEARRAY *psa, LONG *rgIndices, void *pv);
UINT	WINAPI SafeArrayGetElemsize(SAFEARRAY *psa);
HRESULT	WINAPI SafeArrayAccessData(SAFEARRAY *psa,void HUGEP **ppvData);
HRESULT	WINAPI SafeArrayUnaccessData(SAFEARRAY *psa);








//CERTCLI
/*
HRESULT WINAPI CERTCLI$CAEnumFirstCA(IN LPCWSTR wszScope, IN DWORD dwFlags, OUT LPVOID * phCAInfo);
HRESULT WINAPI CERTCLI$CAEnumNextCA(IN LPVOID hPrevCA, OUT LPVOID * phCAInfo);
HRESULT WINAPI CERTCLI$CACloseCA(IN LPVOID hCA);
DWORD WINAPI CERTCLI$CACountCAs(IN LPVOID hCAInfo);
LPCWSTR WINAPI CERTCLI$CAGetDN(IN LPVOID hCAInfo);
HRESULT WINAPI CERTCLI$CAGetCAProperty(IN LPVOID hCAInfo, IN LPCWSTR wszPropertyName, OUT PZPWSTR *pawszPropertyValue);
HRESULT WINAPI CERTCLI$CAFreeCAProperty(IN LPVOID hCAInfo, IN PZPWSTR awszPropertyValue);
HRESULT WINAPI CERTCLI$CAGetCAFlags(IN LPVOID hCAInfo, OUT DWORD  *pdwFlags);
HRESULT WINAPI CERTCLI$CAGetCACertificate(IN LPVOID hCAInfo, OUT PCCERT_CONTEXT *ppCert);
HRESULT WINAPI CERTCLI$CAGetCAExpiration(IN LPVOID hCAInfo, OUT DWORD * pdwExpiration, OUT DWORD * pdwUnits);
HRESULT WINAPI CERTCLI$CAGetCASecurity(IN LPVOID hCAInfo, OUT PSECURITY_DESCRIPTOR * ppSD);
HRESULT WINAPI CERTCLI$CAGetAccessRights(IN LPVOID hCAInfo, IN DWORD dwContext, OUT DWORD *pdwAccessRights);
HRESULT WINAPI CERTCLI$CAEnumCertTypesForCA(IN LPVOID hCAInfo, IN DWORD dwFlags, OUT LPVOID * phCertType);
HRESULT WINAPI CERTCLI$CAEnumCertTypes(IN DWORD dwFlags, OUT LPVOID * phCertType);
HRESULT WINAPI CERTCLI$CAEnumNextCertType(IN LPVOID hPrevCertType, OUT LPVOID * phCertType);
DWORD WINAPI CERTCLI$CACountCertTypes(IN LPVOID hCertType);
HRESULT WINAPI CERTCLI$CACloseCertType(IN LPVOID hCertType);
HRESULT WINAPI CERTCLI$CAGetCertTypeProperty(IN LPVOID hCertType, IN LPCWSTR wszPropertyName, OUT PZPWSTR *pawszPropertyValue);
HRESULT WINAPI CERTCLI$CAGetCertTypePropertyEx(IN LPVOID hCertType, IN LPCWSTR wszPropertyName, OUT LPVOID *pPropertyValue);
HRESULT WINAPI CERTCLI$CAFreeCertTypeProperty(IN LPVOID hCertType, IN PZPWSTR awszPropertyValue);
HRESULT WINAPI CERTCLI$CAGetCertTypeExtensionsEx(IN LPVOID hCertType, IN DWORD dwFlags, IN LPVOID pParam, OUT PCERT_EXTENSIONS * ppCertExtensions);
HRESULT WINAPI CERTCLI$CAFreeCertTypeExtensions(IN LPVOID hCertType, IN PCERT_EXTENSIONS pCertExtensions);
HRESULT WINAPI CERTCLI$CAGetCertTypeFlagsEx(IN LPVOID hCertType, IN DWORD dwOption, OUT DWORD * pdwFlags);
HRESULT WINAPI CERTCLI$CAGetCertTypeExpiration(IN LPVOID hCertType, OUT OPTIONAL FILETIME * pftExpiration, OUT OPTIONAL FILETIME * pftOverlap);
HRESULT WINAPI CERTCLI$CACertTypeGetSecurity(IN LPVOID hCertType, OUT PSECURITY_DESCRIPTOR * ppSD);
HRESULT WINAPI CERTCLI$CAGetCertTypeAccessRights(IN LPVOID hCertType, IN DWORD dwContext, OUT DWORD *pdwAccessRights);
HRESULT WINAPI CERTCLI$caTranslateFileTimePeriodToPeriodUnits(IN FILETIME const *pftGMT, IN BOOL Flags, OUT DWORD *pcPeriodUnits, OUT LPVOID*prgPeriodUnits);
*/











//dbghelp
WINBOOL WINAPI DBGHELP$MiniDumpWriteDump(HANDLE hProcess,DWORD ProcessId,HANDLE hFile,MINIDUMP_TYPE DumpType,CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

//WLDAP32
WINLDAPAPI LDAP* LDAPAPI WLDAP32$ldap_init(PSTR, ULONG);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_bind_s(LDAP *ld,const PSTR  dn,const PCHAR cred,ULONG method);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_search_s(LDAP *ld,PSTR base,ULONG scope,PSTR filter,PZPSTR attrs,ULONG attrsonly,PLDAPMessage *res);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_count_entries(LDAP*,LDAPMessage*);
WINLDAPAPI struct berval **LDAPAPI WLDAP32$ldap_get_values_lenA (LDAP *ExternalHandle,LDAPMessage *Message,const PCHAR attr);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_value_free_len(struct berval **vals);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_set_optionA(LDAP *ld,int option,const void *invalue);
WINLDAPAPI PLDAPSearch LDAPAPI WLDAP32$ldap_search_init_pageA(PLDAP ExternalHandle,const PCHAR DistinguishedName,ULONG ScopeOfSearch,const PCHAR SearchFilter,PCHAR AttributeList[],ULONG AttributesOnly,PLDAPControlA *ServerControls,PLDAPControlA *ClientControls,ULONG PageTimeLimit,ULONG TotalSizeLimit,PLDAPSortKeyA *SortKeys);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_get_paged_count(PLDAP ExternalHandle,PLDAPSearch SearchBlock,ULONG *TotalCount,PLDAPMessage Results);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_get_next_page_s(PLDAP ExternalHandle,PLDAPSearch SearchHandle,struct l_timeval *timeout,ULONG PageSize,ULONG *TotalCount,LDAPMessage **Results);

WINLDAPAPI LDAPMessage*  LDAPAPI WLDAP32$ldap_first_entry(LDAP *ld,LDAPMessage *res);
WINLDAPAPI LDAPMessage*  LDAPAPI WLDAP32$ldap_next_entry(LDAP*,LDAPMessage*);
WINLDAPAPI PCHAR LDAPAPI WLDAP32$ldap_first_attribute(LDAP *ld,LDAPMessage *entry,BerElement **ptr);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_count_values(PCHAR);
WINLDAPAPI PCHAR * LDAPAPI WLDAP32$ldap_get_values(LDAP *ld,LDAPMessage *entry,const PSTR attr);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_value_free(PCHAR *);
WINLDAPAPI PCHAR LDAPAPI WLDAP32$ldap_next_attribute(LDAP *ld,LDAPMessage *entry,BerElement *ptr);
WINLDAPAPI VOID LDAPAPI WLDAP32$ber_free(BerElement *pBerElement,INT fbuf);
WINLDAPAPI VOID LDAPAPI WLDAP32$ldap_memfree(PCHAR);

WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_unbind(LDAP*);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_unbind_s(LDAP*);
WINLDAPAPI ULONG LDAPAPI WLDAP32$ldap_msgfree(LDAPMessage*);

//RPCRT4
RPCRTAPI RPC_STATUS RPC_ENTRY RPCRT4$UuidToStringA(UUID *Uuid,RPC_CSTR *StringUuid);
RPCRTAPI RPC_STATUS RPC_ENTRY RPCRT4$RpcStringFreeA(RPC_CSTR *String);

//PSAPI
WINBOOL WINAPI PSAPI$EnumProcessModulesEx(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded, DWORD dwFilterFlag);
DWORD WINAPI PSAPI$GetModuleFileNameExA(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize);

//VERSION
DWORD WINAPI VERSION$GetFileVersionInfoSizeA(LPCSTR lptstrFilenamea ,LPDWORD lpdwHandle);
WINBOOL WINAPI VERSION$GetFileVersionInfoA(LPCSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
WINBOOL WINAPI VERSION$VerQueryValueA(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen);
#endif
#else

#define intAlloc(size) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)
#define intRealloc(ptr, size) (ptr) ? HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ptr, size) : HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)
#define intFree(addr) HeapFree(GetProcessHeap(), 0, addr)
#define intZeroMemory(addr,size) memset((addr),0,size)

#ifndef BOF
#define VirtualAlloc  VirtualAlloc 
#define VirtualFree  VirtualFree 
#define LocalAlloc  LocalAlloc 
#define LocalFree  LocalFree 
#define HeapAlloc  HeapAlloc 
#define HeapReAlloc  HeapReAlloc 
#define GetProcessHeap GetProcessHeap
#define HeapFree  HeapFree 
#define Kernel32$FormatMessageA  FormatMessageA 
#define Kernel32$WideCharToMultiByte  WideCharToMultiByte 
#define FileTimeToLocalFileTime  FileTimeToLocalFileTime 
#define FileTimeToSystemTime  FileTimeToSystemTime 
#define GetDateFormatW  GetDateFormatW 
#define GetSystemTimeAsFileTime  GetSystemTimeAsFileTime 
#define GetLocalTime  GetLocalTime 
#define SystemTimeToFileTime  SystemTimeToFileTime 
#define SystemTimeToTzSpecificLocalTime  SystemTimeToTzSpecificLocalTime 
#define GlobalMemoryStatusEx  GlobalMemoryStatusEx 
#define GetDiskFreeSpaceExA  GetDiskFreeSpaceExA 
#define GetCurrentProcess  GetCurrentProcess 
#define GetCurrentProcessId GetCurrentProcessId
#define GetLastError  GetLastError 
#define CloseHandle  CloseHandle 
#define CreateThread  CreateThread 
#define GetTickCount  GetTickCount 
#define GetTickCount64  GetTickCount64 
#define CreateFiber  CreateFiber 
#define ConvertThreadToFiber  ConvertThreadToFiber 
#define ConvertFiberToThread  ConvertFiberToThread 
#define DeleteFiber  DeleteFiber 
#define SwitchToFiber  SwitchToFiber 
#define WaitForSingleObject  WaitForSingleObject 
#define Sleep  Sleep 
#define DeleteFileW  DeleteFileW 
#define CreateFileW  CreateFileW 
#define GetFileSize  GetFileSize 
#define ReadFile  ReadFile 
#define OpenProcess  OpenProcess 
#define GetComputerNameExW  GetComputerNameExW 
#define lstrlenW  lstrlenW 
#define lstrcatW  lstrcatW 
#define lstrcpynW  lstrcpynW 
#define GetFullPathNameW  GetFullPathNameW 
#define GetFileAttributesW  GetFileAttributesW 
#define GetCurrentDirectoryW  GetCurrentDirectoryW 
#define FindFirstFileW  FindFirstFileW 
#define FindNextFileW  FindNextFileW 
#define FindFirstFileA  FindFirstFileA
#define FindNextFileA  FindNextFileA 
#define FindClose  FindClose 
#define SetLastError  SetLastError 
#define HeapAlloc HeapAlloc
#define HeapReAlloc HeapReAlloc
#define HeapFree HeapFree
#define memset memset
#define GlobalAlloc GlobalAlloc
#define GlobalFree GlobalFree
#define GetEnvironmentStrings GetEnvironmentStrings
#define FreeEnvironmentStringsA FreeEnvironmentStringsA
#define ExpandEnvironmentStringsW  ExpandEnvironmentStringsW 
#define CreateToolhelp32Snapshot CreateToolhelp32Snapshot
#define Process32First Process32First
#define Process32Next Process32Next
#define Module32First Module32First
#define Module32Next Module32Next
#define LoadLibraryA LoadLibraryA
#define GetProcAddress GetProcAddress
#define FreeLibrary FreeLibrary
#define lstrlenA lstrlenA
#define GetLocaleInfoEx GetLocaleInfoEx
#define GetSystemDefaultLocaleName GetSystemDefaultLocaleName
#define LocaleNameToLCID LocaleNameToLCID
#define GetDateFormatEx GetDateFormatEx

#define WTSAPI32$WTSEnumerateSessionsA WTSEnumerateSessionsA
#define WTSAPI32$WTSQuerySessionInformationA WTSQuerySessionInformationA
#define WTSAPI32$WTSFreeMemory WTSFreeMemory
#define IPHLPAPI$GetAdaptersInfo  GetAdaptersInfo 
#define IPHLPAPI$GetAdaptersInfo GetAdaptersInfo
#define IPHLPAPI$GetIpForwardTable  GetIpForwardTable 
#define IPHLPAPI$GetNetworkParams GetNetworkParams
#define IPHLPAPI$GetUdpTable  GetUdpTable 
#define IPHLPAPI$GetTcpTable  GetTcpTable 
#define IPHLPAPI$GetIpNetTable GetIpNetTable
#define calloc calloc
#define memcpy memcpy
#define memcmp memcmp
#define realloc realloc
#define free free
#define memset memset
#define sprintf sprintf
#define vsnprintf vsnprintf
#define _snwprintf _snwprintf
#define wcscpy_s wcscpy_s
#define wcslen wcslen
#define wcstombs wcstombs
#define sprintf  sprintf 
#define wcscmp wcscmp
#define wcstok wcstok
#define wcstok_s wcstok_s
#define wcsstr wcsstr
#define wcscat wcscat
#define wcsncat wcsncat
#define wcscpy wcscpy
#define _wcsicmp _wcsicmp
#define wcschr wcschr
#define wcsncat wcsncat
#define wcsrchr wcsrchr
#define wcsrchr wcsrchr
#define wcstoul wcstoul
#define strcat strcat
#define strnlen strnlen
#define strlen strlen
#define strcmp strcmp
#define strncmp strncmp
#define _stricmp _stricmp
#define strcpy strcpy
#define strstr strstr
#define strchr strchr
#define strtok strtok
#define strtok_s strtok_s
#define strtoul strtoul
#define DNSAPI$DnsQuery_A DnsQuery_A
#define DNSAPI$DnsFree DnsFree
#define WSOCK32$inet_addr inet_addr
#define closesocket closesocket
#define connect connect
#define freeaddrinfo freeaddrinfo
#define getaddrinfo getaddrinfo
#define htonl htonl
#define htons htons
#define inet_ntoa inet_ntoa
#define ioctlsocket ioctlsocket
#define select select
#define socket socket
#define __WSAFDIsSet __WSAFDIsSet
#define WSAGetLastError WSAGetLastError
#define DsGetDcNameA DsGetDcNameA
#define NetUserGetInfo NetUserGetInfo
#define NetUserModalsGet NetUserModalsGet
#define NetServerEnum NetServerEnum
#define NetUserGetGroups NetUserGetGroups
#define NetUserGetLocalGroups NetUserGetLocalGroups
#define NetApiBufferFree NetApiBufferFree
#define NetGetAnyDCName NetGetAnyDCName
#define NetUserEnum NetUserEnum
#define NetGroupGetUsers NetGroupGetUsers
#define NetQueryDisplayInformation NetQueryDisplayInformation
#define NetLocalGroupEnum NetLocalGroupEnum
#define NetLocalGroupGetMembers NetLocalGroupGetMembers
#define NetUserSetInfo NetUserSetInfo
#define NetShareEnum NetShareEnum
#define NetWkstaUserEnum NetWkstaUserEnum
#define NetWkstaGetInfo NetWkstaGetInfo
#define NetStatisticsGet NetStatisticsGet
#define NetApiBufferFree NetApiBufferFree
#define NetSessionEnum NetSessionEnum
#define MPR$WNetOpenEnumW WNetOpenEnumW
#define MPR$WNetEnumResourceW WNetEnumResourceW
#define MPR$WNetCloseEnum WNetCloseEnum
#define MPR$WNetGetNetworkInformationW WNetGetNetworkInformationW
#define MPR$WNetGetConnectionW WNetGetConnectionW
#define MPR$WNetGetResourceInformationW WNetGetResourceInformationW
#define MPR$WNetGetUserW WNetGetUserW
#define MPR$WNetAddConnection2W WNetAddConnection2W
#define MPR$WNetCancelConnection2W WNetCancelConnection2W
#define EnumDesktopWindows EnumDesktopWindows
#define IsWindowVisible  IsWindowVisible 
#define GetWindowTextA GetWindowTextA
#define GetClassNameA GetClassNameA
#define CharPrevW CharPrevW
#define FindWindowExA FindWindowExA 
#define SendMessageA SendMessageA
#define GetWindowTextA GetWindowTextA
#define GetClassNameA GetClassNameA
#define EnumChildWindows EnumChildWindows
#define SECUR32$GetUserNameExA  GetUserNameExA 
#define SHLWAPI$StrStrIA StrStrIA
#define SHLWAPI$SHFormatDateTimeA SHFormatDateTimeA
#define OpenProcessToken  OpenProcessToken 
#define GetTokenInformation  GetTokenInformation 
#define ConvertSidToStringSidA ConvertSidToStringSidA
#define ConvertStringSecurityDescriptorToSecurityDescriptorW ConvertStringSecurityDescriptorToSecurityDescriptorW
#define LookupAccountSidA  LookupAccountSidA 
#define LookupAccountSidW  LookupAccountSidW
#define LookupPrivilegeNameA  LookupPrivilegeNameA 
#define LookupPrivilegeDisplayNameA  LookupPrivilegeDisplayNameA 
#define OpenSCManagerA OpenSCManagerA
#define OpenServiceA OpenServiceA
#define QueryServiceStatus QueryServiceStatus
#define QueryServiceConfigA QueryServiceConfigA
#define CloseServiceHandle CloseServiceHandle
#define EnumServicesStatusExA EnumServicesStatusExA
#define QueryServiceStatusEx QueryServiceStatusEx
#define QueryServiceConfig2A QueryServiceConfig2A
#define ChangeServiceConfig2A ChangeServiceConfig2A
#define ChangeServiceConfigA ChangeServiceConfigA
#define CreateServiceA CreateServiceA
#define DeleteService DeleteService
#define RegOpenKeyExW RegOpenKeyExW
#define EnumServicesStatusExW EnumServicesStatusExW
#define RegCreateKeyA RegCreateKeyA
#define RegSetValueExA RegSetValueExA
#define RegOpenKeyExA RegOpenKeyExA
#define RegConnectRegistryA RegConnectRegistryA
#define RegCloseKey RegCloseKey
#define RegOpenKeyA RegOpenKeyA
#define RegCreateKeyExA RegCreateKeyExA
#define RegDeleteKeyExA RegDeleteKeyExA
#define RegDeleteKeyValueA RegDeleteKeyValueA
#define RegQueryValueExA RegQueryValueExA
#define RegQueryInfoKeyA RegQueryInfoKeyA
#define RegEnumValueA RegEnumValueA
#define RegEnumKeyExA RegEnumKeyExA
#define RegDeleteValueA RegDeleteValueA
#define RegQueryValueExW RegQueryValueExW
#define RegSaveKeyExA RegSaveKeyExA
#define GetFileSecurityW GetFileSecurityW 
#define GetSecurityDescriptorOwner GetSecurityDescriptorOwner
#define GetSecurityDescriptorDacl GetSecurityDescriptorDacl 
#define GetAclInformation GetAclInformation
#define GetAce GetAce 
#define LookupAccountSidW LookupAccountSidW 
#define ConvertSidToStringSidW ConvertSidToStringSidW
#define MapGenericMask  MapGenericMask 
#define OpenProcessToken  OpenProcessToken 
#define GetTokenInformation  GetTokenInformation 
#define InitializeSecurityDescriptor  InitializeSecurityDescriptor 
#define SetSecurityDescriptorDacl  SetSecurityDescriptorDacl 
#define ConvertSecurityDescriptorToStringSecurityDescriptorW ConvertSecurityDescriptorToStringSecurityDescriptorW
#define StartServiceA StartServiceA
#define ControlService ControlService
#define EnumDependentServicesA EnumDependentServicesA
#define RegQueryInfoKeyA RegQueryInfoKeyA
#define NTDLL$NtCreateFile NtCreateFile
#define NTDLL$NtClose NtClose
#define IMAGEHLP$ImageEnumerateCertificates ImageEnumerateCertificates
#define IMAGEHLP$ImageGetCertificateHeader ImageGetCertificateHeader
#define IMAGEHLP$ImageGetCertificateData ImageGetCertificateData
#define CRYPT32$CryptVerifyMessageSignature  CryptVerifyMessageSignature 
#define CRYPT32$CertGetNameStringW  CertGetNameStringW 
#define CRYPT32$CertGetCertificateContextProperty CertGetCertificateContextProperty
#define CRYPT32$CertCreateCertificateContext  CertCreateCertificateContext
#define CRYPT32$CertFreeCertificateContext  CertFreeCertificateContext 
#define CRYPT32$CertGetCertificateChain CertGetCertificateChain
#define CRYPT32$CertFreeCertificateChain CertFreeCertificateChain
#define CRYPT32$CryptFindOIDInfo CryptFindOIDInfo
#define InetNtopW InetNtopW
#define inet_pton inet_pton
#define DNSAPI$DnsFree DnsFree
#define DNSAPI$DnsGetCacheDataTable DnsGetCacheDataTable
#define CoInitializeEx  CoInitializeEx 
#define CoUninitialize  CoUninitialize 
#define CoInitializeSecurity  CoInitializeSecurity 
#define CoCreateInstance  CoCreateInstance 
#define CLSIDFromString  CLSIDFromString 
#define IIDFromString  IIDFromString 
#define StringFromGUID2 StringFromGUID2
#define CoSetProxyBlanket CoSetProxyBlanket
#define CoTaskMemAlloc CoTaskMemAlloc
#define CoTaskMemFree CoTaskMemFree
#define SysAllocString SysAllocString
#define SysReAllocString SysReAllocString
#define SysFreeString SysFreeString
#define SysStringLen SysStringLen
#define VariantInit VariantInit
#define VariantClear VariantClear
#define SysAddRefString SysAddRefString
#define VariantChangeType VariantChangeType
#define VarFormatDateTime VarFormatDateTime
#define SafeArrayDestroy SafeArrayDestroy
#define SafeArrayLock SafeArrayLock
#define SafeArrayGetLBound SafeArrayGetLBound
#define SafeArrayGetUBound SafeArrayGetUBound
#define SafeArrayGetElement SafeArrayGetElement
#define SafeArrayGetElemsize SafeArrayGetElemsize
#define SafeArrayAccessData SafeArrayAccessData
#define SafeArrayUnaccessData SafeArrayUnaccessData

#define DBGHELP$MiniDumpWriteDump MiniDumpWriteDump
#define WLDAP32$ldap_init ldap_init
#define WLDAP32$ldap_bind_s ldap_bind_s
#define WLDAP32$ldap_search_s ldap_search_s
#define WLDAP32$ldap_count_entries ldap_count_entries
#define WLDAP32$ldap_get_values_lenA  ldap_get_values_lenA 
#define WLDAP32$ldap_value_free_len ldap_value_free_len
#define WLDAP32$ldap_set_optionA ldap_set_optionA
#define WLDAP32$ldap_search_init_pageA ldap_search_init_pageA
#define WLDAP32$ldap_get_paged_count ldap_get_paged_count
#define WLDAP32$ldap_get_next_page_s ldap_get_next_page_s
#define WLDAP32$ldap_first_entry ldap_first_entry
#define WLDAP32$ldap_next_entry ldap_next_entry
#define WLDAP32$ldap_first_attribute ldap_first_attribute
#define WLDAP32$ldap_count_values ldap_count_values
#define WLDAP32$ldap_get_values ldap_get_values
#define WLDAP32$ldap_value_free ldap_value_free
#define WLDAP32$ldap_next_attribute ldap_next_attribute
#define WLDAP32$ber_free ber_free
#define WLDAP32$ldap_memfree ldap_memfree
#define WLDAP32$ldap_unbind ldap_unbind
#define WLDAP32$ldap_unbind_s ldap_unbind_s
#define WLDAP32$ldap_msgfree ldap_msgfree
#define RPCRT4$UuidToStringA UuidToStringA
#define RPCRT4$RpcStringFreeA RpcStringFreeA
#define PSAPI$EnumProcessModulesEx EnumProcessModulesEx
#define PSAPI$GetModuleFileNameExA GetModuleFileNameExA
#define VERSION$GetFileVersionInfoSizeA GetFileVersionInfoSizeA
#define VERSION$GetFileVersionInfoA GetFileVersionInfoA
#define VERSION$VerQueryValueA VerQueryValueA
#endif

#define BeaconPrintf(x, y, ...) printf(y, ##__VA_ARGS__)
#define internal_printf printf
#endif
