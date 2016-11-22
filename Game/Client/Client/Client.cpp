// Client.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
//

#include "stdafx.h"
#include "C_Verbindung.h"
#include "C_Spiel.h"
#include "C_Messung.h"

HWND hWnd = NULL;
SOCKET iClient = INVALID_SOCKET;				// �ber diesen Socket wird die Verbindung zum Server hergestellt

LRESULT CALLBACK WSClientProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP)
{
	switch (msg)
	{

	case WM_SOCKET:							// WINSOCK-Messages
	{
		switch (WSAGETSELECTEVENT(lP))
		{
		case FD_CONNECT:				// Nachricht von connect()
			cout << "FD_CONNECT" << endl;
			if (WSAGETSELECTERROR(lP) == 0)
			{
				//EnableWindow(GetDlgItem(hWnd, IDC_CLOSE), TRUE);
				//EnableWindow(GetDlgItem(hWnd, IDC_CONNECT), FALSE);
				//EnableWindow(GetDlgItem(hWnd, IDC_SEND), TRUE);
			}
			else
			{
				closesocket(iClient);
				iClient = INVALID_SOCKET;
				cout << "Kein Server!" << endl;
				//EnableWindow(GetDlgItem(hWnd, IDC_CONNECT), TRUE);
				//EnableWindow(GetDlgItem(hWnd, IDC_CLOSE), FALSE);
				//EnableWindow(GetDlgItem(hWnd, IDC_SEND), FALSE);
			}
			break;







		case FD_READ:					// Daten eingegangen
		{ char readbytes[255];
		int  iReadLen;

		cout << "FD_READ" << endl;
		// Alle verf�gbaren Daten einlesen
		iReadLen = recv(iClient, readbytes, sizeof(readbytes) - 1, 0);
		if (iReadLen > 0)
		{
			readbytes[iReadLen] = 0;
			//SendMessage(GetDlgItem(hWnd, IDC_RECEIVE),LB_ADDSTRING,(WPARAM)-1,(LPARAM)readbytes);
		}
		}
		break;

		case FD_CLOSE:					// Server hat Verbindung beendet
			cout << "FD_CLOSE" << endl;
			//EnableWindow(GetDlgItem(hWnd, IDC_SEND), FALSE);
			//EnableWindow(GetDlgItem(hWnd, IDC_CONNECT), TRUE);
			//EnableWindow(GetDlgItem(hWnd, IDC_CLOSE), FALSE);
			closesocket(iClient);
			iClient = INVALID_SOCKET;
			break;
		}
	}
	break;

	case WM_COMMAND:						// Schaltfl�chen-Nachrichten
		switch (LOWORD(wP))
		{
		case IDCANCEL:                  // CloseBox des Fensters
			EndDialog(hWnd, 0);
			break;

		case IDC_SEND:					// Daten senden
			if (iClient != INVALID_SOCKET)
			{
				char Buffer[255];
				int  iSendLen;
				cout << "FD_WRITE" << endl;
				// Serveradresse einlesen
				iSendLen = GetWindowText(GetDlgItem(hWnd, IDC_SENDDATA), Buffer, sizeof(Buffer));
				cout << "send()...";

				if (send(iClient, Buffer, iSendLen, 0) != SOCKET_ERROR)
				{
					cout << "... fertig" << endl;
					//EnableWindow(GetDlgItem(hWnd, IDC_SEND), TRUE);
				}
				else
				{
					if (WSAGetLastError() == WSAEWOULDBLOCK)
						cout << "... blockiert" << endl;
					else
						cout << "Fehler bei send()" << endl;

					cout << "closesocket()" << endl;
					//EnableWindow(GetDlgItem(hWnd, IDC_SEND), TRUE);
					closesocket(iClient);
					iClient = INVALID_SOCKET;
					cout << "Abbruch" << endl;
				}
			}
			break;

		case IDC_CONNECT:
		{ SOCKADDR_IN sin;
		char remoteIP[64];
		char Buffer[80];
		// Zieladresse auslesen
		GetWindowText(GetDlgItem(hWnd, IDC_DESTADDRESS),remoteIP,sizeof(remoteIP));

		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(SERVER_PORT);
		// IP-Adresse -> Dot Notation
		cout << "inet_addr()" << endl;
		sin.sin_addr.s_addr = inet_addr(remoteIP);

		// Adresse �ber DNS aufl�sen
		if (sin.sin_addr.s_addr == INADDR_NONE)
		{
			HOSTENT *he;
			cout << "gethostbyname()" << endl;
			he = gethostbyname(remoteIP);
			if (he)
				sin.sin_addr.s_addr = *((DWORD*)he->h_addr);
			else
			{
				cout << "ung�litige Internet-Adresse" << endl;
				break;
			}
		}
		// Zieladresse protokollieren
		wsprintf(Buffer, "Adresse: 0x%08lx",
			ntohl(sin.sin_addr.s_addr));
		cout << Buffer << endl;
		// Socket erzeugen
		cout << "socket()" << endl;
		iClient = socket(AF_INET, SOCK_STREAM, 0);

		if (iClient == INVALID_SOCKET)
		{
			cout << "Fehler beim Allokieren des Connect-Sockets" << endl;
			cout << "Kein Verbindungsaufbau m�glich" << endl;
			break;
		}

		// asynchronen Mode aktivieren
		cout << "WSAAsyncSelect()" << endl;
		if (WSAAsyncSelect(iClient,
			hWnd,
			WM_SOCKET,
			FD_CONNECT |
			FD_READ |
			FD_CLOSE) == 0)
		{
			cout << "connect()" << endl;
			if (connect(iClient, (SOCKADDR*)&sin, sizeof(sin))
				== SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					cout << "Warte..." << endl;
					// Senden-Schalfl�che deaktivieren
					//EnableWindow(GetDlgItem(hWnd, IDC_SEND),FALSE);
					break;
				}
			}
		}
		else
			cout << "Fehler bei WSAAsyncSelect()" << endl;

		cout << "closesocket()" << endl;
		closesocket(iClient);
		iClient = INVALID_SOCKET;
		}
		break;
		case IDC_CLOSE:
			cout << "closesocket()" << endl;
			closesocket(iClient);
			iClient = INVALID_SOCKET;
			//EnableWindow(GetDlgItem(hWnd, IDC_CLOSE), FALSE);
			//EnableWindow(GetDlgItem(hWnd, IDC_CONNECT), TRUE);
			//EnableWindow(GetDlgItem(hWnd, IDC_SEND), FALSE);
			break;
		}
		break;

	case WM_DESTROY:						// Fenster schlie�en
	{ if (iClient != INVALID_SOCKET)		// Socket schlie�en
		closesocket(iClient);
	}
	break;
	}
	return FALSE;
}

bool WINAPI ConsoleHandler(DWORD CEvent)
{
	char mesg[128];

	switch (CEvent)
	{
	case CTRL_C_EVENT:
		//MessageBox(NULL,"CTRL+C received!", "CEvent", MB_OK);
		break;
	case CTRL_BREAK_EVENT:
		//MessageBox(NULL,"CTRL+BREAK received!", "CEvent", MB_OK);
		break;
	case CTRL_CLOSE_EVENT:
		//MessageBox(NULL,"Program being closed!", "CEvent", MB_OK);
		break;
	case CTRL_LOGOFF_EVENT:
		//MessageBox(NULL,"User is logging off!", "CEvent", MB_OK);
		break;
	case CTRL_SHUTDOWN_EVENT:
		//MessageBox(NULL,"User is logging off!", "CEvent", MB_OK);
		break;

	}
	return TRUE;
}

int main()
{
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return -1;
	}

	LPCWSTR class_name = _T("HiddenWindow");
	WNDCLASSEX wx = {};
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = (WNDPROC) WSClientProc; // pWndProc;        // function which will handle messages
	wx.hInstance = NULL; // current_instance;
	wx.lpszClassName = class_name;
	if (RegisterClassEx(&wx)) {
		hWnd = CreateWindowEx(0, class_name, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
	}

	Verbindung_INIT(); //Eingabe IP-Adresse, Warten auf klingeln, evtl. der Rest auch

	while (42) {
		//bei Broadcasts Event -> C_Verbindung empfangen()
		
		//bei Tastendruck Event C_Spiel Tastendruck()
/*		while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
		{
			
			if (bRet == -1)
			{
				// handle the error and possibly exit
			}
			else
			{
				if (FilterMessage(&msg) == 0) // <-- Function you write
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			
		}*/
	}
    return 0;
}

