#include "MemRepair.h"

HANDLE g_hProcess;
unordered_set<DWORD> g_HashMap;

int main()
{
	// wchar_t szFileName[] = L"D:\\Code\\Cpp_program\\200428_test\\Debug\\200428_test.exe";
	
	ShowProcess();

	DWORD ProcessID;
	cout << "Input ProcessID: ";
	cin >> ProcessID;
	g_hProcess = FindProcess(ProcessID);
	cout << endl << "-----------------------------" << endl;
	int val;
	cout << "Input val = ";
	cin >> val;
	FindFirst(val);
	ShowHash();
	cout << endl << "----------------" << endl;
	size_t countAddr = 0;
	while (g_HashMap.size() > 1 && countAddr != g_HashMap.size())
	{
		countAddr = g_HashMap.size();
		cout << "Input val = ";
		cin >> val;

		FindNext(val);
		if (FindNext == false)
		{
			cout << "找不到此数据! " << endl;
			return -1;
		}
		ShowHash();
		cout << "----------------" << endl;

	}

	cout << "Input new val = ";
	cin >> val;
	auto it = g_HashMap.begin();
	while (it != g_HashMap.end())
	{
		if (WriteMemory(*it, val))
		{
			cout << "Write data success!" << endl;
		}
		else
		{
			cout << "Write data fail!" << endl;
		}
		it++;
	}

	return 0;
}

//int main()
//{
//	// wchar_t szFileName[] = L"D:\\Code\\Cpp_program\\200428_test\\Debug\\200428_test.exe";
//	wchar_t szFileName[] = L"D:\\game\\GHra2-china\\ra2.exe";
//
//	STARTUPINFO si = { sizeof(si) };
//	PROCESS_INFORMATION pi;
//	BOOL bRet = CreateProcess(NULL, szFileName, NULL, NULL, FALSE
//		, 0, NULL, NULL, &si, &pi);
//	cout << "bRet: " << bRet << endl;
//	system("pause");
//	CloseHandle(pi.hThread);
//	g_hProcess = pi.hProcess;
//
//	int val;
//	cout << "Input val = ";
//	cin >> val;
//	FindFirst(val);
//	ShowHash();
//	cout << "----------------" << endl;
//	while (g_HashMap.size() > 1)
//	{
//		cout << "Input val = ";
//		cin >> val;
//
//		FindNext(val);
//
//		ShowHash();
//		cout << "----------------" << endl;
//	}
//
//	cout << "Input new val = ";
//	cin >> val;
//	auto it = g_HashMap.begin();
//
//	if (WriteMemory(*it, val))
//	{
//		cout << "Write data success!" << endl;
//	}
//	else
//	{
//		cout << "Write data fail!" << endl;
//	}
//
//	return 0;
//}