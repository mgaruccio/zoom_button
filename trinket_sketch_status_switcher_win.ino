//#include <Adafruit_DotStar.h>
#include <Bounce2.h>
#include <Keyboard.h>

#define BUTTON_PIN 0
#define LED_PIN 2

Bounce debouncer = Bounce();

unsigned long prevMillis = 0;
int brightness = 0;
int incomingByte = 0;
char incomingState;
char muteState = '0';
char unmuteState = '1';
bool increase = true;
bool muted = true;

void setup() {
  debouncer.attach(BUTTON_PIN, INPUT_PULLUP);
  debouncer.interval(30);
  Keyboard.begin();
  Serial.begin(9600);
}
 
void loop() {
  debouncer.update();

  unsigned long nowMillis = millis();
  if (brightness >= 255) {
    increase = false;
  }
  if (brightness <= 0) {
    increase = true;
  }
  if (nowMillis - prevMillis >= 12 && muted) {
    prevMillis = nowMillis;
    if (increase == true) {
      brightness++;
    } else {
      brightness --;
    }
    analogWrite(LED_PIN, brightness);
  } else if (!muted) {
    analogWrite(LED_PIN, 255);
  }
  

  

  if (debouncer.fell()) {
    Serial.println("button pressed");
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('a');
    delay(100);
    Keyboard.releaseAll();
    delay(600);
  }

  if (Serial.available() > 0) {
    //incomingState = Serial.readString();
    //incomingState.trim();
    incomingState = Serial.read();
    Serial.println(incomingState);
    if (incomingState == muteState) {
      Serial.println("muted");
      muted = true;
    } else if (incomingState == unmuteState) {
      Serial.println("unmuted");
      muted = false;
    } else {
      Serial.println("ignoring invalid input");
    }
  }
}
