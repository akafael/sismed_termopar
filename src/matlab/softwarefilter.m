function softwarefilter()

%-------- VARIAVEIS --------

rng(0, 'twister');
desvio = 4;
media = 0;
n = 50;
alfa = 0.5;
filtromedia = 0;
filtromediamov = 0;
filtromediaexp = 0;

% -------- FILTROS --------

figure;
hold on;

for k = 1:500
	% Cria um vetor de dados randomicos com ruído Gaussiano
	% de media zero e desvio padrao de 4 metros
	x(k) = desvio.*randn(1) + media;
	plot(k, x(k), '-ob', 'MarkerSize', 3);

	% Implementa o filtro de media
	filtromedia = (((k-1)/k)*filtromedia) + (x(k)/k);
	plot(k, filtromedia, '-xr', 'MarkerSize', 3);

	% Implementa o filtro de media movel
	if k <= n
		filtromediamov = filtromedia;
	else
		filtromediamov = mean(x(k-n:k-1)) + ((x(k) - x(k-n))/n);
	end
	plot(k, filtromediamov, '-sg', 'MarkerSize', 3);

	% Implementa o filtro de media movel ponderado exponencialmente
	filtromediaexp = alfa*filtromediaexp + ((1-alfa)*x(k));
	plot(k, filtromediaexp, '-dm', 'MarkerSize', 3);

	% Simula a espera de 5 segundo entre um dado e outro
	%pause(5);
end
grid on;
hold off;

%disp(filtromedia);
%disp(filtromediamov);
%disp(filtromediaexp);
%stats = [mean(x) std(x) var(x)]
