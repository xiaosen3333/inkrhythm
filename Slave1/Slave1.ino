#include <DFPlayer_Mini_Mp3.h>
#include <espnow.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
//这是从机1的代码

typedef struct struct_message {
  int CardType1;
  int CardType2;
  unsigned char delaytime;
  unsigned char AudioTrack;
} struct_message;

// Create a struct_message called myData
struct_message myData;

int MusicType1[10];
int MusicType2[10];
int TempMusicType1;
int TempMusicType2;
unsigned char MusicDelayTime;
bool PlayButton;
bool ClearButton;
bool StopButton;

// callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("CardType1: ");
  Serial.println(myData.CardType1);
  Serial.print("CardType2: ");
  Serial.println(myData.CardType2);
  Serial.print("DelayTime: ");
  Serial.println(myData.delaytime);
  Serial.print("AudioTrack: ");
  Serial.println(myData.AudioTrack);
  Serial.println();
  TempMusicType1 = myData.CardType1;
  TempMusicType2 = myData.CardType2;
  if(myData.AudioTrack == 0x01){
    MusicDelayTime = myData.delaytime;
    MusicType1[MusicDelayTime] = TempMusicType1;
    MusicType2[MusicDelayTime] = TempMusicType2;
  }
  if(myData.AudioTrack == 0x09){//测试用例 之后修改  
    PlayButton = true;
  }
  if(myData.AudioTrack == 0x08){
    ClearButton = true;
  }
  if(myData.AudioTrack == 0x07){
    StopButton = true;
  }
  Serial.print("MusicType1: ");
  Serial.println(TempMusicType1);
  Serial.print("MusicType2: ");
  Serial.println(TempMusicType2);
  Serial.print("MusicDelayTime ");
  Serial.println(MusicDelayTime);
  int dt;
  for(dt = 1;dt<=8;dt++){
    Serial.print(MusicType1[dt]);
    Serial.print(" ");
  }
  Serial.println();
  for(dt = 1;dt<=8;dt++){
    Serial.print(MusicType2[dt]);
    Serial.print(" ");
  }
  Serial.println();
}

void MusicPlay(int type1,int type2){
  int type;
  type = type1*100 + type2;
  mp3_play(type);
  Serial.println(type);
}

/*void FullMusic(){
  int time;
  for(time = 1;time<=7;time++){
    MusicPlay(MusicType1[time],MusicType2[time]);
    delay(9000);
  }
}*/

void setup () {
	Serial.begin (9600);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

    //设置ESP8266角色：
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  //先前创建的功能 测试ESP-NOW通信
  esp_now_register_recv_cb(OnDataRecv);
	mp3_set_serial (Serial);	//set Serial for DFPlayer-mini mp3 module 
	delay(1);  //wait 1ms for mp3 module to set volume
	mp3_set_volume (20);
  PlayButton = 0;
}


//
void loop () {
  if(PlayButton){
    int time;
    for(time = 1;time<=8;time++){
      if(MusicType1[time]!=0){
        MusicPlay(MusicType1[time],MusicType2[time]);
      }
      if(StopButton){
        mp3_stop();
        StopButton = false;
        break;
      }
      delay(8766);
    }
    PlayButton = false;
  }
  if(ClearButton){
    int time;
    for(time = 1;time <= 8;time++){
      MusicType1[time] = 0;
      MusicType2[time] = 0;
    }
    ClearButton = false;
  }
}

/*
   mp3_play ();		//start play
   mp3_play (5);	//play "mp3/0005.mp3"
   mp3_next ();		//play next 
   mp3_prev ();		//play previous
   mp3_set_volume (uint16_t volume);	//0~30
   mp3_set_EQ ();	//0~5
   mp3_pause ();
   mp3_stop ();
   void mp3_get_state (); 	//send get state command
   void mp3_get_volume (); 
   void mp3_get_u_sum (); 
   void mp3_get_tf_sum (); 
   void mp3_get_flash_sum (); 
   void mp3_get_tf_current (); 
   void mp3_get_u_current (); 
   void mp3_get_flash_current (); 
   void mp3_single_loop (boolean state);	//set single loop 
   void mp3_DAC (boolean state); 
   void mp3_random_play (); 
 */
