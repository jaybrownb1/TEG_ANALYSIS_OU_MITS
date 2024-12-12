#include <Arduino.h>
#include <U8x8lib.h>
# include <SoftwareSerial.h>
 
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/*reset=*/U8X8_PIN_NONE);
static char recv_buf[512];
static bool is_exist = false;
static bool is_join = false;
const byte rxPin = 2;
const byte txPin = 3;
 
const byte ultrasonicPin = 4;
 
//----- what pin the components are connected to -----
 
SoftwareSerial SoftSerial (rxPin, txPin);       // Set up a new SoftwareSerial object
 
static int at_send_check_response(char *p_ack, int timeout_ms, char *p_cmd, ...) {
  int ch;
  int index = 0;
  int startMillis = 0;
  va_list args;
  memset(recv_buf, 0, sizeof(recv_buf));
  va_start(args, p_cmd);
  Serial1.printf(p_cmd, args);
  Serial.printf(p_cmd, args);
  va_end(args);
  delay(200);
  startMillis = millis();
 
  if (p_ack == NULL) {
    return 0;
  }
 
  do {
    while (Serial1.available() > 0) {
      ch = Serial1.read();
      recv_buf[index++] = ch;
      Serial.print((char)ch);
      delay(2);
    }
 
    if (strstr(recv_buf, p_ack) != NULL) {
      return 1;
    }
 
  } while (millis() - startMillis < timeout_ms);
  return 0;
}
 
static void send_synthetic_data() {
  static int synthetic_data = 0;
  char cmd[128];
  sprintf(cmd, "AT+CMSGHEX=\"%04X\"\r\n", synthetic_data);
  int ret = at_send_check_response("Done", 5000, cmd);
  if (ret) {
    Serial.print("Sent synthetic data: ");
    Serial.println(synthetic_data);
  } else {
    Serial.print("Send failed!\r\n\r\n");
  }
  synthetic_data++;  // Change the synthetic data for the next iteration
}
 
void setup(void) {
  u8x8.begin();
  u8x8.setFlipMode(1);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
 
  Serial.begin(9600);
 
  pinMode(rxPin, INPUT);            // Define pin modes for TX and RX
  pinMode(txPin, OUTPUT);
  Serial1.begin(9600);
  Serial.print("E5 LORAWAN TEST\r\n");
  u8x8.setCursor(0, 0);
 
  // Additional setup code for LoRaWAN configuration and initialization can be added here
  if (at_send_check_response("+AT: OK", 100, "AT\r\n")) {
    is_exist = true;
    at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
    //at_send_check_response("+ID: DevEui", 1000, "AT+ID\r\n");
    //at_send_check_response("+ID: DevAddr", 1000, "AT+ID\r\n");
    at_send_check_response("+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n");
    at_send_check_response("+DR: US915", 1000, "AT+DR=0\r\n");
    at_send_check_response("+CH: NUM", 1000, "AT+CH=NUM,8\r\n");
    at_send_check_response("+KEY: APPKEY", 1000, "AT+KEY=APPKEY,\"2B7E151628AED2A6ABF7158809CF4F3C\"\r\n");
    at_send_check_response("+CLASS: C", 1000, "AT+CLASS=A\r\n");
    at_send_check_response("+PORT: 8", 1000, "AT+PORT=8\r\n");
    delay(200);
    u8x8.setCursor(5, 0);
    u8x8.print("LoRaWAN");
    is_join = true;
  } else {
    is_exist = false;
    Serial.print("No E5 module found.\r\n");
    u8x8.setCursor(0, 1);
    u8x8.print("unfound E5 !");
  }
}
 
/*void loop(void) {
  if (is_exist) {
    if (is_join) {*/
      // Additional code for joining the network can be added here
void loop(void) {
  if (is_exist) {
    if (is_join) {
      int ret = at_send_check_response("+JOIN: Network joined", 12000, "AT+JOIN\r\n");
      if (ret) {
        is_join = false;
      } else {
        at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
        Serial.print("JOIN failed!\r\n\r\n");
        delay(5000);
      }
    } else {
      send_synthetic_data();  // Call the function to send synthetic data
      delay(5000);
    }
  } else {
    delay(1000);
  }
}
   /* } else {
      send_synthetic_data();  // Send synthetic data instead of using sensor readings
      delay(5000);
    }
  } else {
    delay(1000);
  }
}*/