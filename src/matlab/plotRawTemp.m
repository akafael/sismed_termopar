function plotRawTemp(serialPort,axisRaw,axisFilter)
%PLOTRAWTEMP Plot Raw Input
%   Detailed explanation goes here

    % Init Variables
    rawMeasure = [0 0 0]
    filteredData = [0 0 0 0]
    buffersize = 200;
    x = zeros(buffersize,7);
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

        for n = 4:7
            filteredData(n-3) = str2double(str{n})
        end

        % Save Data
        x(i,1:3) = rawMeasure;
        x(i,4:7) = filteredData;
        %i = mod(i+1,buffersize)+1

        % Plot
        axes(axisRaw)
        plot(x(:,2))
        hold on;
        plot(x(:,3))
        hold off;
        legend('Raw LM35','Raw MTK-01','Location','southeast')
        
        axes(axisFilter)
        plot(x(:,4))
        hold on;
        plot(x(:,5))
        plot(x(:,6))
        plot(x(:,7))
        hold off;
        legend('F. Média','F. Média Móvel','F. Média Móvel Pond.',...
              'F. Kalman',...
            'Location','southeast');
        
        drawnow;

        % Update Index
        if (i>=buffersize)
            i = 1
        else
            i = i+1
        end
    end
end

