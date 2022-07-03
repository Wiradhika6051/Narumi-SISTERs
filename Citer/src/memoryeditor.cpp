#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include "atlstr.h"
#pragma comment(lib, "psapi.lib")
DWORD* getListOfProcessId(DWORD& processCount);
void print_ps_name(DWORD pid) {
	TCHAR pname[MAX_PATH] = TEXT("<unknown>");
	HANDLE phandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (phandle != NULL) {
		HMODULE pmod;
		DWORD temp;
		if (EnumProcessModules(phandle, &pmod, sizeof(pmod), &temp))
			GetModuleBaseName(phandle, pmod, pname, sizeof(pname) / sizeof(TCHAR));
	} 
	_tprintf(TEXT("%s(PID %u)\n"), pname, pid);
	CloseHandle(phandle);
}
void print_ps() {
	DWORD ps_ctr = 0;
	DWORD* ps_list = getListOfProcessId(ps_ctr);
	for (int i = 0; i < ps_ctr; i++)
		if (ps_list[i] != 0)
			print_ps_name(ps_list[i]);
}
DWORD* getListOfProcessId(DWORD& processCount) {
	DWORD ps_list[1024], ps_memctr;
	if (!EnumProcesses(ps_list, sizeof(ps_list), &ps_memctr))
		return NULL;
	processCount = ps_memctr / sizeof(DWORD);
	return ps_list;
}
HANDLE getProcess(TCHAR* name,DWORD& pid_) {
	DWORD p_count = 0;
	DWORD* processIdList = getListOfProcessId(p_count);
	HANDLE phandle;
	TCHAR pname[MAX_PATH] = TEXT("<unknown>");
	if (processIdList != NULL) {
		for (int i = 0; i < p_count; i++) {
			if (processIdList[i] != 0) {
				phandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIdList[i]);
				if (phandle != NULL) {
					HMODULE pmod;
					DWORD temp;
					if (EnumProcessModules(phandle, &pmod, sizeof(pmod), &temp))
						GetModuleBaseName(phandle, pmod, pname, sizeof(pname) / sizeof(TCHAR));
					if (_tcscmp(name, pname) == 0) {
						pid_ = processIdList[i];
						return phandle;
					}
				}
			}
		}
	}
	return NULL;
}
int main() {
	//print_ps();
	// TODO : Add memory editor
	std::cout <<std::endl<< "Masukkan proses target:" << std::endl;
	std::string name;
	getline(std::cin, name);
	char* procName = new char[name.length()+1];
	strncpy(procName, name.c_str(), name.length() + 1);
	procName[name.length()] = '\0';
	std::cout << "Nama Proses: "<<procName<<'\n';
	//konversi ke TCHAR
	USES_CONVERSION;
	TCHAR* namaApp = A2T(procName);
	//_tprintf(TEXT("%s\n"), namaApp); -->buat debug
	DWORD id;
	HANDLE processHandler= getProcess(namaApp,id);
	if (processHandler != NULL) {
//		_tprintf(TEXT("%d\n"), id);
		//cetak proses
		_tprintf(TEXT("Proses %s ditemukan!! pid: %d\n"), namaApp, id);
	}
	else {
		_tprintf(TEXT("Proses %s tidak ditemukan!\n"), namaApp, id);
	}
	//print_named_pid(name);
	//print_ps();
	return 0;
}