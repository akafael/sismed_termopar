function plotRawTemp(serialPort,axisRaw,axisFilter)
%PLOTRAWTEMP Plot Raw Input
%   Detailed explanation goes here

    % Init Variables
    rawMeasure = [0 0 0]
    buffersize = 300;
    x = zeros(buffersize,3);
    i = 1;

    % Remove First Line
    fgetl(serialPort);
    
    while ((~isempty(serialPort))&& strcmp(serialPort.Status,'open'))
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
        x(i,1:3) = rawMeasure;
        x(i,4:6) = filteredData;
        %i = mod(i+1,buffersize)+1

        % Plot
        axes(axisRaw)
        plot(x(:,2))
        hold on;
        plot(x(:,3))
        hold off;
        legend('Raw LM35','Raw MTK-01')
        
        axes(axisFilter)
        plot(x(:,4))
        hold on;
        plot(x(:,5))
        plot(x(:,6))
        hold off;
        legend('F. Média','F. Média Móvel','F. Média Móvel Pond.');
        
        drawnow;

        % Update Index
        if (i>=buffersize)
            i = 1
        else
            i = i+1
        end
    end
end

