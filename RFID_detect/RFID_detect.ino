#include <SPI.h>
#include <MFRC522.h>
#include <espnow.h>
#include <ESP8266WiFi.h>

#define RST_PIN         5           // Configurable, see typical pin layout above
#define SS_PIN          4          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

//uint8_t broadcastAddress1[] = {0xFC, 0xF5, 0xC4, 0x95, 0x84, 0x82};
//uint8_t broadcastAddress2[] = {0xA4, 0xCF, 0x12, 0xC2, 0x59, 0x0E};
uint8_t broadcastAddress3[] = {0x94, 0xB9, 0x7E, 0x1C, 0xE3, 0xB3};
//uint8_t broadcastAddress4[] = {0x5C, 0xCF, 0x7F, 0x1E, 0xA9, 0x4D};
int cardtype1 = 0;
int cardtype2 = 0;
int cardtype3 = -1;

String carid="";

typedef struct struct_message {
  int CardType1;      //乐器种类
  int CardType2;      //乐段种类
  unsigned char delaytime;     //延迟时间
  unsigned char AudioTrack;    //音轨
} struct_message;

//int flag = 0;   //压力是否触发

struct_message myData;

//esp_now_peer_info peerInfo;   //1

void onDataSent(uint8_t *mac, uint8_t sendStatus) {

  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print(macStr);

  if (sendStatus == 0) {
    Serial.println("Send success");
  } else {
    Serial.println("Send failed");
  }
}

/**
 * Initialize.
 */
void setup() {
    Serial.begin(9600); // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
    Serial.print(F("Using key (for A and B):"));
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();

    Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    esp_now_register_send_cb(onDataSent);
  
    //esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    //esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_COMBO, 2, NULL, 0);
    esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_COMBO, 3, NULL, 0);
    //esp_now_add_peer(broadcastAddress4, ESP_NOW_ROLE_COMBO, 4, NULL, 0);
    //esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

}

/**
 * Main loop.
 */
void loop() {
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();

    if (mfrc522.uid.uidByte[0] == 0x87 &&
        mfrc522.uid.uidByte[1] == 0x3C &&
        mfrc522.uid.uidByte[2] == 0x61 &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype1 = 1;
        }
    if (mfrc522.uid.uidByte[0] == 0x87 &&
        mfrc522.uid.uidByte[1] == 0x3A &&
        mfrc522.uid.uidByte[2] == 0xC2 &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype1 = 2;
        }
    if (mfrc522.uid.uidByte[0] == 0x77 &&
        mfrc522.uid.uidByte[1] == 0x44 &&
        mfrc522.uid.uidByte[2] == 0x0E &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype1 = 3;
        }
    if (mfrc522.uid.uidByte[0] == 0x77 &&
        mfrc522.uid.uidByte[1] == 0x8D &&
        mfrc522.uid.uidByte[2] == 0x05 &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype1 = 4;
        }
    if (mfrc522.uid.uidByte[0] == 0x87 &&
        mfrc522.uid.uidByte[1] == 0x4B &&
        mfrc522.uid.uidByte[2] == 0x0E &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype1 = 5;
        }

    if (mfrc522.uid.uidByte[0] == 0x77 &&
        mfrc522.uid.uidByte[1] == 0x95 &&
        mfrc522.uid.uidByte[2] == 0xA3 &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype3 = 5*0;
        }

    if (mfrc522.uid.uidByte[0] == 0x87 &&
        mfrc522.uid.uidByte[1] == 0x2E &&
        mfrc522.uid.uidByte[2] == 0x9E &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype3 = 5*1;
        }

    if (mfrc522.uid.uidByte[0] == 0x77 &&
        mfrc522.uid.uidByte[1] == 0xED &&
        mfrc522.uid.uidByte[2] == 0x89 &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype3 = 5*2;
        }

    if (mfrc522.uid.uidByte[0] == 0x87 &&
        mfrc522.uid.uidByte[1] == 0x35 &&
        mfrc522.uid.uidByte[2] == 0xEA &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype3 = 5*3;
        }

    if (mfrc522.uid.uidByte[0] == 0x77 &&
        mfrc522.uid.uidByte[1] == 0x9B &&
        mfrc522.uid.uidByte[2] == 0xA5 &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype2 = 1;
        }
    if (mfrc522.uid.uidByte[0] == 0x87 &&
        mfrc522.uid.uidByte[1] == 0x0F &&
        mfrc522.uid.uidByte[2] == 0xB2 &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype2 = 2;
        }
    if (mfrc522.uid.uidByte[0] == 0x87 &&
        mfrc522.uid.uidByte[1] == 0x35 &&
        mfrc522.uid.uidByte[2] == 0x53 &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype2 = 3;
        }
    if (mfrc522.uid.uidByte[0] == 0x87 &&
        mfrc522.uid.uidByte[1] == 0x72 &&
        mfrc522.uid.uidByte[2] == 0x3B &&
        mfrc522.uid.uidByte[3] == 0x50){
          cardtype2 = 4;
        }
    Serial.print("cardtype1:");
    Serial.println(cardtype1);
    Serial.print("cardtype2:");
    Serial.println(cardtype2);
    Serial.print("cardtype3:");
    Serial.println(cardtype3);
    if(cardtype3 != -1){
      myData.CardType1 = cardtype1 + cardtype3;
    }else{
      myData.CardType1 = cardtype1;
    }
    myData.CardType2 = cardtype2;
    myData.AudioTrack = 0x99;
    myData.delaytime = 0x99;
    //flag++;

    if(myData.CardType1!=0&&myData.CardType2!=0&&cardtype3!=-1){
      //esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
      //esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));
      esp_now_send(broadcastAddress3, (uint8_t *) &myData, sizeof(myData));
      //esp_now_send(broadcastAddress4, (uint8_t *) &myData, sizeof(myData));
      cardtype1 = 0;
      cardtype2 = 0;
      cardtype3 = -1;
    }

    //esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
    //esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));
    //esp_now_send(broadcastAddress3, (uint8_t *) &myData, sizeof(myData));
    //esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
