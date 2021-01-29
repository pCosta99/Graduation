%Splines
% Introduzir valores de x e de f (todos)
x=[5 5.1 5.2 5.3 5.4 5.5 5.6 5.7 5.8 5.9 6];
f=[0.0639 0.08 0.0988 0.1203 0.1442 0.1714 0.201 0.2331 0.2673 0.3036 0.3414];
%Para determinar splines, usa-se o comando spline
s3 = spline(x,f);
s = s3.coefs;
%Para determinar o valor da spline num ponto, usa-se o comando spline, mas
%indicando esse ponto como terceiro argumento de entrada.
%spline(valores de x, valores de f + declives nos extremos (se existirem), ponto inicial)
%Exemplo : xx = spline(x, [a f b], p.inicial)
xx = spline(x,f,5.45);

%Para se usar uma spline cúbica completa, têm se se reservar o segundo e penúltimo
%ponto, que não serão usados na spline. No entanto, serão usados para determinar uma
%aproximação às derivadas nos extremos.

d0 = (f(2)-f(1))/(x(2)-x(1));
%dn = (f(n)-f(n-1))/(x(n)-x(n-1));
dn = (f(11)-f(10))/(x(11)-x(10));
s3 = spline(x([1,3:9,11]),[d0 f([1,3:9,11]) dn]);
s = s3.coefs;

%Para determinar o valor da spline num ponto:
 xx = spline(x([1,3:9,11]),[d0 f([1,3:9,11]) dn],5.45);
 
%Para descobrir o segmento da spline:
%Ver qual o segmento do valor inicial. Esse valor indica a linha a escolher
%de s3.coefs pelo qual calcularemos o segmento da spline.
% Neste exemplo era necessário forçar um declive de -2 no extremo inferior
% e 3 no extremo superior (o que justifica [-2 f_xi 3])
xi = [1.5 2.0 2.2 3.0 3.8 4.0];
f_xi = [4.9 3.3 3.0 2.0 1.75 1.5];
s3 = spline(xi, [-2 f_xi 3]);
s = s3.coefs;
s(3,:)
%A linha 3 contém os valores para o polinómio. Note-se que é preciso obter
%o ponto inicial do segmento em causa (neste caso, o segmento 3) e
%utilizá-lo em todos os valores de X (neste caso, o ponto é 2.2), da seguinte forma:
%s(3,:) = 1.681348086453748  -1.330162444933926  -1.261932819383257   3.000000000000000
%Para X = 2.5 temos que:
%s3(i=3;X) = (1.681348086453748 * (X - 2.2)^3) + (-1.330162444933926 * (X -
%2.2)^2) + (-1.261932819383257 * (X - 2.2)) + (3.000000000000000)
