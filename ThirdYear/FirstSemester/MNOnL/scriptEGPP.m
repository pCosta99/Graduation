%Resolução de EGPP's (Eliminação de Gauss por pivotagem parcial - matrizes)
A = [2.4 6 -2.7 5; -2.1 -2.7 5.9 -4; 3 5 -4 6; 0.9 1.9 4.7 1.8];
b = [14.6; -11.4; 14; -0.9];
%Resolução do sistema
A\b;
%Determinante de A
det(A);
%Inversa de A
inv(A);
%Seleciona primeira coluna de A
A(:,1);
%Seleciona primeira linha de A
A(1,:);