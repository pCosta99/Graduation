%Resolução de equações não lineares (Método de Newton)
%fsolve('fun', p.inicial) ou fsolve(@fun, p.inicial)
x = fsolve('fun', 6);
%Para obter mais detalhes:
[X, FVAL, EXITFLAG, OUTPUT] = fsolve(@fun,6);
%Para obter o gráfico da função fun e sobrepor uma grelha no mesmo podemos
%correr:
ezplot('fun');
grid;
%Para obter gráfico 3d de uma função:
ezsurfc(@peaks);
%Para verificar as opções que o fsolve está a utilizar por default:
optimset('fsolve');
%Criar um novo conjunto de definições
% Para estes casos : optimset('tolfun', epsolon2, 'tolX', epsolon1)
op = optimset('tolfun', 1e-3, 'tolX', 1e-3);
%Correr fsolve com as opções criadas anteriormente:
[X, FVAL, EXITFLAG, OUTPUT] = fsolve('fun', 6, op);