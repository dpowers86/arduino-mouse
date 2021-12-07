// Serial is over USB
// Serial1 is over Tx/Rx pins
// 2 - SDA
// 3 - SCL
// 9 - Switch (Testing / Mouse Button)
// I had the ball connected to 5V

#include <Mouse.h>
#include <Wire.h>

int selPin = 9;  // select button pin of joystick
int mouseClickFlag = 0;
const int sensitivity = 5;  // Higher sensitivity value = slower mouse, should be <= about 500

// Pimoroni
const int I2C_ADDRESS = 0x0A;
const int I2C_ADDRESS_ALTERNATIVE = 0x0B;

const int CHIP_ID = 0xBA11;
const int VERSION = 1;

const int REG_LED_RED = 0x00;
const int REG_LED_GRN = 0x01;
const int REG_LED_BLU = 0x02;
const int REG_LED_WHT = 0x03;

const int REG_LEFT = 0x04;
const int REG_RIGHT = 0x05;
const int REG_UP = 0x06;
const int REG_DOWN = 0x07;
const int REG_SWITCH = 0x08;
const int MSK_SWITCH_STATE = 0b10000000;

const int REG_USER_FLASH = 0xD0;
const int REG_FLASH_PAGE = 0xF0;
const int REG_INT = 0xF9;
const int MSK_INT_TRIGGERED = 0b00000001;
const int MSK_INT_OUT_EN = 0b00000010;
const int REG_CHIP_ID_L = 0xFA;
const int RED_CHIP_ID_H = 0xFB;
const int REG_VERSION = 0xFC;
const int REG_I2C_ADDR = 0xFD;
const int REG_CTRL = 0xFE;
const int MSK_CTRL_SLEEP = 0b00000001;
const int MSK_CTRL_RESET = 0b00000010;
const int MSK_CTRL_FREAD = 0b00000100;
const int MSK_CTRL_FWRITE = 0b00001000;


void setup()
{
  // Debug Serial Port
  Serial1.begin(115200);
  Serial1.println("Hello World!");
  
  // Start I2C
  Wire.begin();
  
  pinMode(selPin, INPUT);  // set button select pin as input
  digitalWrite(selPin, HIGH);  // Pull button select pin high
  delay(1000);  // short delay to let outputs settle

  
  set_rgbw(0,255,0,0);
}

byte last_left = 0;
byte last_right = 0;
byte last_up = 0;
byte last_down = 0;
byte last_sw = 0;

void loop()
{
  int vertValue = 0;
  int horzValue = 0;
  
  if (vertValue != 0)
    Mouse.move(0, vertValue/sensitivity, 0);  // move mouse on y axis
  if (horzValue != 0)
    Mouse.move(horzValue/sensitivity, 0, 0);  // move mouse on x axis
    
  if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
  {
    mouseClickFlag = 1;
    
    //Mouse.press(MOUSE_LEFT);  // click the left button down
  }
  else if ((digitalRead(selPin))&&(mouseClickFlag)) // if the joystick button is not pressed
  {
    mouseClickFlag = 0;
    
    //Mouse.release(MOUSE_LEFT);  // release the left button
  }

  // Read I2C Status of Ball
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(REG_LEFT);
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDRESS, 5);
  
  while(Wire.available() < 5){
    // Wait to recieve the 5 flags
  }

  byte left = Wire.read();
  byte right = Wire.read();
  byte up = Wire.read();
  byte down = Wire.read();
  byte sw = Wire.read();

  if(left != last_left){
    last_left = left;
    Serial1.println("Left");
    Mouse.move(-1 * sensitivity, 0, 0);
  }

  if(right != last_right){
    last_right = right;
    Serial1.println("Right");
    Mouse.move(1 * sensitivity, 0, 0);
  }

  if(up != last_up){
    last_up = up;
    Serial1.println("Up");
    Mouse.move(0, 1 * sensitivity, 0);
  }

  if(down != last_down){
    last_down = down;
    Serial1.println("Down");
    Mouse.move(0, -1 * sensitivity, 0);
  }

  if(sw != last_sw){
    last_sw = sw;
    Serial1.println("Switch");

    if(sw = 1){
      Mouse.click();
    }
  }
}



/**
 * Set all LED brightness as RGBW.
 */
void set_rgbw(byte r, byte g, byte b, byte w){
  byte buff[5];
  buff[0] = REG_LED_RED;
  buff[1] = r;
  buff[2] = g;
  buff[3] = b;
  buff[4] = w;
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(buff, 5);
  Wire.endTransmission();
}

/**
 * Set brightness of trackball red LED.
 */
void set_red(byte value){
  byte buff[2];
  buff[0] = REG_LED_RED;
  buff[1] = value;
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(buff, 2);
  Wire.endTransmission();
}

/**
 * Set brightness of trackball green LED.
 */
void set_green(byte value){
  byte buff[2];
  buff[0] = REG_LED_GRN;
  buff[1] = value;
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(buff, 2);
  Wire.endTransmission();
}


/**
 * Set brightness of trackball blue LED.
 */
void set_blue(byte value){
  byte buff[2];
  buff[0] = REG_LED_BLU;
  buff[1] = value;
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(buff, 2);
  Wire.endTransmission();
}

/**
 * Set brightness of trackball white LED.
 */
void set_white(byte value){
  byte buff[2];
  buff[0] = REG_LED_WHT;
  buff[1] = value;
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(buff, 2);
  Wire.endTransmission();
}
