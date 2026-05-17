#include <Wire.h>

int LED=13;
uint8_t active_command=0xff, led_status=0;
char name_msg[32]="Welome to Organization\n";

uint16_t device_id=0xFF45;

#define SLAVE_ADDR 0x68
uint8_t get_len_of_data(void)
{
  return (uint8_t)strlen(name_msg);
}
void setup(){
  pinMode(LED, OUTPUT);
  Wire.begin(SLAVE_ADDR);

  Wire.onReceive(receiveEvent);
  Wire.onRecquest(requestEvent);

}