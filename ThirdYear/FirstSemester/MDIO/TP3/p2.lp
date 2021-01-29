/* Objective function */
min: 200c10 + 600c11 + 1000c12 + 800c14 + 1600c15 + 180c16 + 1000c110 + 600c111 +
     100c20 + 300c21 + 500c22 + 400c24 + 800c25 + 90c26 + 500c210 + 300c211;

/* Restrição temporal */
tf <= 24;

/* Primeiras reduções máximas */
c10 <= 0.5;
c11 <= 1;
c12 <= 3;
c14 <= 2;
c15 <= 0.5;
c16 <= 1;
c110 <= 0.5;
c111 <= 1;

/* Restrições que forçam a condição de apenas podermos usar
as segundas reduções quando esgotamos as primeiras */

c10 >= 0.5y10;
c11 >= 1y11;
c12 >= 3y12;
c14 >= 2y14;
c15 >= 0.5y15;
c16 >= 1y16;
c110 >= 0.5y110;
c111 >= 1y111;

/* Segundas reduções máximas */
c20 <= 0.5y10;
c21 <= 1y11;
c22 <= 1y12;
c24 <= 1y14;
c25 <= 0.5y15;
c26 <= 1y16;
c210 <= 0.5y110;
c211 <= 1y111;

/* Relações de precedência */
t1 >= t0 - c10 - c20 + 4;
t2 >= t1 - c11 - c21 + 6;
t2 >= t4 - c14 - c24 + 9;
t4 >= t0 - c10 - c20 + 4;
t4 >= t7 + 6;
t5 >= t4 - c14 - c24 + 9;
t5 >= t7 + 6;
t5 >= t10 - c110 - c210 + 8;
t6 >= ti + 0;
t0 >= ti + 0;
t7 >= t6 - c16 - c26 + 5;
t9 >= t7 + 6;
t9 >= t10 - c110 - c210 + 8;
t9 >= t11 - c111 - c211 + 7;
t10 >= t6 - c16 - c26 + 5;
t11 >= t10 - c110 - c210 + 8;
tf >= t2 - c12 - c22 + 7;
tf >= t5 - c15 - c25 + 4;
tf >= t9 + 2;
int ti, tf, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11,
    c10, c11, c12, c14, c15, c16, c110, c111,
    c20, c21, c22, c24, c25, c26, c210, c211;
bin y10, y11, y12, y14, y15, y16, y110, y111;