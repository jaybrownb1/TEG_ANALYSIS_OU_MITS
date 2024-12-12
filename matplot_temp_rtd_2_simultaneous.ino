#include <Adafruit_MAX31865.h>

// Define CS pins for each MAX31865 device
#define CS_PIN1 8  // CS pin for the first sensor
#define CS_PIN2 1 // CS pin for the second sensor
#define DI_PIN 10   // Data In pin (shared)
#define DO_PIN 9   // Data Out pin (shared)
#define CLK_PIN 6  // Clock pin (shared)

// Create instances for both sensors
Adafruit_MAX31865 thermo1 = Adafruit_MAX31865(CS_PIN1, DI_PIN, DO_PIN, CLK_PIN);
Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(CS_PIN2, DI_PIN, DO_PIN, CLK_PIN);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
#define RNOMINAL  100.0

const int numReadings = 50; // Number of temperature readings

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");

  // Initialize both sensors
  thermo1.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
  thermo2.begin(MAX31865_3WIRE);
  
  // Set CS pins as OUTPUT and disable both sensors initially
  pinMode(CS_PIN1, OUTPUT);
  pinMode(CS_PIN2, OUTPUT);
  digitalWrite(CS_PIN1, HIGH); // Disable sensor 1
  digitalWrite(CS_PIN2, HIGH); // Disable sensor 2
}

void loop() {
  for (int i = 0; i < numReadings; i++) {
    // Read temperature from the first sensor
    digitalWrite(CS_PIN1, LOW);  // Enable CS for sensor 1
    uint16_t rtd1 = thermo1.readRTD();
    float temperature1 = thermo1.temperature(RNOMINAL, RREF);
    digitalWrite(CS_PIN1, HIGH); // Disable CS for sensor 1

    // Read temperature from the second sensor
    digitalWrite(CS_PIN2, LOW);  // Enable CS for sensor 2
    uint16_t rtd2 = thermo2.readRTD();
    float temperature2 = thermo2.temperature(RNOMINAL, RREF);
    digitalWrite(CS_PIN2, HIGH); // Disable CS for sensor 2

    // Send temperature readings over Serial
    Serial.print("Sensor 1 Temperature: ");
    Serial.println(temperature1);
    
    Serial.print("Sensor 2 Temperature: ");
    Serial.println(temperature2);

    // Delay for 1 second between readings
    delay(1000);
  }

  // Stop the loop after collecting 50 readings
  while (true); // Infinite loop to prevent further readings
}
