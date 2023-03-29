#include <Wire.h>   
#include <SPI.h>
#include <SD.h>
#include <mpu9250.h>
const int chipSelect = 53;
String filename;
char readdata="a";
int filenum=1;
float vrw[3],tvrw[3];
float vrfa[3],tvrfa[3];
float vrb[3],tvrb[3];
float vrs[3],tvrs[3];
int count=0;
int delt_t=50;
mpu9250 rw,rfa,rb,rs;
void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (0){//!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

rw.ini(0x69,1);
rfa.ini(0x68,1);
rb.ini(0x69,2);
rs.ini(0x68,2);
/*
 testsub1.ini(0x68,1);
 testsub2.ini(0x68,2);*/
 filename="MOCAP"+String(filenum,DEC)+".bhv";
 Serial.print(SD.exists(filename));
while(SD.exists(filename)){
  filenum=filenum+1;
  filename="MOCAP"+String(filenum,DEC)+".bhv";
}
Serial.println("ENTER r to start reading data into the SDCARD");
}

void loop() {
  String dataString;
  quatvalget(vrw,vrfa,vrb,vrs);
  readdata=Serial.read();  
  while(readdata=='r'){
  quatvalget(tvrw,tvrfa,tvrb,tvrs);  
  //  RunTPose(dataFile);
  File dataFile = SD.open(filename, FILE_WRITE);
  while(Serial.available() == 0) 
  {     quatvalget(vrw,vrfa,vrb,vrs);
        tposaltered(vrw,vrfa,vrb,vrs,tvrw,tvrfa,tvrb,tvrs);
        delt_t =  millis()-count;
        if(delt_t>100) 
        {
          dataString=datatostring(vrw,vrfa,vrb,vrs);
          if (dataFile) 
          {
            dataFile.println(dataString);
            Serial.println(dataString);
           }
            count=millis();
         }
   }
if(dataFile){
        dataFile.close();
             }
  if(Serial.available()){readdata=Serial.read();}
   
  }
  while(readdata=='e')
  {
  Serial.println("DATA RECORDED SUCCEFULLY");  
  while(1){}
    }
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------
                                                     FUNCTIONS
--------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------*/
float subtarction(float x,float t){
  float fv;
  if (x-t>180){fv=x-t-360;
  }
  else if(x-t<-180){
    fv=x-t+360;
    }
  else {
    fv=x-t;
    }
  return fv;
  
  
  
  }


void tposaltered(float ar1[3],float ar2[3],float ar3[3],float ar4[3],float tar1[3],float tar2[3],float tar3[3],float tar4[3]){
  for(int i=0;i<3;i++){
  ar1[i]=subtarction(ar1[i],tar1[i]);
  ar2[i]=subtarction(ar2[i],tar2[i]);
  ar3[i]=subtarction(ar3[i],tar3[i]);
  ar4[i]=subtarction(ar4[i],tar4[i]);
  }
  }
void quatvalget(float ar1[3],float ar2[3],float ar3[3],float ar4[3]){
 rw.getvalues(ar1);
rfa.getvalues(ar2);
rb.getvalues(ar3);
rs.getvalues(ar4); 
  }
String datatostring(float ar1[3],float ar2[3],float ar3[3],float ar4[3]){
String dataString = "0.000000 0.000000 0.000000 ";
dataString += String(ar4[0], 6);dataString += " ";dataString += String(ar4[2], 6);dataString += " ";dataString += String(ar4[1], 6);
dataString += " ";
dataString += String(ar3[0], 6);dataString += " ";dataString += String(ar3[2], 6);dataString += " ";dataString += String(ar3[1], 6);
dataString += " ";
dataString += String(ar2[0], 6);dataString += " ";dataString += String(ar2[2], 6);dataString += " ";dataString += String(ar2[1], 6);
dataString += " ";
dataString += String(ar1[0], 6);dataString += " ";dataString += String(ar1[2], 6);dataString += " ";dataString += String(ar1[1], 6); 
return dataString;  
  }
