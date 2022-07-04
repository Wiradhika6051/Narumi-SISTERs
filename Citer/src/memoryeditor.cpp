#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include<TlHelp32.h>
#include<shlwapi.h>
#include "atlstr.h"
#pragma comment(lib, "psapi.lib")
DWORD* getListOfProcessId(DWORD& processCount);
DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID);
void print_ps_name(DWORD pid) {
	TCHAR pname[MAX_PATH] = TEXT("<unknown>");
	HANDLE phandle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
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
	for (DWORD i = 0; i < ps_ctr; i++)
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
		for (DWORD i = 0; i < p_count; i++) {
			if (processIdList[i] != 0) {
				phandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processIdList[i]);
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
DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
	//buat nyari base address dari modul/aplikasi tertentu
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); 
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
	{
		do {
			if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
			{
				dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}
int main() {
	//print_ps();
	// TODO : Add memory editor
	std::cout << "Masukkan proses target: " << std::endl;
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
	//DWORD baseAddr;
	LONGLONG addrMemoriTarget;
	SIZE_T bytesRead;
	SIZE_T bytesWrite;
	//memset(buffer, 0x0, 100);
	if (processHandler != NULL) {
//		_tprintf(TEXT("%d\n"), id);
		//cetak proses
		_tprintf(TEXT("Proses %s ditemukan!! pid: %d\n"), namaApp, id);
		//masukkan memori target:
		char input_string[17];
		std::cout << "Masukkan address target: ";
		scanf("%s", input_string);
		StrToInt64Ex(input_string, STIF_SUPPORT_HEX, &addrMemoriTarget);
		_tprintf(_T("Alamat target: 0x%I64x\n"), addrMemoriTarget);
		//baca data
		//tentukan tipe datanya
		std::string tipedata;
		std::cout << "Masukkan tipe data yang ingin diubah: ";
		std::cin >> tipedata;
		int bytesSize=0;
		//int bytesSize = 95;
		//sedian buffer yg mungkin
		//char buffer=NULL;

		BOOL statusBaca = true;
		BOOL statusTulis = true;
		if (tipedata == "string") {
			bytesSize = 256;
			char buffer[256];
			memset(buffer, 0x0, 100);
			statusBaca = ReadProcessMemory(processHandler, (LPCVOID)addrMemoriTarget, (LPVOID)buffer, bytesSize, &bytesRead);
			if (statusBaca) {
				_tprintf(_T("Pembacaan memori pada alamat 0x%I64x Sukses! Nilai yang dibaca: \"%s\"\n"), addrMemoriTarget, buffer);
				_tprintf(_T("Jumlah bytes terbaca: %d\n"), bytesRead);
			}
			//ubah nilainya
			std::string newStr;
			std::cout << "Masukkan string baru:\n";
			getchar();
			getline(std::cin, newStr);
			char* newVal = new char[newStr.length() + 1];
			strncpy(newVal, newStr.c_str(), newStr.length() + 1);
			newVal[newStr.length()] = '\0';
			USES_CONVERSION;
			CHAR* nilaiBaru = A2T(newVal);
			//tulis ke memori
			statusTulis = WriteProcessMemory(processHandler, (LPVOID)addrMemoriTarget, (LPVOID)nilaiBaru, strlen(nilaiBaru) + 1, &bytesWrite);
	}
		else if (tipedata == "int") {
			bytesSize = 4;
			int value;
			statusBaca = ReadProcessMemory(processHandler, (LPCVOID)addrMemoriTarget, (LPVOID)&value, bytesSize, &bytesRead);
			if (statusBaca) {
				_tprintf(_T("Pembacaan memori pada alamat 0x%I64x Sukses! Nilai yang dibaca: 0x%x\n"), addrMemoriTarget, value);
				_tprintf(_T("Jumlah bytes terbaca: %d\n"), bytesRead);
			}
			//ubah nilainya
			std::cout << "Masukkan nilai baru: ";
			std::cin >> value;
			//tulis ke memori
			statusTulis = WriteProcessMemory(processHandler, (LPVOID)addrMemoriTarget, (LPVOID)&value, sizeof(int), &bytesWrite);
		}
		else if (tipedata == "char") {
			bytesSize = 1;
			char value;
			statusBaca = ReadProcessMemory(processHandler, (LPCVOID)addrMemoriTarget, (LPVOID)&value, bytesSize, &bytesRead);
			if (statusBaca) {
				_tprintf(_T("Pembacaan memori pada alamat 0x%I64x Sukses! Nilai yang dibaca: 0x%x\n"), addrMemoriTarget, value);
				_tprintf(_T("Jumlah bytes terbaca: %d\n"), bytesRead);
			}
			//ubah nilainya
			std::cout << "Masukkan nilai baru: ";
			std::cin >> value;
			//tulis ke memori
			statusTulis = WriteProcessMemory(processHandler, (LPVOID)addrMemoriTarget, (LPVOID)&value, sizeof(char), &bytesWrite);
		}
		//else if (tipedata == "float") {
		//	bytesSize = sizeof(float);
		//	float value;
		//	statusBaca = ReadProcessMemory(processHandler, (LPCVOID)addrMemoriTarget, (LPVOID)&value, bytesSize, &bytesRead);
		//	if (statusBaca) {
		//		_tprintf(_T("Pembacaan memori pada alamat 0x%I64x Sukses! Nilai yang dibaca: 0x%x\n"), addrMemoriTarget, value);
		//		_tprintf(_T("Jumlah bytes terbaca: %d\n"), bytesRead);
		//	}
		//	//ubah nilainya
		//	std::cout << "Masukkan nilai baru: ";
		//	std::cin >> value;
		//	//tulis ke memori
		//	statusTulis = WriteProcessMemory(processHandler, (LPVOID)addrMemoriTarget, (LPVOID)&value, sizeof(float), &bytesWrite);
		//}
		//yg kurang:double,wchar_t,bool,short
		if (!statusBaca) {
			//jika gagal
			_tprintf(_T("Pembacaan memori pada alamat 0x%I64x gagal!\n"), addrMemoriTarget);
		}
		if (statusTulis) {
			_tprintf(_T("Penulisan ke memori pada alamat 0x%I64x Sukses!\n"), addrMemoriTarget);
			_tprintf(_T("Jumlah bytes yang ditulis: %d\n"), bytesWrite);
		}
		else if (!statusTulis) {
			//jika gagal
			_tprintf(_T("Penulisan ke memori pada alamat 0x%I64x gagal!\n"), addrMemoriTarget);
		}
	}
	else {
		_tprintf(TEXT("Proses %s tidak ditemukan!\n"), namaApp);
	}
	//print_named_pid(name);
	//print_ps();
	return 0;
}