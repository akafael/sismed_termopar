%% Gráfico da Temperatura
% @author Rafael, Camila
%

close all
clear all
clc

port = '/dev/ttyACM0';

% Perform the connection
delete(instrfind({'Port'},{port}));
serialPort=serial(port);
serialPort.BaudRate=57600;
fopen(serialPort);

% Init Variables
rawMeasure = [0 0 0]
buffersize = 300;
x = zeros(buffersize,3);
i = 1;

% Remove First Line
fgetl(serialPort);

while (~isempty(serialPort))
    % Read Data
    str = fgetl(serialPort);
    str = strtrim(str);
    str = strsplit(str, ' ');
    
    for n = 1:3
        rawMeasure(n) = str2double(str{n})
    end
    
    for n = 4:6
        filteredData(n-3) = str2double(str{n})
    end
    
    % Save Data
    x(i,:) = rawMeasure;
    %i = mod(i+1,buffersize)+1
    
    % Plot
    plot(x(:,2))
    hold on;
    plot(x(:,3))
    hold off;
    legend('Raw LM35','Raw MTK-01')
    drawnow;
    
    % Update Index
    if (i>=buffersize)
        i = 1
    else
        i = i+1
    end
end

fclose(serialPort); 
delete(serialPort);
