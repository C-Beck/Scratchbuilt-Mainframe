//char data[]={0x00,0x1B,0x40,0x1B,0x52,0x00,0x1B,0x74,0x01,0x1B,0x36,0x12,0x1B,0x50,'T','h','i','s',' ','i','s',' ','a',' ','T','E','S','T',' ','o','f',' ','t','h','e',
//' ','E','M','E','R','G','E','N','C','Y',' ','A','L','E','R','T',' ','S','Y','S','T','E','M','.',0x0D,'T','h','i','s',' ','i','s',' ','o','n','l','y',' ','a',' ','T','E','S','T','.',0x0D,0x0C};
char header[]={0x00,0x1B,0x40,0x1B,0x52,0x00,0x1B,
               0x74,0x01,0x1B,0x36,0x12,0x1B,0x50};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(2400);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,INPUT);
  digitalWrite(10,HIGH);
  for(int i=0; i<sizeof(header);i++)
  {
    for(int j=0;j<8;j++)
    {
      if(((header[i]>>j)&1)==1)
      {
        digitalWrite(j+2,HIGH);
      }
      else
      {
        digitalWrite(j+2,LOW);
      }
    }
    delayMicroseconds(10);
    digitalWrite(10,LOW);
    delayMicroseconds(10);
    digitalWrite(10,HIGH);
    while(digitalRead(11)==HIGH){};
  }
  Serial.println("Ready...");
}
int feed = 0;
void serialEvent()
{
  char data=Serial.read();
  for(int j=0;j<8;j++)
    {
      if(((data>>j)&1)==1)
      {
        digitalWrite(j+2,HIGH);
      }
      else
      {
        digitalWrite(j+2,LOW);
      }
    }
    delayMicroseconds(10);
    digitalWrite(10,LOW);
    delayMicroseconds(10);
    digitalWrite(10,HIGH);
    Serial.print(data);
    if(data == 0x0d){feed++;Serial.println();};
    if(data != 0x0d){feed=0;};
    if(feed >= 3){
      feed=0;
      data = 0x0c;
      for(int j=0;j<8;j++)
      {
        if(((data>>j)&1)==1)
        {
          digitalWrite(j+2,HIGH);
        }
        else
        {
          digitalWrite(j+2,LOW);
        }
      }
      delayMicroseconds(10);
      digitalWrite(10,LOW);
      delayMicroseconds(10);
      digitalWrite(10,HIGH);
    }
    while(digitalRead(11)==HIGH){};
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
