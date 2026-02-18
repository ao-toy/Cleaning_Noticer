/*

uint8_t floors[3][4] = {{102, 105, 107, 108}, {109, 113, 118, 111}, {112, 114, 115, 117}};
uint8_t bath[3][5] = {{106, 102, 104, 107, 108}, {103, 109, 113, 118, 111}, {105, 112, 114, 115, 117}};

uint8_t keyf = 0; //通常清掃変更キー
uint8_t keyfd = 0;//通常清掃詳細キー
uint8_t keyb = 0; //風呂清掃変更キー

uint8_t floA[4] = {102, 105, 107, 108};
uint8_t floB[4] = {109, 113, 118, 111};
uint8_t baT[5] = {106, 102, 104, 107, 108};


*/

void cfloor(){
  keyf = (keyf + 2) % 3;
  if(keyf == 0){
    keyfd = (keyfd + 1) % 4;
  }
  for(int i=0; i<4; i++){
    int detail = (keyfd + i) % 4;
    int keyfB = (1 + keyf) % 3;
    floA[i] = floors[keyf][detail];
    floB[i] = floors[keyfB][detail];
  }
}

void cbath(){
  keyb = (keyb + 1) % 3;
  for(int i=0; i<5; i++){
    baT[i] = bath[keyb][i];
  }
}