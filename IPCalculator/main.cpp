
#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<CommCtrl.h>
#include<cstdio>
#include"resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hSpin = GetDlgItem(hwnd, IDC_SPIN_PREFIX);
		SendMessage(hSpin, UDM_SETRANGE32, 0, 32);

		SetFocus(GetDlgItem(hwnd, IDC_IPADDRESS));
	}
	break;
	case WM_COMMAND:
	{
		CONST INT SIZE = 5;
		CHAR sz_prefix[SIZE]{};
		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
		{
			//if (HIWORD(wParam) == EN_CHANGE)
			//{
			//	//BYTE = 8 bit
			//	//WORD = 2 BYTE = 16 bit;
			//	//DWORD = 4 BYTE = 32 bit;
			//	//QWORD = 8 BYTE = 64 bit;
			//	//TBYTE = 10 BYTE = 80 bit;
			//	//DQWORD = 16 BYTE = 128 bit;
			//	DWORD dwAddress = 0;
			//	//https://learn.microsoft.com/en-us/windows/win32/controls/ipm-getaddress
			//	SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwAddress);
			//	BYTE first = FIRST_IPADDRESS(dwAddress);
			//	BYTE dw_prefix = 0;
			//	if (first < 128)dw_prefix = 8;
			//	else if (first < 192)dw_prefix = 16;
			//	else if (first < 224)dw_prefix = 24;
			//	CHAR sz_prefix[5]{};
			//	sprintf(sz_prefix, "%i", dw_prefix);
			//	SendMessage(hEditPrefox, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			//}
		}
		break;
		case IDC_EDIT_PREFIX:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				//MessageBox(hwnd, "Profix changed", "Notification", MB_OK);
				SendMessage(hEditPrefix, WM_GETTEXT, SIZE, (LPARAM)sz_prefix);
				DWORD dw_prefix = atoi(sz_prefix);
				DWORD dw_mask = UINT_MAX;
				ULONG_MAX;
				//dw_mask >>= (32 - dw_prefix);
				dw_mask <<= (32 - dw_prefix);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dw_mask);
			}
		}
		break;
		}
	}
	break;
	case WM_NOTIFY:
	{
		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		switch (wParam)
		{
		case IDC_IPADDRESS:
		{
			NMIPADDRESS* pAddress = ((NMIPADDRESS*)(lParam));
			if (pAddress->iField == 0)
			{
				DWORD dw_prefix = 0;
				if (pAddress->iValue < 128)dw_prefix = 8;
				else if (pAddress->iValue < 192)dw_prefix = 16;
				else if (pAddress->iValue < 224)dw_prefix = 24;
				CHAR sz_prefix[5]{};
				sprintf(sz_prefix, "%i", dw_prefix);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			}
		}
		break;
		case IDC_IPMASK:
		{
			//NMIPADDRESS* pMask = ((NMIPADDRESS*)(lParam));
			DWORD dw_mask = 0;
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dw_mask);
			DWORD dw_prefix = 32;
			for (; dw_mask & 1 ^ 1; dw_mask >>= 1)dw_prefix--;
			CONST INT SIZE = 5;
			CHAR sz_prefix[SIZE]{};
			sprintf(sz_prefix, "%i", dw_prefix);
			SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
		}
		break;
		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}
