#include <ArduinoJson.h>
#include <ArduinoJson.hpp>


// Code for Device 1: Receiver and Transmitter with Button and NeoPixel Notification
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Adafruit_NeoPixel.h>


//for serial in
const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

int dataNumber = 0;

// for communication
struct package {
  int buttonType;
  char sender[300] = "police";
};

typedef struct package Package;
Package dataRecieve;
Package dataTransmit;

class Button {
private:
  bool _state;
  bool _lastButtonState = false;
  uint8_t _pin;
  unsigned long _lastDebounceTime = 0;
  unsigned long _debounceDelay = 25;

public:
  Button(uint8_t pin)
    : _pin(pin) {}

  void begin() {
    pinMode(_pin, INPUT_PULLUP);
    _state = digitalRead(_pin);
  }

  bool isReleased() {
    bool v = digitalRead(_pin);

    if (v!= _lastButtonState) {
      _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > _debounceDelay) {
      if (v != _state) {
        _state = v;
        if (_state) {
          return true;
        }
      }
    }
    _lastButtonState = v;
    return false;
  }
};

RF24 radio(9, 8); // CE, CSN
const byte addresses[][10] = {"ADDRESS01","ADDRESS02"};
Button ACTIVATE_BUTTON(4);
const int pixelPin = 6;   
const int numPixels = 4; 

// Declare the NeoPixel strip
Adafruit_NeoPixel strip(numPixels, pixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Serial.println("police");


  ACTIVATE_BUTTON.begin();
  radio.begin();
  radio.setChannel(124); 
  radio.openWritingPipe(addresses[1]); // ADDRESS02
  radio.openReadingPipe(1, addresses[0]); // ADDRESS01
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  delay(10);
  radio.startListening(); // Always listening

  if (Serial.available()) {
    // String temp = Serial.readString();
    // char * tab2 = new char [temp.length()+1];
    // strcpy (tab2, temp.c_str());
    // deserializeJson(serialIn, tab2);
    // int eventType = serialIn["event"];
    for (int i=1; i<4; i++) {
      checkOk(i + 1);
      if (radio.available()) {
        setStripColor(0, 0, 0, 4);
        break;
      }
      Serial.print("{\"event\":2, \"repeated\":"); Serial.print(i); Serial.println("}");
    }
  }

  if (radio.available()) {

    radio.read(&dataRecieve, sizeof(dataRecieve));
    Serial.print("{\"event\":0, \"buttonType\":"); Serial.print(dataRecieve.buttonType); Serial.println("}"); 
    
    if (dataRecieve.buttonType == 2) {
      for (int i = 0; i < 3; i++) {
        setStripColor(0, 0, 255, 4); 
        delay(200);
        setStripColor(0, 0, 0, 4);   
        delay(200);
      }
    } else if (dataRecieve.buttonType == 3) {
      for (int i = 0; i < 3; i++) {
        setStripColor(255, 0, 0, 4); 
        delay(200);
        setStripColor(0, 0, 0, 4);   
        delay(200);
      }
    }
  }

  if (ACTIVATE_BUTTON.isReleased()) { 
    // Serial.println("here");
    for (int i=1; i<4; i++) {
      checkOk(i + 1);
      if (radio.available()) {break;}
      Serial.print("{\"event\":2, \"repeated\":"); Serial.print(i); Serial.println("}");
    }

    if (radio.available()){
      Serial.println("{\"event\":1}");
      for (int i = 0; i < 3; i++) {
        setStripColor(0, 255, 0, 4); 
        delay(200);
        setStripColor(0, 0, 0, 4);   
        delay(200);
      }
    }
  }
}

void setStripColor(uint8_t red, uint8_t green, uint8_t blue, int numPix) {
  for(int i = 0; i < numPix; i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

void checkOk(int attempt) {
  radio.stopListening();
  // Serial.println("radio.stopListening();");
  dataTransmit.buttonType = 0;
  // Serial.println("dataTransmit.buttonType = 0;");
  radio.write(&dataTransmit, sizeof(dataTransmit));
  // Serial.println("radio.write(&dataTransmit, sizeof(dataTransmit));");
  // Serial.println("Sent to: child");

  for (int i = 0; i < 3; i++){
    setStripColor(0, 0, 0, 4);
    delay(200);
    setStripColor(255, 0, 0, attempt);
    delay(200);
  }

  //start waiting
  radio.startListening();
  // Serial.println("radio.startListening();");
  unsigned long started_waiting_at = millis();
  // Loop here until we get indication that some data is ready for us to read (or we time out)
  while ( ! radio.available() ) {
    // Oh dear, no response received within our timescale
    if (millis() - started_waiting_at > 5000 ) {
      // Serial.println("No respond from child for too long.");
      Serial.println("{\"event\":3}");
      return;
    }
  }
}