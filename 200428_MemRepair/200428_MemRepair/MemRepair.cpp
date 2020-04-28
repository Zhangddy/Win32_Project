#include "MemRepair.h"
#pragma warning(disable: 4996)      
// Windows��4KBΪ��λ�����ڴ�, ��ҳ������Ŀ���ڴ�����������Ч��
extern HANDLE g_hProcess;
extern unordered_set<DWORD> g_HashMap;

BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	// ��ȡ1ҳ�ڴ�
	BYTE arBytes[4096];
	if (!ReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL))
	{
		return FALSE;
	}

	// ����1ҳ�в���
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

	// ���û���ַ�ռ��������, Win98ΪӦ�õ�ַԤ����ַΪ4MB��2GB
	// Win2000Ԥ����ַΪ64KB��2GB
	// �����жϲ���ϵͳ����, ���Ծ�����ʼ��ַ
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(vi) };
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		dwBase = 4 * 1024 * 1024; // Win98ϵ��, 4MB��
	}
	else
	{
		dwBase = 64 * 1024;
	}

	// �ӿ�ʼ��ַ��2GB�ĵ�ַ�ռ���в���
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
	// ��g_HashMap�в���
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

