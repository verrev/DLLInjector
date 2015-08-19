#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <fstream>

bool startProcess(const char *exePath)
{
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	bool retVal = CreateProcess(exePath, nullptr, nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return retVal;
}

DWORD getProcessID(const std::string &procName)
{
	DWORD id = 0;
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	HANDLE processes = 0;
	while (!id) {
		processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (Process32First(processes, &pe)) {
			do {
				if (!strcmp(pe.szExeFile, procName.c_str())) {
					id = pe.th32ProcessID;
					break;
				}
			} while (Process32Next(processes, &pe));
		}
	}
	CloseHandle(processes);
	return id;
}

void inject(const std::string &processName, const std::string &dllPath, DWORD sleep = 0)
{
	Sleep(sleep);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, getProcessID(processName));
	void *address = VirtualAllocEx(process, 0, dllPath.size(), MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(process, address, dllPath.c_str(), dllPath.size(), 0);
	CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(LoadLibrary("KERNEL32"), "LoadLibraryA"), address, 0, 0);
	CloseHandle(process);
}

int main(int argc, char **argv)
{
	std::ifstream inFile("DLLInjector.txt");
	if (inFile.is_open() && inFile.good()) {
		std::string procName = "", dllPath = "", exePath = "";
		inFile >> procName;
		inFile >> dllPath;

		bool start = 0;
		inFile >> (bool) start;
		inFile >> exePath;
		DWORD sleep = 0;
		inFile >> sleep;
		inFile.close();
		
		if (start) startProcess(exePath.c_str());
		inject(procName, dllPath, sleep);
	}
	else {
		std::ofstream outFile("DLLInjector.txt");
		if (outFile.is_open() && outFile.good()) {
			outFile << "example.exe\n" << "yourDLL.dll\n" << "1\n" << "c:/users/username/program files(x86)/game/game.exe\n" << "0";
			outFile.close();
		}
	}
	return 0;
}