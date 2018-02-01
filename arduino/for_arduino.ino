/*
  code from the ShiftRegsiter example from Hannio github

 */
#include <FlipDot_5x7.h>
// display connected as this
#define CLOCK 13
#define DATA  11
#define LATCH 10

int dataReceived = 0;

/*
  Wiring:
display arduino
  OUT     Not connected to Arduino
  5V      5V
  LAT     LATCH
  GND     GND
  SCL     CLOCK
  SDA     DATA
  GND     GND (from power supply)
  12V     VIN (from power supply)
  12V     VIN (from power supply)

  Tie the OUT-pin from the module to SDA of the next module.
  Daisychain the other pins. NOT TESTED

  NOTE:
  There is a mistake (LAT, 5V) in the silkscreen on a batch of the PCBs.
  Above wiring is correct.
*/
FlipDot_5x7 flipdot(1, 1, true);
/* Change false to true if the color is inverted. */
int liste[35];
boolean flag = false;
int nItem = 0;
int temp;

void setup() {
  // serial for espruino.
  Serial.begin(19200);
  delay(100);
  // to initiate serial
  Serial.print("");
  //  initiate the dot display, with a flip sequence
  flipdot.begin(DATA, CLOCK, LATCH);
  flipdot.fillScreen(FLIPDOT_YELLOW);
  flipdot.display();
  delay(1000);
  flipdot.fillScreen(FLIPDOT_BLACK);
  flipdot.display();
  // to begin data sending / Begin Mark
  Serial.write("@");
}

void loop() {
  delay(10);
}

void drawText() {
  // draw dot by dot 
  for (int i = 0; i < 35; i++) {
    flipdot.drawPixel(i%5, int(i/5)%7, int(liste[i]));
  }
  // show on display
  flipdot.display();
  flag = false;
  // empty array
  liste[35];
}
 
void serialEvent() {
  // for debugging purpose
  temp += Serial.available();
  // while datas are available in the buffer
  while (Serial.available() > 0 && !flag) {
    dataReceived = byte(Serial.read());
    if (dataReceived == 0x3E) { // if endMark
      Serial.flush();
      // for debugging purpose
      /*Serial.print("n bytes recus: ");
      Serial.println(temp);
      for (int i=0; i<35; i++) {
        Serial.print(liste[i]);
      }
      Serial.println("");*/
      nItem = 0;
      temp = 0;
      flag = true;
      // show on display
      drawText();
    } else {
      // fill the array with datas
      liste[nItem] = dataReceived;
      nItem++;
    }
  }
}
