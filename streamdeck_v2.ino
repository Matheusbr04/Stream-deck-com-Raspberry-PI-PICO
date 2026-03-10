#include <Adafruit_TinyUSB.h>

#define LED_R 20
#define LED_G 21
#define LED_B 22

#define NUM_BUTTONS 9

int buttonPins[NUM_BUTTONS] = {5,6,7,8,9,10,11,12,13};
bool lastState[NUM_BUTTONS];

Adafruit_USBD_HID usb_hid;

uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD()
};

int r=255;
int g=0;
int b=0;

int phase=0;
int fade=0;

void setColor(int r,int g,int b){
  analogWrite(LED_R,r);
  analogWrite(LED_G,g);
  analogWrite(LED_B,b);
}

void rainbowEffect(){

  switch(phase){

    case 0:
      r=255;
      g=fade;
      b=0;
      break;

    case 1:
      r=255-fade;
      g=255;
      b=0;
      break;

    case 2:
      r=0;
      g=255;
      b=fade;
      break;

    case 3:
      r=0;
      g=255-fade;
      b=255;
      break;

    case 4:
      r=fade;
      g=0;
      b=255;
      break;

    case 5:
      r=255;
      g=0;
      b=255-fade;
      break;
  }

  setColor(r,g,b);

  fade+=5;

  if(fade>=255){
    fade=0;
    phase++;
    if(phase>5) phase=0;
  }
}

void sendKey(uint8_t key){
  uint8_t keycode[6] = { key,0,0,0,0,0 };
  usb_hid.keyboardReport(0,0,keycode);
  delay(10);
  usb_hid.keyboardRelease(0);
}

void setup(){

  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);

  for(int i=0;i<NUM_BUTTONS;i++){
    pinMode(buttonPins[i],INPUT_PULLUP);
    lastState[i]=HIGH;
  }

  usb_hid.setReportDescriptor(desc_hid_report,sizeof(desc_hid_report));
  usb_hid.begin();

  while(!TinyUSBDevice.mounted()) delay(10);
}

void loop(){

  rainbowEffect();   // efeito RGB contínuo

  for(int i=0;i<NUM_BUTTONS;i++){

    bool state=digitalRead(buttonPins[i]);

    if(state==LOW && lastState[i]==HIGH){

      switch(i){

        case 0: sendKey(HID_KEY_F13); break;
        case 1: sendKey(HID_KEY_F14); break;
        case 2: sendKey(HID_KEY_F15); break;
        case 3: sendKey(HID_KEY_F16); break;
        case 4: sendKey(HID_KEY_F17); break;
        case 5: sendKey(HID_KEY_F18); break;
        case 6: sendKey(HID_KEY_F19); break;
        case 7: sendKey(HID_KEY_F20); break;
        case 8: sendKey(HID_KEY_F21); break;

      }

      delay(150);
    }

    lastState[i]=state;
  }

  delay(10);
}
