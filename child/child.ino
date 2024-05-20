// Code for Device 1: Receiver and Transmitter with Button and NeoPixel Notification
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Adafruit_NeoPixel.h>
#include <TimerHelpers.h>

struct package {
  int buttonType;
  char sender[300] = "child";
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
Button OK_BUTTON(4); 
Button COUNSELING_BUTTON(2);
Button EMERGENCY_BUTTON(10);
const int pixelPin = 6;   
const int numPixels = 5; 

const byte n = 17;  // for frequency

const byte OUTPUT_PIN = 3;  // Timer 2 "B" output: OC2B
const byte CONTROL_PIN = 5;

// Declare the NeoPixel strip
Adafruit_NeoPixel strip(numPixels, pixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial.println("child");


  OK_BUTTON.begin();
  COUNSELING_BUTTON.begin();
  EMERGENCY_BUTTON.begin();
  radio.begin();
  radio.setChannel(124); 
  radio.openWritingPipe(addresses[1]); // ADDRESS02
  radio.openReadingPipe(1, addresses[0]); // ADDRESS01
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode (OUTPUT_PIN, OUTPUT);
  pinMode(CONTROL_PIN, OUTPUT);

  TCCR2A = bit(WGM20) | bit(WGM21) | bit(COM2B1);  // fast PWM, clear OC2A on compare
  TCCR2B = bit(WGM22) | bit(CS22);                 // fast PWM, prescaler of 64
  OCR2A = n;                                       // from table
  OCR2B = ((n + 1) / 2) - 1;                       // 50% duty cycle
  
}

void loop() {
  delay(10);
  radio.startListening(); // Always listening

  if (radio.available()) {
    // char txt_received[10] = "";
    // radio.read(&txt_received, sizeof(txt_received));

    digitalWrite(CONTROL_PIN, 1);

    radio.read(&dataRecieve, sizeof(dataRecieve));
    Serial.print("Received from: "); Serial.println(dataRecieve.sender);
    Serial.print("Button Type: "); Serial.println(dataRecieve.buttonType);
    Serial.println("---------------------------------");
    Serial.println(" ");
    
    
    for (int i = 0; i < 3; i++) {
      setStripColor(255, 0, 0); 
      delay(200);
      setStripColor(0, 0, 0);   
      delay(200);
    }    
  }
//  Serial.println(count);
  if (OK_BUTTON.isReleased()) { 
    digitalWrite(CONTROL_PIN, 0);

    radio.stopListening();
    // const char txt_sent[] = "Police";
    // radio.write(&txt_sent, sizeof(txt_sent));
    dataTransmit.buttonType = 1;
    radio.write(&dataTransmit, sizeof(dataTransmit));
    Serial.println("Sent to: police");
    Serial.println("Button Type: OK");
    Serial.println("---------------------------------");
    Serial.println(" ");
  }

  if (COUNSELING_BUTTON.isReleased()) { 
    digitalWrite(CONTROL_PIN, 0);

    radio.stopListening();
    // const char txt_sent[] = "Police";
    // radio.write(&txt_sent, sizeof(txt_sent));
    dataTransmit.buttonType = 2;
    radio.write(&dataTransmit, sizeof(dataTransmit));
    Serial.println("Sent to: police");
    Serial.println("Button Type: COUNSEL");
    Serial.println("---------------------------------");
    Serial.println(" ");
  }

  if (EMERGENCY_BUTTON.isReleased()) { 
    digitalWrite(CONTROL_PIN, 0);

    radio.stopListening();
    // const char txt_sent[] = "Police";
    // radio.write(&txt_sent, sizeof(txt_sent));
    dataTransmit.buttonType = 3;
    radio.write(&dataTransmit, sizeof(dataTransmit));
    Serial.println("Sent to: police");
    Serial.println("Button Type: EMERGENCY");
    Serial.println("---------------------------------");
    Serial.println(" ");
  }
}

void setStripColor(uint8_t red, uint8_t green, uint8_t blue) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}
