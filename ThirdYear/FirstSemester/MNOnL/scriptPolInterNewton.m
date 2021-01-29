%Polinómio interpolador de Newton
% Introduzir todos os valores de x
x = [1,3,5,7,20];
% Introduzir todos os valores de f
f = [800,2310,3090,3940,8000];
% polyfit(Valores de x dentro da gama selecionada, Valores de f dentro da
% gama selecionada, grau do polinómio)

% Para selecionar a gama, precisamos de escolher os n+1 valores mais
% próximos daquele que queremos calcular (pelo menos um tem de estar à
% direita do ponto e pelo menos um à esquerda)(-6)/(
p3 = polyfit(x(2:5),f(2:5),3);
% p3 = -0.95664 23.09955 252.07881 1371.69683
% Polinómio = -0.95664 * (x^3) + 23.09955 * (x^2) + 252.07881 * x +
% 1371.69683

%polyval(pn, ponto inicial)
polyval(p3,10)

