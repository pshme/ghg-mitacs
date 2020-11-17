#define mqPin 6
#define targetGas "CO"
#define gasRes 27.0
#define totalRes 4.97
#define terminalRes 3.3

const float n = 1/0.542; //power
const float m = 0.8239; //numerator

void setup() 
{
  Serial.begin(115200); //serial baud rate
  pinMode(mqPin, INPUT); //analog input
  analogReadResolution(12); //using 12-bit ADC resolution (4096)
}
 
void loop() 
{  
  float sensor_voltage; //sensor voltage
  float rs_target_gas; //sensor resistance for target gas 
  float ratio; //rs/r0
  float sensor_analog = analogRead(mqPin); //reading analog value from pin 0
  sensor_voltage = sensor_analog*(3.3/4095.0); //ADC conversion
  rs_target_gas = (5.0*terminalRes/sensor_voltage)-totalRes; //calculation of resistance using voltage divider
  ratio = rs_target_gas/gasRes; //ratio rs/r0

  double ppm = pow((m/ratio),n);

  //Serial.println(sensor_analog); //debugging for analog sensor value
  //Serial.println(sensor_voltage); //debugging for sensor voltage value
  //Serial.println(ratio); // debugging for ratio value
  Serial.print(targetGas);
  Serial.print(" ppm: ");
  Serial.println(ppm);
  delay(500);
}
  
