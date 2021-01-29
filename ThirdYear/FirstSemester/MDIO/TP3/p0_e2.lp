/* Objective function */
min: tf;

/* Variable bounds */
t1 >= t0 + 4;
t2 >= t1 + 6;
t2 >= t4 + 9;
t4 >= t0 + 4;
t4 >= t7 + 6;
t5 >= t4 + 9;
t5 >= t7 + 6;
t5 >= t10 + 8;
t6 >= ti + 0;
t0 >= ti + 0;
t7 >= t6 + 5;
t9 >= t7 + 6;
t9 >= t10 + 8;
t9 >= t11 + 7;
t10 >= t6 + 5;
t11 >= t10 + 8;
tf >= t2 + 7;
tf >= t5 + 4;
tf >= t9 + 2;
int ti, tf, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;