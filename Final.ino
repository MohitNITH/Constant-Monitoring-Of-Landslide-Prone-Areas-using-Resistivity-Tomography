const int relayPins[8] = {22, 23, 24, 25, 26, 27, 28, 29}; // Relay control pins
int value = 0;
float voltage;
//Processing part********************************************//
String dataString = "";
//****************************************************************************//
//For Current Sensor
//*****************************************************************************//
float sensitivity = 66.0f;
float current;
int adcValue= 0;
const int currentPin = A0;
float adcVoltage = 0.0f;
float currentValue = 0.0f;
int ARDUINO_REF = 6000;
int offsetVoltage = ARDUINO_REF / 2;
float errorCorrectionAmp = 0.0f; // Set this to use a correcting offset on readings, e.g. -0.15f;
const float alpha = 0.1f;
//*********************************************************************************//
void setup() {
  Serial.begin(115200); // Initialize serial communication

  // Initialize relay pins
  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW); // Turn off relays initially
  }
}

void loop() {
  for (int i = 0; i < 8; i++) {
    // Activate relay for electrode i
    digitalWrite(relayPins[i], HIGH);
    delay(1000); // Allow time for stabilization
    // Measure voltage
    value = analogRead(A1);
    voltage = (value * 2.9/1023)*10;
    //Processing****************************************************************************//
    dataString = String(i) + "," + String(voltage) + "," + String(voltage / current);
    //*************************************************************************************************//
    Serial.print("Current");
    doReadAlphaFilter();
    //Current********************************************************************************//
    //Print Statements(OUTPUT)***********************************************************************//
    Serial.print("Electrode No.=");
    Serial.println(i);
    Serial.print("Voltage= ");
    Serial.println(voltage);
    Serial.print("Resistivity");
    Serial.println(voltage/current);
    Serial.println(dataString);
    Serial.println("**********************************************");
    // Deactivate relay
    digitalWrite(relayPins[i], LOW);
    delay(1000); // Delay before switching to next electrode
  }

  // Add any additional logic or data processing here
}
void doReadAlphaFilter() {
   float average = 0.0f, previousValue = 0.0f;

   analogRead(currentPin); // Dummy read, doesn't help

   for(int j = 0; j < 1000; j++) {
      adcValue = analogRead(currentPin);
      adcVoltage = (adcValue / 1024.0f) * ARDUINO_REF;
      currentValue = (((float)(adcVoltage - offsetVoltage)) / sensitivity);

      if (j>0)
        currentValue = alpha * currentValue + (1-alpha) * previousValue;

      average += currentValue;
      previousValue = currentValue;
   }
   current = average / 1000.0f + errorCorrectionAmp;
   Serial.println(average / 1000.0f + errorCorrectionAmp, 2);
}
