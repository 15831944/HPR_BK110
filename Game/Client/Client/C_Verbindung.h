#pragma once

extern char debug;
extern SOCKET ConnectSocket;
extern HWND hWnd;
extern unsigned char Spieler;

char Verbindung_INIT();
void senden(unsigned char);
void empfangen();
void cleanup();
