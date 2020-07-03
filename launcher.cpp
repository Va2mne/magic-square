#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

using std::cout;
using std::cin;


int main()
{
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi = { 0 };
	BOOL bSuccess;
	std::string genProg;
	std::string checkProg;

	cout << "The program that generates the matrix(.exe): ";
	std::getline(cin, genProg);
	
	cout << "The program that checks the matrix(.exe): ";
	std::getline(cin, checkProg);;
	cout << "\nStarting the generator";
	bSuccess = CreateProcess(NULL, (LPSTR)genProg.c_str(),
		NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
		NULL, NULL, &si, &pi);

	if (bSuccess)
	{
		cout << "\nGenerator started!";
		
		WaitForSingleObject(pi.hProcess, INFINITE);
		cout << "\nStarting the checker!";
		bSuccess = CreateProcess(NULL, (LPSTR)checkProg.c_str(),
			NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
			NULL, NULL, &si, &pi);
		if (bSuccess)
		{
			cout << "\nChecker Started!";
			
			WaitForSingleObject(pi.hProcess, INFINITE);		
			
				
			
			
		}
		else
		{
			cout << "\nUnable to start the Checker!";
			return 0;
		}
	}
	else
	{
		cout << "\nUnable to start the Generator!";
		return 0;
	}
	cout << "\nDone!\n";
	system("pause");
	return 0;
}