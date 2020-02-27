#include <iostream>
#include <Windows.h>
#include <string>

using namespace std;

int main()
{
	HANDLE _event = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Event_" + to_string(GetCurrentProcessId())).c_str());
	string str = to_string(GetCurrentProcessId()), message = GetCommandLineA();
	while (1)
	{
		WaitForSingleObject(_event, INFINITE);
		for (char a : message + "   id " + str)
		{
			cout << a;
			Sleep(1);
		}
		PulseEvent(_event);
		cout << endl;
	}
	return 0;
}