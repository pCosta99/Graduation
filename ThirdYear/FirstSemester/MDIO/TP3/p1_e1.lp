/* Objective function */
min: tf;

/* Restrições de precedência */
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

/* Restrições de não-simultaneidade */
/* Entre 1 e 7 */
t1 + 6 <= t7 + 100 - 100y17;
t7 + 6 <= t1 + 100y17;

/* Entre 7 e 10 */
t7 + 6 <= t10 + 100 - 100y710;
t10 + 8 <= t7 + 100y710;

/* Entre 1 e 10 */
t1 + 6 <= t10 + 100 - 100y110;
t10 + 8 <= t1 + 100y110;

int ti, tf, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;
bin y17, y710, y110;