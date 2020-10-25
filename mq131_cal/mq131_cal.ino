#define mqPin 0
#define airConvert 1
#define totalRes 1530
#define terminalRes 1000

void setup() 
{
  Serial.begin(115200); //serial baud rate
  pinMode(mqPin, INPUT); //analog input
  analogReadResolution(12); //using 12-bit ADC resolution (4096)
}
 
void loop() 
{ 
  float sensor_voltage = 0; //sensor voltage
  float rs_air = 0; //sensor resistance in air given by graph
  float r0_air = 0; //sensor resistance in air in ambient environment
  float sensor_analog = 0; //analog sensor reading
  
  //take 500 analog samples, then average them
  for(int x = 0 ; x < 500 ; x++)
  {
    sensor_analog = sensor_analog + analogRead(mqPin);
  }
  sensor_analog = sensor_analog/500.0; //take the average of analog values 
  sensor_voltage = sensor_analog*(3.3/4095.0); //converting analog value to voltage using ADC conversion 
  rs_air = (5.0*terminalRes/sensor_voltage)-totalRes; //calculate rs in air using a voltage divider
  r0_air = rs_air/airConvert; //calculate r0 in air
  
  //Serial.println(sensor_analog); //debugging for analog sensor value
  //Serial.println(sensor_voltage); //debugging for sensor voltage value
  Serial.print("R0 = ");
  Serial.println(r0_air);
  delay(500);
}
