#pragma once
#include <Windows.h>
#include <iostream>
#include <unordered_set>
// 1. �������ڵ�ַ(��������ҵ������ֵ��Ψһ�ĵ�ַ)
// 2. �޸ĵ�ַ�ϵ���Ϣ
using namespace std;


BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);
BOOL FindFirst(DWORD dwValue);
BOOL FindNext(DWORD dwValue);
BOOL WriteMemory(DWORD dwAddr, DWORD dwValue);
void ShowHash();