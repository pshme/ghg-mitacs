function [crt_array] = timeCorrect(raw_array)
% timeCorrect requires the raw data (raw_array) directly from the sensor,
% and converts the 2 second interval to 1 second by doubling each data
% point in the array. The output is the corrected array (crt_array).

temp_array = [];

for i = 1:1:length(raw_array)
    temp_val = raw_array(i);
    temp_array = [temp_array temp_val temp_val];
end

crt_array = temp_array;

end
