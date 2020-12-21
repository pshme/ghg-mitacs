function [crt_array] = timeCorrect(raw_array)

temp_array = [];

for i = 1:1:length(raw_array)
    temp_val = raw_array(i);
    temp_array = [temp_array temp_val temp_val];
end

crt_array = temp_array;

end
