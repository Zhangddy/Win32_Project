#include <cstdio>
#include <Windows.h>
#include <iostream>
using namespace std;


int main()
{
	int nSelect = MessageBox(NULL, L"Hello, Windows 10", L"Greetings", MB_OKCANCEL);

	if (nSelect == IDOK)
	{
		printf("�û�ѡ���� ȷ�� ��ť \n");
	}
	else
	{
		printf("�û�ѡ���� ȡ�� ��ť \n");
	}
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	//wchar_t szCommandLine[20] = L"notepad.exe";
	wchar_t szCommandLine[] = L"D:\\Code\\Cpp_program\\200428_test\\Debug\\200428_test.exe";
	::CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	return 0;
}