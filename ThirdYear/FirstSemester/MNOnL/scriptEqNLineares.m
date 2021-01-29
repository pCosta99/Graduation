%Resolu��o de equa��es n�o lineares (M�todo de Newton)
%fsolve('fun', p.inicial) ou fsolve(@fun, p.inicial)
x = fsolve('fun', 6);
%Para obter mais detalhes:
[X, FVAL, EXITFLAG, OUTPUT] = fsolve(@fun,6);
%Para obter o gr�fico da fun��o fun e sobrepor uma grelha no mesmo podemos
%correr:
ezplot('fun');
grid;
%Para obter gr�fico 3d de uma fun��o:
ezsurfc(@peaks);
%Para verificar as op��es que o fsolve est� a utilizar por default:
optimset('fsolve');
%Criar um novo conjunto de defini��es
% Para estes casos : optimset('tolfun', epsolon2, 'tolX', epsolon1)
op = optimset('tolfun', 1e-3, 'tolX', 1e-3);
%Correr fsolve com as op��es criadas anteriormente:
[X, FVAL, EXITFLAG, OUTPUT] = fsolve('fun', 6, op);