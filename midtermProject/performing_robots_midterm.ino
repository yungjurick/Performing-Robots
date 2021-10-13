#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define NEO_PIN 3
#define NUMPIXELS 64

const int CEPIN = 9;
const int CSNPIN = 10;

RF24 radio(CEPIN, CSNPIN);
const byte address[6] = "00001";

/* SERVO VARIBLES */
Servo LEFT_ARM;
Servo RIGHT_ARM;

int LEFT_ARM_POS = 180;
int RIGHT_ARM_POS = 0;

bool LEFT_ARM_MOVE_UP = true;
bool RIGHT_ARM_MOVE_UP = true;

/* NANO PIXEL VARIABLES */
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

int DULL_FACE[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 0, 0, 1, 1, 0,
  0, 1, 1, 0, 0, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};

int HAPPY_FACE[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};

int SAD_FACE[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 1, 1, 0, 0, 1, 1, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};

const int DULL_INDEX = 0b00000001;
const int HAPPY_INDEX = 0b00000010;
const int ANGRY_INDEX = 0b00000100;
int CUR_FACE_INDEX = 0;

bool hasStarted = false;
bool needFaceUpdate = false;
bool needHappyArmPosUpdate = false;
bool needSadArmPosUpdate = false;
bool isHoldAfterSequence = false;

unsigned long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  pixels.begin();
  LEFT_ARM.attach(5);
  RIGHT_ARM.attach(6);

  LEFT_ARM.write(180);
  RIGHT_ARM.write(0);

  for (int i = 0; i < NUMPIXELS; i++) {
     if (DULL_FACE[i] == 1) {
       pixels.setPixelColor(i, pixels.Color(0, 0, 100)); 
     } else {
       pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
     }
  }

  pixels.show();
  
  Serial.begin(115200);
  
  // RF24 setup
  if (!radio.begin()) {
    Serial.println("radio  initialization failed");
    while (1)
      ;
  } else {
    Serial.println("radio successfully initialized");
  }
  
  radio.openReadingPipe(0, address);  // Destination address
  radio.setPALevel(RF24_PA_MIN);      // Min or max
  radio.startListening();             // sets  module as receiver
}

void loop() {
    unsigned long currentMillis = millis();

    uint8_t pipeNum;
    if (radio.available(&pipeNum)) {
      int data;
  
      Serial.print("data available on pipe ");
      Serial.println(pipeNum);
  
      radio.read(&data, sizeof(data));  //Reading the data
      Serial.print("data = ");
      Serial.println( data);
  
      switch (data) {
        case 0b00000001:
          Serial.println("START SEQUENCE");
          hasStarted = true;
          needFaceUpdate = true;
          needHappyArmPosUpdate = true;
          needSadArmPosUpdate = false;
          CUR_FACE_INDEX = 1;

          LEFT_ARM_POS = 180;
          RIGHT_ARM_POS = 0;

          LEFT_ARM.write(LEFT_ARM_POS);
          RIGHT_ARM.write(RIGHT_ARM_POS);
          delay(50);
          
          break;
        default:
          Serial.println("invalid code");
          break;
      }
    }
    
    if (needHappyArmPosUpdate) {
      Serial.print("HAPPY ARM");
      Serial.print(LEFT_ARM_POS);
      Serial.print(RIGHT_ARM_POS);
      Serial.print("\n");
      
      if (LEFT_ARM_POS >= 0) {
        LEFT_ARM_POS -= 5;
        LEFT_ARM.write(LEFT_ARM_POS);
      }

      if (RIGHT_ARM_POS < 180) {
        RIGHT_ARM_POS += 5;
        RIGHT_ARM.write(RIGHT_ARM_POS);
      }

      delay(50);

      if (LEFT_ARM_POS <= 0 && RIGHT_ARM_POS >= 180) {
        needHappyArmPosUpdate = false;
      }
    } else if (
      !needHappyArmPosUpdate &&
      !needSadArmPosUpdate &&
      RIGHT_ARM_POS > 0 &&
      LEFT_ARM_POS >= 0
     ) {      
      LEFT_ARM_POS += 1;
      RIGHT_ARM_POS -= 1;
      LEFT_ARM.write(LEFT_ARM_POS);
      RIGHT_ARM.write(RIGHT_ARM_POS);

      Serial.print("Returning Back");
      Serial.print(LEFT_ARM_POS);
      Serial.print(RIGHT_ARM_POS);
      Serial.print("\n");

      delay(15);
    }

    // Convert Back to Stagnant State
    if (
      hasStarted &&
      !needHappyArmPosUpdate &&
      !isHoldAfterSequence &&
      !needSadArmPosUpdate &&
      RIGHT_ARM_POS == 0 &&
      LEFT_ARM_POS == 180 
     ) {
      Serial.println("Done");
      needFaceUpdate = true;
      isHoldAfterSequence = true;
      CUR_FACE_INDEX = 0;
      previousMillis = currentMillis;
    }

    if (isHoldAfterSequence) {
      if (currentMillis - previousMillis >= 5000) {
        Serial.println("STOP HOLD AND SAD BEGIN");
        needSadArmPosUpdate = true;
        isHoldAfterSequence = false;
        needFaceUpdate = true;
        CUR_FACE_INDEX = 2;

        LEFT_ARM.write(90);
        RIGHT_ARM.write(90);
        delay(50);
      }
    }

    if (needSadArmPosUpdate) {
      Serial.println("IS SAD");
      if (LEFT_ARM_POS > 120) {
        LEFT_ARM_MOVE_UP = false;
      } else if (LEFT_ARM_POS < 60) {
        LEFT_ARM_MOVE_UP = true;
      }

      if (RIGHT_ARM_POS > 120) {
        RIGHT_ARM_MOVE_UP = false;
      } else if (RIGHT_ARM_POS < 60) {
        RIGHT_ARM_MOVE_UP = true;
      }

      if (LEFT_ARM_MOVE_UP) {
        LEFT_ARM_POS += 1;
        LEFT_ARM.write(LEFT_ARM_POS);
      } else {
        LEFT_ARM_POS -= 1;
        LEFT_ARM.write(LEFT_ARM_POS);
      }

      if (RIGHT_ARM_MOVE_UP) {
        RIGHT_ARM_POS += 1;
        RIGHT_ARM.write(RIGHT_ARM_POS);
      } else {
        RIGHT_ARM_POS -= 1;
        RIGHT_ARM.write(RIGHT_ARM_POS);
      }

      delay(15);
    }

    //  UPDATE NEOPIXEL
    if (needFaceUpdate) {
      needFaceUpdate = false;
      
      switch(CUR_FACE_INDEX) {
        case 0:
          for (int i = 0; i < NUMPIXELS; i++) {
             if (DULL_FACE[i] == 1) {
               pixels.setPixelColor(i, pixels.Color(0, 0, 100)); 
             } else {
               pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
             }
          }
          break;

        case 1:
          for (int i = 0; i < NUMPIXELS; i++) {
             if (HAPPY_FACE[i] == 1) {
               pixels.setPixelColor(i, pixels.Color(0, 100, 0));
             } else {
               pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
             }
          }
          break;

         case 2:
          for (int i = 0; i < NUMPIXELS; i++) {
             if (SAD_FACE[i] == 1) {
               pixels.setPixelColor(i, pixels.Color(100, 0, 0));
             } else {
               pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
             }
          }
          break;
      }

      pixels.show();
    }
}
