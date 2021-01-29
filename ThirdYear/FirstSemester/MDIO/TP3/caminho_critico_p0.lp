/* Objective function */
max: 4x01 + 4x04 + 6x12 + 7x2f + 9x42 + 9x45 + 4x5f + 5x610 + 5x67 + 6x74 +
     6x75 + 6x79 + 2x9f + 8x105 + 8x1011 + 7x119;

/* Variable bounds */
v_i: 1 = xi0 + xi6;
v_0: xi0 = x01 + x04;
v_1: x01 = x12;
v_2: x12 + x42 = x2f;
v_4: x04 + x74 = x42 + x45;
v_5: x45 + x75 + x105 = x5f;
v_6: xi6 = x67 + x610;
v_7: x67 = x74 + x75 + x79;
v_9: x79 + x109 + x119 = x9f;
v_10: x610 = x105 + x109 + x1011;
v_11: x1011 = x119;
v_f: x2f + x5f + x9f = 1;
int xi0, xi6, x01, x04, x12, x2f, x42, x45, x5f, x610, x67, x74, x75, x79, x9f, x195, x1011, x119;