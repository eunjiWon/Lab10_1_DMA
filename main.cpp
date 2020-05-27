#include "mbed.h"
RawSerial pc(PA_2, PA_3, 115200); // Serial로 하면 pc.getc and pc.putc에서 에러남.

volatile int flag;
static int s_Buffer[1024];
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
char rx_buffer[10];
int idx = 0;

void set_number(void) {
  int num;
  num = atoi((const char *)rx_buffer);
  pc.printf("num = %d \r\n", num);
  // 
  for (int i = 0; i < num; i++){
    if (i % 5 == 0 && i != 0) pc.printf("\r\n");
    pc.printf(" %d ", s_Buffer[i]);
  }
  pc.printf("\r\n");
}

void rx_handler(void) {
  char ch;
  ch = pc.getc();
  pc.putc(ch);
  rx_buffer[idx++] = ch;
  if (ch == 0x0D)
  {                //CR
    pc.putc(0x0A); //LF
    rx_buffer[--idx] = '\0';
    idx = 0;
    flag = 1;
  }
}

int main() {
  pc.attach(&rx_handler);
  pc.printf("\r\nWelcome to Lab10_1!\r\n"); 
  
  // calculate fibonacci
  s_Buffer[0] = 0;
  s_Buffer[1] = 1; 
  // 아래 반복을 num까지만 하게 해도 될듯.
  for (int i = 2; i < ARRAY_SIZE(s_Buffer); i++) {
    s_Buffer[i] = s_Buffer[i - 1] + s_Buffer[i - 2];
  }
  
  while(1){
    flag = 0;
      pc.printf("Enter the number (N<=40): ");
      while (flag != 1) {}
      set_number();
      ThisThread::sleep_for(500);
  }
}
