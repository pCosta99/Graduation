%Integração Numérica
%Com tabela:
t = [0 10 15 25 30 48 60 70 90];
v = [0 10 30 25 10 28 40 42 30];
trapz(t,v)

%Sem tabela:
%Comando quadl/quad:
%quadl/quad('fun',a,b,tol), Exemplo: quadl/quad('x.*(1-exp(-x))+x.^3',0,10)
quadl('x.*(1-exp(-x))+x.^3',0,10);
quad('x.*(1-exp(-x))+x.^3',0,10);

%Exemplo:
% I = Integral entre 1 e 2 de 1/x
% a) Calcule I
% b) Calcule I usando 6 pontos

% a)
quad('1./x', 1, 2)
% b)
% n = 6-1 = 5 intervalos
% Criar tabela
x = 1 : 0.2 : 2;
y = 1./x;
trapz(x,y)
% Operações nesta secção são operações em matrizes: .* ./ .^
% quad > quadl