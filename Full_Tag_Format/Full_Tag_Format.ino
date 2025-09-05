/**
 * Formats a Mifare Classic tags as an NDEF tag
 * This will fail if the tag is already formatted NDEF
 * nfc.clean will turn a NDEF formatted Mifare Classic tag back to the Mifare Classic format
 * 
 * Clean resets a tag back to factory-like state
 * For Mifare Classic, tag is zero'd and reformatted as Mifare Classic
 * For Mifare Utralight, tags is zero'd and left empty
 * 
 * Erases a NFC tag by writing an empty NDEF message
 * 
 * Read the tag informations
*/
#include <CLPIoT.h>

#if 0
  #include <SPI.h>
  #include <PN532_SPI.h>
  #include <PN532.h>
  #include <NfcAdapter.h>

  PN532_SPI pn532spi(SPI, 10);
  NfcAdapter nfc = NfcAdapter(pn532spi);
#elif 1
  #define NDEF_DEBUG

  #include <PN532_HSU.h>
  #include <PN532.h>
  #include <NfcAdapter.h>

  PN532_HSU pn532hsu(16, 17);
  NfcAdapter nfc(pn532hsu);
#else
  #include <Wire.h>
  #include <PN532_I2C.h>
  #include <PN532.h>
  #include <NfcAdapter.h>

  PN532_I2C pn532_i2c(Wire);
  NfcAdapter nfc = NfcAdapter(pn532_i2c);
#endif

void setup(void) {
  Serial.begin(115200);
  clpiot.begin();
}

void loop(void) {
  clpiot.print(0, "Preparacao TAG", "center");

  // Preparing the tag information
  eraseRFID();
  cleanRFID();
  formatRFID();

  delay(5000);
}

void eraseRFID() {
  printf("Entrou");
  delay(1000);
  nfc.begin();

  clpiot.print(1, "Insira a TAG...", "left");

  // Identifier if tag is present
  while (!nfc.tagPresent()) delay(500);

  // Erase the tag returning the success of erasing
  clpiot.print(1, "Restaurando", "center");
  bool success = nfc.erase();
  delay(500);

  // Return the message of erasing
  if (success) clpiot.print(1, "Sucesso", "center");
  else clpiot.print(1, "Ja restaurada", "center");
}

void cleanRFID() {
  nfc.begin();

  // Identifier if tag is present
  while (!nfc.tagPresent()) delay(500);

  // Erase the tag returning the success of erasing
  clpiot.print(1, "Limpando", "center");
  bool success = nfc.clean();

  if (success) clpiot.print(1, "Sucesso", "center");
  else clpiot.print(1, "Ja limpa", "center");
}

void formatRFID() {
  nfc.begin();

  // Identifier if tag is present
  while (!nfc.tagPresent()) delay(500);

  // Erase the tag returning the success of erasing
  clpiot.print(1, "Formatando", "center");
  bool success = nfc.format();

  if (success) clpiot.print(1, "Sucesso", "center");
  else clpiot.print(1, "Ja formatada", "center");
}