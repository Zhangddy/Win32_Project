#include "MemRepair.h"

HANDLE g_hProcess;
unordered_set<DWORD> g_HashMap;

int main()
{
	//wchar_t szFileName[] = L"D:\\Code\\Cpp_program\\200428_test\\Debug\\200428_test.exe";
	wchar_t szFileName[] = L"D:\\jianyushijishi\\Prison Architect v12b\\Prison Architect64.exe";

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, szFileName, NULL, NULL, FALSE
					, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	g_hProcess = pi.hProcess;

	int val;
	cout << "Input val = ";
	cin >> val;
	FindFirst(val);
	ShowHash();
	cout << "----------------" << endl;
	while (g_HashMap.size() > 1)
	{
		cout << "Input val = ";
		cin >> val;

		FindNext(val);

		ShowHash();
		cout << "----------------" << endl;
	}

	cout << "Input new val = ";
	cin >> val;
	auto it = g_HashMap.begin();

	if (WriteMemory(*it, val))
	{
		cout << "Write data success!" << endl;
	}
	else
	{
		cout << "Write data fail!" << endl;
	}

	return 0;
}