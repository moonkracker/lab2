#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <ctime>
#include <vector>

using namespace std;

int main()
{
	HANDLE _event_Time = CreateEvent(NULL, true, FALSE, ("Event_" + to_string(20202)).c_str());

	STARTUPINFOA cif0;
	ZeroMemory(&cif0, sizeof(STARTUPINFOA));
	PROCESS_INFORMATION pi0;

	CreateProcessA("C:/Users/monst/source/repos/lab2/Debug/lab2child.exe",
		NULL, 
		NULL,
		NULL, 
		FALSE, 
		CREATE_NEW_CONSOLE,
		NULL, 
		NULL, 
		&cif0,
		&pi0);

	Sleep(300);

	int _count = 1;

	vector<HANDLE> _vecEvent, _vecProcess;

	HANDLE _event_11 = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Event_" + to_string(1)).c_str());
	HANDLE _event_22 = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Event_" + to_string(2)).c_str());
	HANDLE _event_33 = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Event_" + to_string(3)).c_str());

	const HANDLE _masHandle[3] = { _event_11 , _event_22 ,_event_33 };

	STARTUPINFOA cif;
	ZeroMemory(&cif, sizeof(STARTUPINFOA));
	PROCESS_INFORMATION pi;

	while (1)
	{
		cout << "Number of processes: " << _vecProcess.size() << endl;
		switch (WaitForMultipleObjects(3, _masHandle, FALSE, 500))
		{
		case WAIT_FAILED:
			cout << "Code error: " << GetLastError() << endl;
			system("pause");
			break;
		case WAIT_TIMEOUT:
			for (int i = 0; i < _vecEvent.size(); i++)
			{
				if (WAIT_OBJECT_0 == WaitForSingleObject(_event_Time, 1)) {
					ResetEvent(_event_Time);
					break;
				}
				Sleep(1);
				PulseEvent(_vecEvent[i]);
				WaitForSingleObject(_vecEvent[i], INFINITE);

			}
			break;
		case WAIT_OBJECT_0 + 0:
			if (!CreateProcessA("C:/Users/monst/source/repos/lab2/Debug/lab2second_child.exe",
				const_cast<LPSTR>(("String - " + to_string(_count)).c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi))
				cout << "Process creation error!!!";
			else
			{
				_vecProcess.push_back(pi.hProcess);
				_vecEvent.push_back(CreateEvent(NULL, true, false, ("Event_" + to_string(pi.dwProcessId)).c_str()));
				_count++;
				ResetEvent(_masHandle[0]);
			}
			break;
		case WAIT_OBJECT_0 + 1:
			if (!_vecProcess.empty())
			{
				_count--;
				TerminateProcess(_vecProcess.back(), 0);
				CloseHandle(_vecEvent.back());
				CloseHandle(_vecProcess.back());
				_vecEvent.pop_back();
				_vecProcess.pop_back();
				ResetEvent(_masHandle[1]);
			}
			break;
		case WAIT_OBJECT_0 + 2:
			while (!_vecProcess.empty())
			{
				TerminateProcess(_vecProcess.back(), 0);
				CloseHandle(_vecEvent.back());
				CloseHandle(_vecProcess.back());
				_vecEvent.pop_back();
				_vecProcess.pop_back();
				ResetEvent(_masHandle[3]);
			}
			return 1;
		}
		system("CLS");
	}
	return 0;
}