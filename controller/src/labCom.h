#ifndef labCom_h
#define labCom_h

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "config.h"
#include "mfc_main.h"

namespace communication {
    // an die MFCs werden absolutwerte uerbtragen. Diese basieren auf der Zeit, die gespeichert
    // wird, wenn diese Klasse erstellt wird
    class LabCom : public Thread {
    public:
        //Defaultconstructor
        LabCom();
        //Destructor
        ~LabCom();
        //Gebe Adresse des Hauptobjektes an LabCom, um zu kommunizieren
        void setMainMfcObjectPointer(control::MfcMain *mfcMain);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        //Diese Funktion wird zu Programmstart mehrmals ausgefuehrt, bis die kompletten Daten
        //eingetroffen sind. Sie liefert bei erfolgreicher Ausfuehrung 1, ansonsten
        //einen Errorcode mit einer Fehlermeldung in der Konsole
        int readLine();
        //Eingetroffene und vollstaendige Zeile wird an Kommata zerlegt und in Array gespeichert.
        //Gibt nach vollstaendiger Durchfuehrung die Anzahl an Eintraegen im Array zurueck.
        int splitLine();
        //gibt Befehl zum Start der Threads von MFC und Ventil, uebergibt die Startzeit, welche als
        //Nullpunkt dient
        void start(unsigned long startTime);

        control::MfcMain *mfcMain;

        bool reading;
        bool sending;

        char inDataBuffer[SERIAL_READ_MAX_LINE_SIZE];
        int bufferCharIndex;

        char inDataArray [SERIAL_READ_MAX_BLOCK_AMOUNT][SERIAL_READ_MAX_BLOCK_SIZE];
        int headerLineCounter;

        //Header-Varablen:
        int amount_MFC;
        int amount_valve;
    };
}

#endif
