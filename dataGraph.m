function dataGraph(time_frame,crt_data,smooth_factor,data_name, data_unit)

figure;
plot(time_frame,movmean(crt_data,smooth_factor),'DurationTickFormat','hh:mm:ss')
title(append(data_name,' vs. ','Time'));
xlabel('Time (hr:min:sec)');
ylabel(append(data_name,' ','(',data_unit,')'));
ylim([min(crt_data)-5,max(crt_data)+5]);
grid on
end


