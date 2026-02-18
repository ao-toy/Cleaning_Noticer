/*
static bool seg0[7] = {1, 1, 1, 1, 1, 1, 0};
static bool seg1[7] = {0, 1, 1, 0, 0, 0, 0};
static bool seg2[7] = {1, 1, 0, 1, 1, 0, 1};
static bool seg3[7] = {1, 1, 1, 1, 0, 0, 1};
static bool seg4[7] = {0, 1, 1, 0, 0, 1, 1};
static bool seg5[7] = {1, 0, 1, 1, 0, 1, 1};
static bool seg6[7] = {1, 0, 1, 1, 1, 1, 1};
static bool seg7[7] = {1, 1, 1, 0, 0, 0, 0};
static bool seg8[7] = {1, 1, 1, 1, 1, 1, 1};
static bool seg9[7] = {1, 1, 1, 1, 0, 1, 1};
*/
#define DTIME 4
#define RTIME 1

static uint8_t ledh[2][10] = { { 0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11110110 }, { 0b11111101, 0b01100001, 0b11011011, 0b11110011, 0b01100111, 0b10110111, 0b10111111, 0b11100001, 0b11111111, 0b11110111 } };

static int8_t digits[4] = { 0 };

void runseg(unsigned int v, uint8_t tpe) {
  if (tpe == NTIME) {
    uint8_t hour = v / 60;
    uint8_t min = v % 60;
    digits[3] = hour / 10;
    digits[2] = hour % 10;
    digits[1] = min / 10;
    digits[0] = min % 10;
  } else if (tpe == NBATH) {
    digits[3] = -1;
    digits[2] = v / 100;
    digits[1] = (v % 100) / 10;
    digits[0] = v % 10;
  } else {
    if ((xyz < 2) && uvw == -1 ) {
      digits[3] = 1;
    } 
    else if(xyz >= 2 && uvw == -1) {
      digits[3] = 2;
    }
    else if(xyz == -1 && uvw < 2) {
      digits[3] = 3;
    }
    else if(xyz == -1 && uvw >= 2) {
      digits[3] = uvw +2;
    }
    // Serial.println(uvw);
    digits[2] = v / 100;
    digits[1] = (v % 100) / 10;
    digits[0] = v % 10;
  }
}

static uint8_t gnds = 0;

void seven_seg() {
  digitalWrite(D4, HIGH);
  
  for (int i = 0; i < 8; i++) {
    digitalWrite(i + 2, (ledh[0][digits[0]] >> (7-i)) & 1);
  }
  digitalWrite(D1, LOW);
  delay(DTIME);

  digitalWrite(D1, HIGH);
  
  for (int i = 0; i < 8; i++) {
    digitalWrite(i + 2, (ledh[0][digits[1]] >> (7-i)) & 1);
  }
  digitalWrite(D2, LOW);
  delay(DTIME);

  digitalWrite(D2, HIGH);
  
  for (int i = 0; i < 8; i++) {
    digitalWrite(i + 2, (ledh[dp * dpo][digits[2]] >> (7-i)) & 1);
  }
  digitalWrite(D3, LOW);
  delay(DTIME);

  digitalWrite(D3, HIGH);

  if (digits[3] == -1) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(i + 2, LOW);
    }
  } else {
    for (int i = 0; i < 8; i++) {
      digitalWrite(i + 2, (ledh[ch][digits[3]] >> (7-i)) & 1);
    }
    
  }
  digitalWrite(D4, LOW);
  delay(DTIME);
  
}

static void allnight() {
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
}