#include <MsTimer2.h>

// 4digits 7segLED cathode pins
#define D1 10
#define D2 11
#define D3 12
#define D4 13

// dwitch pins
#define FLOOC A3
#define BATHC A4

// notification LED pins
#define FLOON A0
#define BATHN A1

// notification speaker pins
#define SPEAK A2

// type of runseg
#define NTIME 0
#define NBATH 1
#define NFLOO 2

// time to show a room number
#define STIME 1500

unsigned int timc = 0;                                                                                          // 500ms counter (0 ~ 499)
unsigned int ntim = 0; // 1284;                                                                                 // current time(0 ~ 1439);
uint8_t floors[3][4] = {{ 102, 105, 107, 108 }, { 109, 113, 118, 111 }, { 112, 114, 115, 117 } };               // room number list Init(floor cleaning)
uint8_t bath[3][5] = { { 106, 102, 104, 107, 108 }, { 103, 109, 113, 118, 111 }, { 105, 112, 114, 115, 117 } }; // room number list Init(bathroom cleaning)

uint8_t keyf = 1;   // key of change of room number(floor cleaning 1) (0~2)
uint8_t keyfd =3;   // key of change of room number(floor cleaning 2) (0~3)
uint8_t keyb = 2;   // kee of change of room number(bathroom cleaning)(0~2)

uint8_t floA[4] = {0/* 102, 105, 107, 108 */};
uint8_t floB[4] = {0/* 109, 113, 118, 111 */};
uint8_t baT[5] = {0/* 106, 102, 104, 107, 108 */};

uint8_t week = 0;      //0~6
uint8_t nineDays = 0;  //day counter for bathroom cleaning 0~8

uint8_t dp = 1;

uint8_t dpo = 1;  //dot point lock

int callno = -1;
uint8_t place = 0;

using namespace MsTimer2;

bool ch = 0;
int xyz = -1;
int uvw = -1;

bool Flock = 0;
bool Block = 0;

void setup() {
  // put your setup code here, to run once:

  // set timer interrupt
  set(500, timer_count);

  for (int i = 2; i <= 16; i++) {
    pinMode(i, OUTPUT);
  }

  // floA, floB, baT Init
  cfloor();
  cbath();

  pinMode(FLOOC, INPUT_PULLUP);
  pinMode(BATHC, INPUT_PULLUP);

  Serial.begin(9600);

  start();
}

void loop() {
  // put your main code here, to run repeatedly:

  // notification floor cleaning
  if (week == 0 || week == 2 || week == 4) {
    digitalWrite(FLOON, HIGH);
    if (ntim == 1320 && !Flock) {
      tellF();
      Flock = true;
    }

    if (ntim != 1320) Flock = false;

  } else {
    digitalWrite(FLOON, LOW);
  }

  // notification bathroom cleaning
  if ((nineDays + 1) % 3 == 0) {
    digitalWrite(BATHN, HIGH);
    if (ntim == 1285 && !Block) {
      tellB();
      Block = true;
    }

    if (ntim != 1285) Block = false;

  } else {
    digitalWrite(BATHN, LOW);
  }

  // check floor cleaning
  if (digitalRead(FLOOC) == 0) {
    dpo = 0;
    ch = 1;
    digitalWrite(SPEAK, LOW);
    for (int i = 0; i < 4; i++) {
      uvw = -1;
      xyz = i;
      runseg(floA[i], NFLOO);
      digitalWrite(FLOON, 1 - digitalRead(FLOON));
      secshow(STIME);
    }
    xyz = -1;
    for (int i = 0; i < 4; i++) {
      xyz = -1;
      uvw = i;
      runseg(floB[i], NFLOO);
      digitalWrite(FLOON, 1 - digitalRead(FLOON));
      secshow(STIME);
    }
    uvw = -1;
    ch = 0;
    dpo = 1;
  }

  // check bath cleaning
  if (digitalRead(BATHC) == 0) {
    dpo = 0;
    digitalWrite(SPEAK, LOW);
    for (int i = 0; i < 5; i++) {
      runseg(baT[i], NBATH);
      digitalWrite(BATHN, 1 - digitalRead(BATHN));
      secshow(STIME);
    }
    dpo = 1;
  }

  // show current time
  runseg(ntim, NTIME);
  seven_seg();
}

void timer_count() {
  timc++;
  
  // probably unused
  if (timc % 2 == 0) {
    callno = 0;
  }

  // status 3rd DP Pin(for show current time)
  dp = 1 - dp;

  // update current tie
  if (timc >= 120) {

    timc = 0;
    ntim++;
    if (ntim >= 1440) {

      week = (week + 1) % 7;
      nineDays = (nineDays + 1) % 9;

      // update floA, floB
      if (week == 0) {
        cfloor();
      }

      // update baT
      if ((nineDays + 1) % 3 == 1) {
        cbath();
      }

      ntim = 0;
    }
  }
}


// show the number for [fitme]msec
void secshow(uint16_t ftime) {

  int ss = millis();
  int sn = ss;
  while (sn - ss <= ftime) {
    seven_seg();
    sn = millis();
  }
}

// notification floor cleaning
void tellF() {
  dpo = 0;
  ch = 1;
  digitalWrite(SPEAK, HIGH);
  while (digitalRead(FLOOC) == 1) {
    for (int i = 0; i < 4; i++) {
      uvw = -1;
      xyz = i;
      runseg(floA[i], NFLOO);
      digitalWrite(FLOON, 1 - digitalRead(FLOON));
      loopshow(STIME, FLOOC);
    }
    xyz = -1;
    for (int i = 0; i < 4; i++) {
      xyz = -1;
      uvw = i;
      runseg(floB[i], NFLOO);
      loopshow(STIME, FLOOC);
    }
    uvw = -1;
  }
  ch = 0;
  dpo = 1;
  digitalWrite(SPEAK, LOW);
}

// notification bathroom cleaning
void tellB() {
  dpo = 0;
  digitalWrite(SPEAK, HIGH);
  while (digitalRead(BATHC) == 1) {
    for (int i = 0; i < 5; i++) {
      runseg(baT[i], NBATH);

      loopshow(STIME, BATHC);
    }
  }
  digitalWrite(SPEAK, LOW);
  dpo = 1;
}

// notification showing
void loopshow(uint16_t ftime, uint8_t pin) {
  int ss = millis();
  int sn = ss;
  int timi = 0;
  while (sn - ss <= ftime && digitalRead(pin) == 1) {
    seven_seg();
    sn = millis();
    if ((sn - ss) / 125 > timi) {
      timi++;
      digitalWrite(SPEAK, !digitalRead(SPEAK));
      if (pin == FLOOC) {
        digitalWrite(FLOON, 1 - digitalRead(FLOON));
      } else {
        digitalWrite(BATHN, 1 - digitalRead(BATHN));
      }
    }
  }
}