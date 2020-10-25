#define mqPin 0
#define targetGas "O3"
#define gasRes 4750
#define totalRes 1530
#define terminalRes 1000

const float n = 1/0.4493; //power
const float m = 0.3579; //denominator

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

  double ppb = pow((ratio/m),n);

  //Serial.println(sensor_analog); //debugging for analog sensor value
  //Serial.println(sensor_voltage); //debugging for sensor voltage value
  //Serial.println(ratio); // debugging for ratio value
  Serial.print(targetGas);
  Serial.print(" ppb: ");
  Serial.println(ppb);
  delay(500);
}
  
