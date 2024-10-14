//arduino的GND接串口屏或串口工具的GND,共地
 //arduino的9接串口屏或串口工具的RX
 //arduino的8接串口屏或串口工具的TX
 //arduino的5V接串口屏的5V,如果是串口工具,不用接5V也可以
 //根据自己的实际修改对应的TX_Pin和RX_Pin
#include <espnow.h>
#include <ESP8266WiFi.h>
#define TJC Serial
#define FRAME_LENGTH 7

uint8_t broadcastAddress1[] = {0x80, 0x7D, 0x3A, 0x2E, 0x41, 0x4D};
uint8_t broadcastAddress2[] = {0xA4, 0xCF, 0x12, 0xC2, 0x59, 0x0E};
uint8_t broadcastAddress3[] = {0x5C, 0xCF, 0x7F, 0x1E, 0xA9, 0x4D};
uint8_t broadcastAddress4[] = {0x50, 0x02, 0x91, 0xFC, 0x0F, 0xF4};
uint8_t broadcastAddress5[] = {0xBC, 0xDD, 0xC2, 0x65, 0x28, 0xF1};

typedef struct struct_message {
  int CardType1;      //卡片种类
  int CardType2;
  unsigned char delaytime;     //延迟时间
  unsigned char AudioTrack;    //音轨
} struct_message;

struct_message myData;
int picid;
int EraseMode = -1;
int EraseType1;
int EraseType2;
int guide = 0;
int HaveImage[50];

void SendEnd(){
  TJC.write(0xff);
  TJC.write(0xff);
  TJC.write(0xff);  
}

int UCharToInt(unsigned char buffer){
  if(buffer == 0x00) return 0;
  if(buffer == 0x01) return 1;
  if(buffer == 0x02) return 2;
  if(buffer == 0x03) return 3;
  if(buffer == 0x04) return 4;
  if(buffer == 0x05) return 5;
  if(buffer == 0x06) return 6;
  if(buffer == 0x07) return 7;
  if(buffer == 0x08) return 8;
  if(buffer == 0x09) return 9;
  if(buffer == 0x10) return 10;
  if(buffer == 0x11) return 11;
  if(buffer == 0x12) return 12;
  if(buffer == 0x13) return 13;
  if(buffer == 0x14) return 14;
  if(buffer == 0x15) return 15;
  if(buffer == 0x16) return 16;
  if(buffer == 0x17) return 17;
  if(buffer == 0x18) return 18;
  if(buffer == 0x19) return 19;
  if(buffer == 0x20) return 20;
  if(buffer == 0x21) return 21;
  if(buffer == 0x22) return 22;
  if(buffer == 0x23) return 23;
  if(buffer == 0x24) return 24;
  if(buffer == 0x25) return 25;
  if(buffer == 0x26) return 26;
  if(buffer == 0x27) return 27;
  if(buffer == 0x28) return 28;
  if(buffer == 0x29) return 29;
  if(buffer == 0x30) return 30;
  if(buffer == 0x31) return 31;
  if(buffer == 0x32) return 32;
  if(buffer == 0x33) return 33;
  if(buffer == 0x34) return 34;
  if(buffer == 0x35) return 35;
  if(buffer == 0x36) return 36;
  if(buffer == 0x37) return 37;
  if(buffer == 0x38) return 38;
  if(buffer == 0x39) return 39;
  return -1;
}

void ChangeImage(int buffer,int id){
  char str[100];
  SendEnd();
  sprintf(str,"p%d.pic=%d",buffer,id);
  TJC.print(str);
  SendEnd();
}

void ChangeImageIf(int buffer,int id){
  if(HaveImage[buffer]==0){
    char str[100];
    SendEnd();
    sprintf(str,"p%d.pic=%d",buffer,id);
    TJC.print(str);
    SendEnd();
  }
}

void ChangeClearImage(int erase){
  if(erase == 1){
    ChangeImage(43,86);
  }
  if(erase == -1){
    ChangeImage(43,87);
  }
  //在插入更多图片后记得修改这里的参数
}

void ChangeGuideImage(int guide){
  if(guide == 0){
    ChangeImage(41,81);
  }
  if(guide == 1){
    ChangeImage(41,82);
  }
  if(guide == 2){
    ChangeImage(41,83);
  }
  if(guide == 3){
    ChangeImage(41,84);
  }
}

void ClearImage(){
  int i;
  for(i = 0;i<=39;i++){
    ChangeImage(i,0);
  }
}

void Guide1(){
  if(myData.CardType1%5 == 2){
    ChangeImageIf(5,91);
    ChangeImageIf(10,91);
    ChangeImageIf(15,91);
    ChangeImageIf(20,91);
    ChangeImageIf(25,91);
    ChangeImageIf(30,91);
    ChangeImageIf(21,0);
    ChangeImageIf(26,0);
    ChangeImageIf(31,0);
    ChangeImageIf(36,0);
    ChangeImageIf(2,0);
    ChangeImageIf(7,0);
    ChangeImageIf(12,0);
    ChangeImageIf(17,0);
    ChangeImageIf(22,0);
    ChangeImageIf(27,0);
    ChangeImageIf(13,0);
    ChangeImageIf(18,0);
    ChangeImageIf(23,0);
    ChangeImageIf(24,0);
    ChangeImageIf(29,0);
    ChangeImageIf(34,0);
  }
  else if(myData.CardType2>=3){
    ChangeImageIf(5,0);
    ChangeImageIf(10,0);
    ChangeImageIf(15,0);
    ChangeImageIf(20,0);
    ChangeImageIf(25,0);
    ChangeImageIf(30,0);
    ChangeImageIf(21,0);
    ChangeImageIf(26,0);
    ChangeImageIf(31,0);
    ChangeImageIf(36,0);
    ChangeImageIf(2,91);
    ChangeImageIf(7,91);
    ChangeImageIf(12,91);
    ChangeImageIf(17,0);
    ChangeImageIf(22,0);
    ChangeImageIf(27,0);
    ChangeImageIf(13,0);
    ChangeImageIf(18,0);
    ChangeImageIf(23,0);
    ChangeImageIf(24,91);
    ChangeImageIf(29,0);
    ChangeImageIf(34,0);
  }
  else if(myData.CardType2<=2){
    ChangeImageIf(5,0);
    ChangeImageIf(10,0);
    ChangeImageIf(15,0);
    ChangeImageIf(20,0);
    ChangeImageIf(25,0);
    ChangeImageIf(30,0);
    ChangeImageIf(21,91);
    ChangeImageIf(26,91);
    ChangeImageIf(31,91);
    ChangeImageIf(36,91);
    ChangeImageIf(2,0);
    ChangeImageIf(7,0);
    ChangeImageIf(12,0);
    ChangeImageIf(17,91);
    ChangeImageIf(22,91);
    ChangeImageIf(27,91);
    ChangeImageIf(13,91);
    ChangeImageIf(18,91);
    ChangeImageIf(23,91);
    ChangeImageIf(24,0);
    ChangeImageIf(29,91);
    ChangeImageIf(34,91);
  }
}

void Guide2(){
  if(myData.CardType1%5 == 2){
    ChangeImageIf(10,91);
    ChangeImageIf(15,91);
    ChangeImageIf(20,91);
    ChangeImageIf(25,91);
    ChangeImageIf(30,91);
    ChangeImageIf(16,0);
    ChangeImageIf(21,0);
    ChangeImageIf(26,0);
    ChangeImageIf(17,0);
    ChangeImageIf(22,0);
    ChangeImageIf(27,0);
    ChangeImageIf(32,0);
    ChangeImageIf(37,0);
    ChangeImageIf(8,0);
    ChangeImageIf(13,0);
    ChangeImageIf(18,0);
    ChangeImageIf(23,0);
    ChangeImageIf(4,0);
    ChangeImageIf(9,0);
    ChangeImageIf(14,0);
    ChangeImageIf(19,0);
    ChangeImageIf(24,0);
    ChangeImageIf(29,0);
    ChangeImageIf(34,0);    
  }
  else if(myData.CardType2>=3){
    ChangeImageIf(10,0);
    ChangeImageIf(15,0);
    ChangeImageIf(20,0);
    ChangeImageIf(25,0);
    ChangeImageIf(30,0);
    ChangeImageIf(16,0);
    ChangeImageIf(21,0);
    ChangeImageIf(26,0);
    ChangeImageIf(17,0);
    ChangeImageIf(22,0);
    ChangeImageIf(27,0);
    ChangeImageIf(32,0);
    ChangeImageIf(37,0);
    ChangeImageIf(8,0);
    ChangeImageIf(13,0);
    ChangeImageIf(18,0);
    ChangeImageIf(23,0);
    ChangeImageIf(4,0);
    ChangeImageIf(9,0);
    ChangeImageIf(14,0);
    ChangeImageIf(19,91);
    ChangeImageIf(24,91);
    ChangeImageIf(29,91);
    ChangeImageIf(34,91);
  }
  else if(myData.CardType2<=2){
    ChangeImageIf(10,0);
    ChangeImageIf(15,0);
    ChangeImageIf(20,0);
    ChangeImageIf(25,0);
    ChangeImageIf(30,0);
    ChangeImageIf(16,91);
    ChangeImageIf(21,91);
    ChangeImageIf(26,91);
    ChangeImageIf(17,91);
    ChangeImageIf(22,91);
    ChangeImageIf(27,91);
    ChangeImageIf(32,91);
    ChangeImageIf(37,91);
    ChangeImageIf(8,91);
    ChangeImageIf(13,91);
    ChangeImageIf(18,91);
    ChangeImageIf(23,91);
    ChangeImageIf(4,91);
    ChangeImageIf(9,91);
    ChangeImageIf(14,91);
    ChangeImageIf(19,0);
    ChangeImageIf(24,0);
    ChangeImageIf(29,0);
    ChangeImageIf(34,0);
  }  
}

void Guide3(){
  if(myData.CardType1%5 == 2){
    ChangeImageIf(5,91);
    ChangeImageIf(10,91);
    ChangeImageIf(15,91);
    ChangeImageIf(20,91);
    ChangeImageIf(25,91);
    ChangeImageIf(30,91);
    ChangeImageIf(35,91);
    ChangeImageIf(6,0);
    ChangeImageIf(11,0);
    ChangeImageIf(16,0);
    ChangeImageIf(21,0);
    ChangeImageIf(26,0);
    ChangeImageIf(31,0);
    ChangeImageIf(36,0);
    ChangeImageIf(12,0);
    ChangeImageIf(17,0);
    ChangeImageIf(22,0);
    ChangeImageIf(27,0);
    ChangeImageIf(32,0);
    ChangeImageIf(23,0);
    ChangeImageIf(28,0);
    ChangeImageIf(33,0);
    ChangeImageIf(38,0);
    ChangeImageIf(4,0);
    ChangeImageIf(29,0);
  }
  else if(myData.CardType2>=3){
    ChangeImageIf(5,0);
    ChangeImageIf(10,0);
    ChangeImageIf(15,0);
    ChangeImageIf(20,0);
    ChangeImageIf(25,0);
    ChangeImageIf(30,0);
    ChangeImageIf(35,0);
    ChangeImageIf(6,0);
    ChangeImageIf(11,0);
    ChangeImageIf(16,0);
    ChangeImageIf(21,0);
    ChangeImageIf(26,0);
    ChangeImageIf(31,91);
    ChangeImageIf(36,91);
    ChangeImageIf(12,0);
    ChangeImageIf(17,0);
    ChangeImageIf(22,91);
    ChangeImageIf(27,91);
    ChangeImageIf(32,0);
    ChangeImageIf(23,0);
    ChangeImageIf(28,0);
    ChangeImageIf(33,91);
    ChangeImageIf(38,91);
    ChangeImageIf(4,0);
    ChangeImageIf(29,0);
  }
  else if(myData.CardType2<=2){
    ChangeImageIf(5,0);
    ChangeImageIf(10,0);
    ChangeImageIf(15,0);
    ChangeImageIf(20,0);
    ChangeImageIf(25,0);
    ChangeImageIf(30,0);
    ChangeImageIf(35,0);
    ChangeImageIf(6,91);
    ChangeImageIf(11,91);
    ChangeImageIf(16,91);
    ChangeImageIf(21,91);
    ChangeImageIf(26,91);
    ChangeImageIf(31,0);
    ChangeImageIf(36,0);
    ChangeImageIf(12,91);
    ChangeImageIf(17,91);
    ChangeImageIf(22,0);
    ChangeImageIf(27,0);
    ChangeImageIf(32,91);
    ChangeImageIf(23,91);
    ChangeImageIf(28,91);
    ChangeImageIf(33,0);
    ChangeImageIf(38,0);
    ChangeImageIf(4,91);
    ChangeImageIf(29,91);
  }  
}

int TypeToImage(int t1,int t2){
  return (t1-1)*4+t2;
}

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
  char str[100];
  SendEnd();

  sprintf(str,"p40.pic=%d",TypeToImage(myData.CardType1,myData.CardType2));
  TJC.print(str);
  SendEnd();

  picid = TypeToImage(myData.CardType1,myData.CardType2);
  if(guide == 1){
    Guide1();
  }
  if(guide == 2){
    Guide2();
  }
  if(guide ==3){
    Guide3();
  }
}

void setup() {
   // put your setup code here, to run once:
   //初始化串口
   TJC.begin(9600);

   //因为串口屏开机会发送88 ff ff ff,所以要清空串口缓冲区
   while (TJC.read() >= 0); //清空串口缓冲区
   TJC.print("page main\xff\xff\xff"); //发送命令让屏幕跳转到main页面
   SendEnd();
   
   WiFi.mode(WIFI_STA);

    if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(OnDataRecv);
  
    esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_COMBO, 2, NULL, 0);
    esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_COMBO, 3, NULL, 0);
    esp_now_add_peer(broadcastAddress4, ESP_NOW_ROLE_COMBO, 4, NULL, 0);
    esp_now_add_peer(broadcastAddress5, ESP_NOW_ROLE_COMBO, 5, NULL, 0);
 }

 void loop() {
  unsigned char Datadelaytime;
  unsigned char Dataaudiotrack;
   //串口数据格式：
   //串口数据帧长度：7字节
   //帧头     参数1    参数2   参数3       帧尾
   //0x55     1字节   1字节    1字节     0xffffff
   //当参数是01时
   //帧头     参数1    参数2   参数3       帧尾
   //0x55     01     led编号  led状态    0xffffff
   //例子1：上位机代码  printh 55 01 01 00 ff ff ff  含义：1号led关闭
   //例子2：上位机代码  printh 55 01 04 01 ff ff ff  含义：4号led打开
   //例子3：上位机代码  printh 55 01 00 01 ff ff ff  含义：0号led打开
   //例子4：上位机代码  printh 55 01 04 00 ff ff ff  含义：4号led关闭

   //当参数是02或03时
   //帧头     参数1    参数2   参数3       帧尾
   //0x55     02/03   滑动值    00    0xffffff
   //例子1：上位机代码  printh 55 02 64 00 ff ff ff  含义：h0.val=100
   //例子2：上位机代码  printh 55 02 00 00 ff ff ff  含义：h0.val=0
   //例子3：上位机代码  printh 55 03 64 00 ff ff ff  含义：h1.val=100
   //例子4：上位机代码  printh 55 03 00 00 ff ff ff  含义：h1.val=0


   //当串口缓冲区大于等于7时
   while (TJC.available() >= FRAME_LENGTH) {
     unsigned char ubuffer[FRAME_LENGTH];
     //从串口缓冲读取1个字节但不删除
     unsigned char frame_header = TJC.peek();
     //当获取的数据是包头(0x55)时
     if (frame_header == 0x55) {
       //从串口缓冲区读取8字节
       TJC.readBytes(ubuffer, FRAME_LENGTH);
       if (ubuffer[4] == 0xff && ubuffer[5] == 0xff && ubuffer[6] == 0xff) {
          TJC.print(ubuffer[1]);
          Dataaudiotrack = ubuffer[1];
          TJC.print(" ");
          TJC.println(ubuffer[2]);
          Datadelaytime = ubuffer[2];
          //myData.CardType1 = 0; //错误值 告诉从机不要覆盖
          //myData.CardType2 = 0;
          myData.delaytime = Datadelaytime;
          myData.AudioTrack = Dataaudiotrack;
          if(ubuffer[3]==0x41){
            guide ++;
            if(guide == 4){
              guide = 0;
            }
            ChangeGuideImage(guide);
          }
          if(ubuffer[3]==0x42){
            ClearImage();
            int a;
            for(a = 0;a<=49;a++){
              HaveImage[a] = 0;
            }
          }
          if(EraseMode == 1){
            picid = 0;
            myData.CardType1 = 0;
            myData.CardType2 = 0;
          }
          if(ubuffer[3]<=0x39){
            ChangeImage(UCharToInt(ubuffer[3]),picid);
            if(picid!=0){
              HaveImage[UCharToInt(ubuffer[3])]=1;
            }
            else{
              HaveImage[UCharToInt(ubuffer[3])]=0;
            }
          }
          if(ubuffer[3]==0x43){
            if(EraseMode == -1){
              EraseType1 = myData.CardType1;
              EraseType2 = myData.CardType2;
            }
            if(EraseMode == 1){
              myData.CardType1 = EraseType1;
              myData.CardType2 = EraseType2;
              picid = TypeToImage(myData.CardType1,myData.CardType2);
            }
            EraseMode = -EraseMode;
            ChangeClearImage(EraseMode);
          }
          if((ubuffer[3]>=0x00 && ubuffer[3]<=0x39)||ubuffer[3]==0x42||ubuffer[3]==0x44||ubuffer[3]==0x45){
            esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
            esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));
            esp_now_send(broadcastAddress3, (uint8_t *) &myData, sizeof(myData));
            esp_now_send(broadcastAddress4, (uint8_t *) &myData, sizeof(myData));
            esp_now_send(broadcastAddress5, (uint8_t *) &myData, sizeof(myData));
          }

       }
     } else {
       TJC.read();  //从串口缓冲读取1个字节并删除
     }
   }
 }