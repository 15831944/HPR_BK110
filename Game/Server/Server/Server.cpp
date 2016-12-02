// Server.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
//

#include "stdafx.h"
#include "S_Verbindung.h"
#include "S_Spiel.h"
#include "S_Messung.h"

//Event Handling for Console Window
bool WINAPI ConsoleHandler(DWORD CEvent)
{
	switch (CEvent)
	{
	case CTRL_C_EVENT:
		printf("Ctrl-C event\n");
		cleanup();
		break;
	case CTRL_BREAK_EVENT:
		printf("Ctrl-Break event\n");
		cleanup();
		break;
	case CTRL_CLOSE_EVENT:
		printf("Ctrl-Close event\n");
		cleanup();
		break;
	case CTRL_LOGOFF_EVENT:
		printf("Ctrl-Logoff event\n");
		cleanup();
		break;
	case CTRL_SHUTDOWN_EVENT:
		printf("Ctrl-Shutdown event\n");
		cleanup();
		break;
	}
	return TRUE;
}

int main(){
	//Event-Handler f�r Consolen-Fenster (zum Aufr�umen bei Programmabbruch)
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return -1;
	}

	Verbindung_INIT();	//TCP-Port �ffnen, Aufforderung zum Verbinden, Spielerhandle speichern, Spielstart wenn alle geklingelt haben
	//Spiel_INIT(); //Karten verteilen, Spielfeld verschicken (wird nun auch in Empfangsschleife gestartet)
	cout << endl << "Abbruch mit ESC"; //
	while (42) { // ende wenn Spiel beendet / "esc" auf Server
		// S_Verbindung - empfangen() (fragt Paketeingang ab)
	}


    return 0;
}

