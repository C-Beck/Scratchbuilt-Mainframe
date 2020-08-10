//char data[]={0x00,0x1B,0x40,0x1B,0x52,0x00,0x1B,0x74,0x01,0x1B,0x36,0x12,0x1B,0x50,'T','h','i','s',' ','i','s',' ','a',' ','T','E','S','T',' ','o','f',' ','t','h','e',
//' ','E','M','E','R','G','E','N','C','Y',' ','A','L','E','R','T',' ','S','Y','S','T','E','M','.',0x0D,'T','h','i','s',' ','i','s',' ','o','n','l','y',' ','a',' ','T','E','S','T','.',0x0D,0x0C};
char header[] = {0x00, 0x1B, 0x40, 0x1B, 0x52, 0x00, 0x1B,
                 0x74, 0x01, 0x1B, 0x36, 0x12, 0x1B, 0x50
                };
char kanaHeader[] = {0x1B, 0x2A, 0x00, 0x0A, 0x00};
char hiragana[][8] = {{0x00, 0x5C, 0x52, 0x58, 0x58, 0xFE, 0x5A, 0x44},  //A あ
  {0x00, 0x00, 0x0C, 0x10, 0x00, 0x04, 0x02, 0x1C},//I い
  {0x00, 0x00, 0x0C, 0x52, 0x52, 0x50, 0x08, 0x00},//U う
  {0x00, 0x02, 0x2E, 0xB8, 0xA8, 0xA4, 0x22, 0x00},//E え
  {0x00, 0x4C, 0x92, 0x10, 0x50, 0xFC, 0x4A, 0x44},//O お
  {0x00, 0x38, 0x40, 0x1C, 0x22, 0xF0, 0x2C, 0x22},//KA か
};
char katakana[][10] = {{0x81, 0x82, 0x84, 0xB8, 0x80, 0xA0, 0xC0, 0x80, 0x00, 0x00}};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(2400);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  digitalWrite(10, HIGH);
  for (int i = 0; i < sizeof(header); i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (((header[i] >> j) & 1) == 1)
      {
        digitalWrite(j + 2, HIGH);
      }
      else
      {
        digitalWrite(j + 2, LOW);
      }
    }
    delayMicroseconds(10);
    digitalWrite(10, LOW);
    delayMicroseconds(10);
    digitalWrite(10, HIGH);
    while (digitalRead(11) == HIGH) {};
  }
  Serial.println("Ready...");
}

int kanaCount = 0;
int kana = 0;
char buf[4];
int index = 0;
unsigned long guard = NULL;
void serialEvent()
{
  char data = Serial.read();
  Serial.print(data);
  if (data == 0x0d) {
    Serial.println();
  };
  if (data == 0x2B && !guard) {
    kanaCount++;
  }
  if (data == 0x2B && guard) {
    guard = NULL;
    for (int i = 0; i < kanaCount + 1; i++) {
      sendData(0x2b);
    }
    kanaCount = 0;
  }
  if (data != 0x2B) {
    if (guard) {
      for (int i = 0; i < kanaCount; i++) {
        sendData(0x2b);
      }
    }
    kanaCount = 0;
    guard = NULL;
  };
  if (kanaCount >= 3) {
    guard = millis();

  }
  if ((kana == 0 || (data & ~0x1f) == 0 || data == 0x20 || (data >> 4) == 3) && (data != 0x2B)) {
    sendData(data);
  }
  else if (kana == 1) {
    buf[index] = data;
    if (buf[index] == 'a' || buf[index] == 'i' || buf[index] == 'u' || buf[index] == 'e' || buf[index] == 'o' || (buf[index] == 'n' && buf[index - 1] == 'n')) {
      switch (buf[0]) {
        case 'a':
          for (int i = 0; i < 5; i++) {
            sendData(kanaHeader[i]);
          }
          for (int i = 0; i < 8; i++) {
            sendData(hiragana[0][i]);
          }
          for (int i = 0; i < 2; i++) {
            sendData(0x00);
          }
          break;
        case 'i':
          for (int i = 0; i < 5; i++) {
            sendData(kanaHeader[i]);
          }
          for (int i = 0; i < 8; i++) {
            sendData(hiragana[1][i]);
          }
          for (int i = 0; i < 2; i++) {
            sendData(0x00);
          }
          break;
        case 'u':
          for (int i = 0; i < 5; i++) {
            sendData(kanaHeader[i]);
          }
          for (int i = 0; i < 8; i++) {
            sendData(hiragana[2][i]);
          }
          for (int i = 0; i < 2; i++) {
            sendData(0x00);
          }
          break;
        case 'e':
          for (int i = 0; i < 5; i++) {
            sendData(kanaHeader[i]);
          }
          for (int i = 0; i < 8; i++) {
            sendData(hiragana[3][i]);
          }
          for (int i = 0; i < 2; i++) {
            sendData(0x00);
          }
          break;
        case 'k':
          switch (buf[1]) {
            case 'a':
              for (int i = 0; i < 5; i++) {
                sendData(kanaHeader[i]);
              }
              for (int i = 0; i < 8; i++) {
                sendData(hiragana[5][i]);
              }
              for (int i = 0; i < 2; i++) {
                sendData(0x00);
              }
              break;
          }
          break;
      }
      for (int i = 0; i < index + 1; i++) {
        buf[i] = 0;
      }
      index = 0;
    } else {
      index++;
      if (index >= 4) {
        for (int i = 0; i < index; i++) {
          sendData(buf[i]);
          buf[i] = 0;
        }
        index = 0;
      }
    }

    //    for (int i = 0; i < 5; i++) {
    //      sendData(kanaHeader[i]);
    //    }
    //    for (int i = 0; i < 8; i++) {
    //      sendData(hiragana[0][i]);
    //    }
    //    for (int i = 0; i < 2; i++) {
    //      sendData(0x00);
    //    }
  }
}

void sendData(char data)
{
  for (int j = 0; j < 8; j++)
  {
    if (((data >> j) & 1) == 1)
    {
      digitalWrite(j + 2, HIGH);
    }
    else
    {
      digitalWrite(j + 2, LOW);
    }
  }
  delayMicroseconds(10);
  digitalWrite(10, LOW);
  delayMicroseconds(10);
  digitalWrite(10, HIGH);
  while (digitalRead(11) == HIGH) {};
}

void loop() {
  // put your main code here, to run repeatedly:
  if (guard && millis() - guard > 1000) {
    kana = (kana == 1) ? 0 : 1;
    Serial.print("Kana set to ");
    Serial.println(kana);
    kanaCount = 0;
    guard = NULL;
  }
}
