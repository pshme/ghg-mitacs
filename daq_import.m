crt_pressure = timeCorrect(raw_pressure);
crt_temperature = timeCorrect(raw_temperature);
crt_humidity = timeCorrect(raw_humidity);
crt_co2 = timeCorrect(raw_co2);
crt_methane = timeCorrect(raw_methane);
crt_ammonia = timeCorrect(raw_ammonia);
crt_monoxide = timeCorrect(raw_monoxide);
crt_ozone = timeCorrect(raw_ozone);

average_scale = 60;
time = seconds(0:1:length(crt_pressure)-1);

dataGraph(time,crt_pressure,average_scale, 'Pressure', 'ppm');
dataGraph(time,crt_humidity,average_scale, 'Humidity', '%');
dataGraph(time,crt_temperature,average_scale, 'Temperature', 'C');
dataGraph(time,crt_co2,average_scale, 'CO2', 'ppm');
dataGraph(time,crt_methane,average_scale, 'Methane', 'ppm');
dataGraph(time,crt_ammonia,average_scale, 'Ammonia', 'ppm');
dataGraph(time,crt_monoxide,average_scale, 'Monoxide', 'ppm');
dataGraph(time,crt_ozone,average_scale, 'Ozone', 'ppb');

