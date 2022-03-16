#include <Windows.h>
#include <stdio.h>
#include <time.h>

HHOOK hook;
time_t seconds;
MSLLHOOKSTRUCT mouseStruct;

int idx = 0;
char* mouseMessage[] = { "default", "WM_LBUTTONDOWN", "WM_LBUTTONUP", "WM_MOUSEMOVE", "WM_MOUSEWHEEL",
"WM_MOUSEHWHEEL", "WM_RBUTTONDOWN", "WM_RBUTTONUP" };

LRESULT CALLBACK LLMP(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		mouseStruct = *((MSLLHOOKSTRUCT*)lParam);

		// LLMHF_INJECTED FLAG
		if (mouseStruct.flags & 0x01)
		{
			switch (wParam)
			{
				case WM_LBUTTONDOWN: idx = 1; break;
				case WM_LBUTTONUP: idx = 2; break;
				case WM_MOUSEMOVE: idx = 3; break;
				case WM_MOUSEWHEEL: idx = 4; break;
				case WM_MOUSEHWHEEL: idx = 5; break;
				case WM_RBUTTONDOWN: idx = 6; break;
				case WM_RBUTTONUP: idx = 7;
			}
			seconds = time(NULL);
			printf("[+] virtual mouse detected || type = %s || time (s) = %i\n", mouseMessage[idx], int(seconds));		
		}
	}

	return CallNextHookEx(hook, nCode, wParam, lParam);
}

void InstallHook()
{
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644990(v=vs.85).aspx
	if (!(hook = SetWindowsHookEx(WH_MOUSE_LL, LLMP, NULL, 0)))
	{
		MessageBox(NULL, L"failed to install hook", L"fail", MB_ICONERROR);
	}
}

void main()
{
	InstallHook();
	printf("|| Virtual Mouse Detector (VMD) ||");

	MSG gmsg;
	while (GetMessage(&gmsg, NULL, 0, 0))
	{
		continue;
	}
}
