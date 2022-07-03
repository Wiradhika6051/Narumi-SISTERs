#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
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
	DWORD ps_list[1024], ps_memctr, ps_ctr;
	if (!EnumProcesses(ps_list, sizeof(ps_list), &ps_memctr))
		return;
	ps_ctr = ps_memctr / sizeof(DWORD);
	for (int i = 0; i < ps_ctr; i++)
		if (ps_list[i] != 0)
			print_ps_name(ps_list[i]);
}
void print_ps__With_name(DWORD pid,std::string name) {
	TCHAR pname[MAX_PATH] = TEXT("<unknown>");
	TCHAR target[MAX_PATH] = TEXT(name);
	HANDLE phandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (phandle != NULL) {
		HMODULE pmod;
		DWORD temp;
		if (EnumProcessModules(phandle, &pmod, sizeof(pmod), &temp))
			GetModuleBaseName(phandle, pmod, pname, sizeof(pname) / sizeof(TCHAR));
	}
	if (pname == name) {
		_tprintf(TEXT("%s(PID %u)\n"), pname, pid);
	}
	CloseHandle(phandle);
}
void print_named_pid(std::string name) {
	DWORD ps_list[1024], ps_memctr, ps_ctr;
	if (!EnumProcesses(ps_list, sizeof(ps_list), &ps_memctr))
		return;
	ps_ctr = ps_memctr / sizeof(DWORD);
	for (int i = 0; i < ps_ctr; i++)
		if (ps_list[i] != 0)
			print_ps__With_name(ps_list[i],name);
}
int main() {
	//print_ps();
	// TODO : Add memory editor
	std::cout <<std::endl<< "Masukkan proses target:" << std::endl;
	std::string name;
	getline(std::cin, name);
	std::cout << "Nama Aplikasi: "<<name;
	print_named_pid(name);

	return 0;
}