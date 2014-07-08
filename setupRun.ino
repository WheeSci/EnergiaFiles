//setupRun Tab 
// Energia Sketch for WheeStat5_4a
// work_on_WheeStat_outputs sketch

void setupRun() {
  gotLims = false;
  iMin = 0;
  iMax = 0;
  /////////////// begin reading parameters //////////
  boolean startread = false;
  while (startread == false) {
      if(Serial.available() > 0) {
      int starti = Serial.read();
      if(starti == 38) {
        startread = true;
        runState = true;
     }
    }
  } 
 /* hiFlag = 0;
  loFlag = 0;
  flag = 0;*/
 
  //////////// mode ////////////////  
 
  Serial.print(",,Mode, ");   //double comma to get header out of data columns
 // Serial.print(",");
  readParam();  
  mode=param;     
  delay(20);  

  ///////////initial voltage///////
  Serial.print("mV init, ");
//  Serial.print(",");
  readParam();  
  mVinit=param-2000;     // 2000 added in GUI to transfer positive value
  dInit = mVinit/10 + halfRes;       //digital value for pwm
  delay(20);  

  ///////////final voltage///////
  Serial.print("mV Final, ");
//  Serial.print(",");
  readParam();
  mVfnl=param-2000;       // 2000 added to value in GUI
  dFnl = mVfnl/10 + halfRes;       //digital value for pwm
  delay(20);

  ///////////scan rate///////
  Serial.print("mV/s, ");
 // Serial.print(",");
  readParam();
  scanRate = param;     
  stepTime = 10000*pwm_step/scanRate;      // ms per step
  delay(20);

  ///////////  Current gain  ///////
  int loGain0[19] = {
    128,127,126,125,124,123,122,120,118,116,112,108,103,96,88,77,64,48,27    };
  int hiGain1[12] = {
    26,19,15,12,9,7,5,4,3,2,1,0    };

  Serial.print("gain, ");
 // Serial.print(",");
  readParam();   
  dB = param;                        //dB = gain in decibels
  if (dB >= 19) {
    gain0 = 0;
    gain1 = hiGain1[dB-19];
  }
  else {
    gain1 = 26;
    gain0 = loGain0[dB];

  } 
  res4 = resPot*(128-gain0)/128+resW;            // Resistor 4 in schematic, resistance from 50 Kohm pot0
  res5 = resPot*(gain1)/128+resW;                // Resistor 5, from pot1
  amplif = res6*res4/res5;                         // amplification factor
 

  delay(20);

  ///////////offset voltage///////
  Serial.print("offset, ");
//  Serial.print(",");
  readParam();  
  dOff1 = param;
  dOff= dOff1 + offAdj; // value from GUI + 165 (half of 330 digital resolution) 
                       // addition of adjustment here assumes that analog write is source of error
                       // and that error in read is not the problem  
  delay(20);

  /////////// delay 1 ////////////////
  Serial.print("initial delay, ");
 // Serial.print(",");
  readParam();  
  delay1=param;          // global varaible for current gain
  delay(20);

  ////// number of runs for data logging //////////////
  Serial.print("runs, ");
//  Serial.print(",");
  readParam();
  runs=param;                
  delay(20);

  //////// delay between runs for data logging //////////////
  Serial.print("log delay, ");
 // Serial.print(",");
  readParam();
  runDelay=param;                
  delay(20);

  //////////// read time for chronoAmp experiments
  Serial.println("read time");
  readParam();
  readTime=param;                
  delay(20);
  status();      // check to see if % sent, if so, reset 
  /********************* print values for header ********************/
  Serial.print(",,");              // two commas to get headers out of data columns
  Serial.print(mode);
  Serial.print(",");
  Serial.print(mVinit);
  Serial.print(",");
  Serial.print(mVfnl);
  Serial.print(",");
  Serial.print(scanRate);
  Serial.print(",");
  Serial.print(dOff); 
  Serial.print(",");
  Serial.print(delay1); 
  Serial.print(",");
  Serial.print(runs);
  Serial.print(",");
  Serial.print(runDelay); 
  Serial.print(",");
  Serial.println(readTime); 

  /////// print header ///////////////
  Serial.print("Vread,");
//  Serial.print(",");
  Serial.println("Iread");
  Serial.println("&");
  //  Serial.println(" ");
}  
/******************************  end setupRun *******************************/

/****************************** read Paramaters ****************************/
void readParam() {
  long place[6]={
    100000,10000,1000,100,10,1      };           
  int incomingByte[6] = {
    0,0,0,0,0,0      };      // serial read variable. each incoming byte (multiplied by the place)
  int n=0;
  param = 0;
  delayMicroseconds(300);  
  while (Serial.available()<= 0) {
  }
  while (Serial.available() > 0){  
    incomingByte[n] = Serial.read()-48;
    delay(2);
    ++n;          
  }

  for(int n=0;n<6;++n){ 
    param = param + (place[n]*(incomingByte[n]));  
  }
}          



