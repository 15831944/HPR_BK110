#include "stdafx.h"
#include "C_Verbindung.h"
#include "C_Spiel.h"
#include <iomanip>  // f�r setw()

unsigned char spielzug = 0;
unsigned char glob_tastensperre = 0;

void aktualisieren(unsigned char Spieleranzahl, unsigned char *kartenanzahl, unsigned char *offene_karte, unsigned char Nachricht) {
	// verarbeitet empfangenes Spielfeld
	string spieler[4];
	int farbe, wert;
	for (int i = 0; i < Spieleranzahl; i++) {
		spieler[i] = "";
		farbe = (int)(offene_karte[i] / 10);
		wert = (int)(offene_karte[i] % 10);
		for (int j = 0; j < wert; j++) {
			switch (farbe) {
			case 1:
				spieler[i] = spieler[i] + "A ";
				break;
			case 2:
				spieler[i] = spieler[i] + "B ";
				break;
			case 3:
				spieler[i] = spieler[i] + "C ";
				break;
			case 4:
				spieler[i] = spieler[i] + "D ";
				break;
			}
		}
	}
	system("CLS");
	cout << "H A L L I G A L L I" << endl << endl;
	cout << "a = Karte aufdecken, Leertaste = Klingeln, q = Ende" << endl << endl;
	cout << "Spieler: " << (int)Spieler << endl;
	cout << "Spielzug: " << (int)spielzug << endl;
	//cout << hex << (int)Nachricht << dec;
	// empfangenes Spielfeld ausgeben
	switch ((int)Spieleranzahl) {
	case 4:
		gotoxy(5, 12);
		cout << "Spieler 4: " << setw(2) << (int)kartenanzahl[3];
		gotoxy(20, 12);
		cout << setw(10) << left << spieler[3];
	case 3:
		gotoxy(33, 19);
		cout << "Spieler 3: " << setw(2) << (int)kartenanzahl[2];
		gotoxy(35, 14);
		cout << setw(10) << left << spieler[2];
	case 2:
		gotoxy(62, 12);
		cout << "Spieler 2: " << setw(2) << (int)kartenanzahl[1];
		gotoxy(50, 12);
		cout << setw(10) << left << spieler[1];
	default:
		gotoxy(33, 5);
		cout << "Spieler 1: " << setw(2) << (int)kartenanzahl[0];
		gotoxy(35, 10);
		cout << setw(10) << left << spieler[0];
	}

	gotoxy(0, 20);
	if (Nachricht & 0x80) {
		cout << "Spieler " << (Nachricht & 0x03) + 1 << " hat richtig geklingelt und erhaelt alle gespielten Karten." << endl;
	}
	if (Nachricht & 0x40) {
		cout << "Spieler " << ((Nachricht & 0x0C) / 4) + 1 << " hat falsch geklingelt und gibt allen anderen Spielern eine Karte." << endl;
	}
	if (Nachricht & 0x20) {
		cout << "Spieler " << ((Nachricht & 0x0C) / 4) + 1 << " hat keine Karten mehr und ist ausgeschieden" << endl;
	}
	if (Nachricht & 0x10) {
		cout << "Spieler " << (Nachricht & 0x03) + 1 << " hat gewonnen";
		// ToDo allgemeine Tastensperre?
	}
	else {
		cout << "Spieler " << (Nachricht & 0x03) + 1 << " ist an der Reihe." << endl;
	}
} 

// Hilfsfunktion zur Konsolenausgabe
void gotoxy(int x, int y)
{
	//Standard-Consolen-Fenster unter Windows werden mit 80 Zeichen Breite und 25 Zeilen H�he gestartet
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}