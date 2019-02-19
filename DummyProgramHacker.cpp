#include <pch.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include <vector>

using namespace std;

int writeProcess(DWORD pid, DWORD address, DWORD valueSet) {

	//writing to process memory
	//open the process specified

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL) { // Failed to get a handle
		cout << "OpenProcess failed. GetLastError = " << dec << GetLastError() << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	//write to memory
	BOOL wpmReturn = WriteProcessMemory(hProcess, (LPVOID)address, &valueSet, sizeof(int), NULL);
	if (wpmReturn == FALSE) {
		cout << "WriteProcessMemory failed. GetLastError = " << dec << GetLastError() << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	return 0;
}

int getID(wstring processName) {

	//get the id of the process

	int pid = 0;

	std::vector<DWORD> pids;
	std::wstring targetProcessName = processName;

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //all processes

	PROCESSENTRY32W entry; //current process
	entry.dwSize = sizeof entry;

	if (!Process32FirstW(snap, &entry)) { //start with the first in snapshot
		return 0;
	}

	do {
		if (std::wstring(entry.szExeFile) == targetProcessName) {
			pids.emplace_back(entry.th32ProcessID); //name matches; add to list
		}
	} while (Process32NextW(snap, &entry)); //keep going until end of snapshot

	for (int i(0); i < pids.size(); ++i) {
		std::cout << pids[i] << std::endl;
		pid = pids[i];
	}

	return pid;
}


int main()
{
	//addresses/offsets

	DWORD pLocalPlayer = 0x00509B74;
	DWORD playerObjectAddress;

	DWORD hpOffset = 0xF8;
	DWORD ammoOffset = 0x150;
	DWORD speedOffset = 0x80;

	DWORD hpAddress = 0x0;
	DWORD ammoAddress = 0x0;
	DWORD speedAddress = 0x0;

	//memory values to update to

	int hpSet = 1000;
	int ammoSet = 1000;
	int choice = 0;
	int godMode = 0;
	int infAmmo = 0;
	int speedHacks = 0;

	wstring procName = L"ac_client.exe";
	int pid = getID(procName);

	system("CLS");

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL) { // Failed to get a handle
		cout << "OpenProcess failed. Is the game open?";
		system("pause");
		return EXIT_FAILURE;
	}

	BOOL rpmReturn = ReadProcessMemory(hProcess, (LPCVOID)pLocalPlayer, &playerObjectAddress, sizeof(playerObjectAddress), NULL);

	hpAddress = playerObjectAddress + hpOffset;
	ammoAddress = playerObjectAddress + ammoOffset;
	speedAddress = playerObjectAddress + speedOffset;

	//user interface

	while (true)
	{
		cout << "YA BOYS CHEAT MENU - ASSAULT CUBE" << endl;
		cout << "---------------------------------" << endl;
		cout << "Pick an option:" << endl;
		cout << "1 - God Mode" << endl;
		cout << "2 - Infinite Ammo" << endl;
		cout << "3 - 2x Speed" << endl;
		cout << "Chice: ";
		cin >> choice;

		if (choice == 1) {
			godMode = 1;
		}

		if (choice == 2) {
			infAmmo = 1;
		}

		if (choice == 3) {
			speedHacks = 1;
		}

		while (godMode == 1) {
			writeProcess(pid, hpAddress, 99999);
		}
		while (infAmmo == 1) {
			writeProcess(pid, ammoAddress, 99999);
		}
		while (speedHacks == 1) {
			if (GetKeyState('W') & 0x8000) {
				writeProcess(pid, speedAddress, 2);
			}
			else {
				writeProcess(pid, speedAddress, 0);
			}
		}
	}

	cout << "Press ENTER to quit.";
	cout << "Changed address: " << hex << uppercase << hpAddress << endl;
	system("pause > nul");

}
