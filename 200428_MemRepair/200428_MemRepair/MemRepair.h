#pragma once
#include <Windows.h>
#include <iostream>
#include <unordered_set>
#include <TlHelp32.h>
#include <cstdio>
// 1. 搜索所在地址(搜索多次找到保存此值的唯一的地址)
// 2. 修改地址上的信息
using namespace std;


BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);
BOOL FindFirst(DWORD dwValue);
BOOL FindNext(DWORD dwValue);
BOOL WriteMemory(DWORD dwAddr, DWORD dwValue);
HANDLE FindProcess(DWORD ProcessID);
void ShowHash();
void ShowProcess();