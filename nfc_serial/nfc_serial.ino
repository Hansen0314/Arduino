
#include "PN532_HSU.h"
#include "PN532.h"
#include "NfcAdapter.h"

PN532_HSU interface(Serial1);
NfcAdapter nfc = NfcAdapter(interface);

void setup(void) {
    Serial.begin(115200);
    Serial.println("NDEF Reader");
    nfc.begin();
}

void loop(void) {
    Serial.println("\nScan a NFC tag\n");
    if (nfc.tagPresent())
    {
        NfcTag tag = nfc.read();
        tag.print();
    }
    delay(5000);
}
