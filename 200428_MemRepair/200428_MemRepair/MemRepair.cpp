#include "MemRepair.h"
#pragma warning(disable: 4996)      
// Windows以4KB为单位分配内存, 按页来搜索目标内存可以提高搜索效率
extern HANDLE g_hProcess;
extern unordered_set<DWORD> g_HashMap;

BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	// 读取1页内存
	BYTE arBytes[4096];
	if (!ReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL))
	{
		return FALSE;
	}

	// 在这1页中查找
	DWORD* pdw;
	for (int i = 0; i < 4 * 1024 - 3; i++)
	{
		pdw = (DWORD*)&arBytes[i];
		if (pdw[0] == dwValue)
		{
			if (g_HashMap.size() >= 1024)
			{
				return FALSE;
			}
			else
			{
				g_HashMap.insert(dwBaseAddr + i);
			}
		}
	}
	return TRUE;
}

BOOL FindFirst(DWORD dwValue)
{
	const DWORD dwOneGB = 1024 * 1024 * 1024;
	const DWORD dwOnewPage = 4 * 1024;

	if (g_hProcess == NULL)
	{
		return FALSE;
	}

	// 在用户地址空间进行搜索, Win98为应用地址预留地址为4MB至2GB
	// Win2000预留地址为64KB至2GB
	// 故需判断操作系统类型, 用以决定开始地址
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(vi) };
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		dwBase = 4 * 1024 * 1024; // Win98系列, 4MB起
	}
	else
	{
		dwBase = 64 * 1024;
	}

	// 从开始地址到2GB的地址空间进行查找
	for (; dwBase < 2 * dwOneGB; dwBase += dwOnewPage)
	{
		CompareAPage(dwBase, dwValue);
	}
	return TRUE;
}

void ShowHash()
{
	for (auto& e : g_HashMap)
	{
		cout << e << endl;
	}
}

BOOL FindNext(DWORD dwValue)
{
	// 在g_HashMap中查找
	BOOL bRet = FALSE;
	DWORD dwReadValue;
	unordered_set<DWORD> newSet;
	for (auto& e : g_HashMap)
	{
		if (ReadProcessMemory(g_hProcess, (LPVOID)e, &dwReadValue, sizeof(DWORD), NULL))
		{
			if (dwReadValue == dwValue)
			{
				newSet.insert(e);
				bRet = TRUE;
			}
		}
	}
	g_HashMap.swap(newSet);
	return bRet;
}

BOOL WriteMemory(DWORD dwAddr, DWORD dwValue)
{
	return WriteProcessMemory(g_hProcess, (LPVOID)dwAddr, &dwValue, sizeof(DWORD), NULL);
}

HANDLE FindProcess(DWORD ProcessID)
{
	PROCESSENTRY32 _pe32;
	_pe32.dwSize = sizeof(_pe32);

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << "调用失败" << endl;
		return NULL;
	}
	DWORD bMore = Process32First(hProcessSnap, &_pe32);

	bool status = false;
	while (bMore)
	{
		if (_pe32.th32ProcessID == ProcessID)
		{
			cout << "Find ID: " << ProcessID << " 进程名称: ";
			printf("%ws", _pe32.szExeFile);
			status = true;
			break;
		}

		bMore = Process32Next(hProcessSnap, &_pe32);
	}
	CloseHandle(hProcessSnap);
	if (status == true)
	{
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	}
	else
	{
		return NULL;
	}
}

void ShowProcess()
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << "调用失败" << endl;
		return;
	}

	BOOL bMore = Process32First(hProcessSnap, &pe32);

	while (bMore)
	{
		cout << "进程名称: ";
		printf("%-30ws", pe32.szExeFile);
		cout << "   进程ID: " << pe32.th32ProcessID << endl;

		bMore = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);

}

