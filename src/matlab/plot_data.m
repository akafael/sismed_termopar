function plot_data()
% Reads the data in a text file and plot its
% values on a graph

fileID = fopen('data.txt', 'r');
content = fscanf(fileID, '%f');
%class(content)
%disp(content);

figure;
hold on;
%grid on;
for n = 1:length(content)

	plot((0.5*(n-1)), content(n), '-ob',...
		 'MarkerSize', 2);
	% Considers 0.5s delay between each
	% new information the sensor reads
	pause(0.5);
end
hold off;
