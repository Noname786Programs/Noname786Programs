#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define ul unsigned int
#define ull unsigned long long

#define rotateleft(x, n) (((x) << (n)) ^ ((x) >> (32 - n)))
#define rotateright(x, n) (((x) >> (n)) ^ ((x) << (32 - n)))

#define update(a, b, n) ((rotateleft((a) ^ (b), (n))))
#define myrand32 ((ul)(4294967296.0 * drand48()))

ull MOD = 4294967296;

void initializeR(ul *x)
{
        int i;
        for (i = 0; i < 16; i++)
        {
                x[i] = myrand32;
        }
        x[0] = 0x61707865;
        x[1] = 0x3320646e;
        x[2] = 0x79622d32;
        x[3] = 0x6b206574;
}

void copystate(ul *x1, ul *x)
{
        int i;
        for (i = 0; i < 16; i++)
        {
                x1[i] = x[i];
        }
}

void qr(ul *x0, ul *x1, ul *x2, ul *x3)
{
        ul z0, z1, z2, z3;

        z0 = *x0 + *x1;
        z3 = update(*x3, z0, 16);
        z2 = *x2 + z3;
        z1 = update(*x1, z2, 12);
        z0 = z0 + z1;
        z3 = update(z3, z0, 8);
        z2 = z2 + z3;
        z1 = update(z1, z2, 7);

        *x0 = z0, *x1 = z1, *x2 = z2, *x3 = z3;
}

void roundodd(ul *x)
{
        qr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
        qr(&(x[1]), &(x[5]), &(x[9]), &(x[13]));
        qr(&(x[2]), &(x[6]), &(x[10]), &(x[14]));
        qr(&(x[3]), &(x[7]), &(x[11]), &(x[15]));
}

void roundeven(ul *x)
{
        qr(&(x[0]), &(x[5]), &(x[10]), &(x[15]));
        qr(&(x[1]), &(x[6]), &(x[11]), &(x[12]));
        qr(&(x[2]), &(x[7]), &(x[8]), &(x[13]));
        qr(&(x[3]), &(x[4]), &(x[9]), &(x[14]));
}

void inqr(ul *x0, ul *x1, ul *x2, ul *x3)
{
        ul p, q, r, s;

        p = *x0, q = *x1, r = *x2, s = *x3;

        q = rotateright(q, 7);
        q ^= r;
        r -= s;
        s = rotateright(s, 8);
        s ^= p;
        p -= q;
        q = rotateright(q, 12);
        q ^= r;
        r -= s;
        s = rotateright(s, 16);
        s ^= p;
        p -= q;

        *x0 = p, *x1 = q, *x2 = r, *x3 = s;
}

void inroundo(ul *x)
{
        inqr(&(x[3]), &(x[7]), &(x[11]), &(x[15]));
        inqr(&(x[2]), &(x[6]), &(x[10]), &(x[14]));
        inqr(&(x[1]), &(x[5]), &(x[9]), &(x[13]));
        inqr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
}

void inrounde(ul *x)
{
        inqr(&(x[3]), &(x[4]), &(x[9]), &(x[14]));
        inqr(&(x[2]), &(x[7]), &(x[8]), &(x[13]));
        inqr(&(x[1]), &(x[6]), &(x[11]), &(x[12]));
        inqr(&(x[0]), &(x[5]), &(x[10]), &(x[15]));
}

int main()
{
        ul x[16], x1[16], x0[16], x01[16], z1[16], z2[16], diff, diff1, pattern, pt;
        int i, j, j3, j4,

            A[] = {31, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 0, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 95, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 1, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 191, 218, 219, 220, 221, 222, 223, 255, 2, 3, 4, 103, 32, 104, 64, 105, 123, 106, 65, 5, 33, 244, 71, 77, 124, 125, 83, 245, 204, 224, 126, 127, 34, 6, 66, 107, 246, 225, 84, 72, 205, 78, 192, 198, 210, 247, 89, 108, 199, 67, 35, 7, 8, 226, 128, 206, 79, 85, 73, 193, 109, 129, 99, 90, 211, 140, 19, 20, 141, 227, 9, 200, 194, 142, 21, 110, 68, 130, 36, 74, 231, 91, 100, 86, 14, 201, 212, 207, 80, 248, 115, 190},

            A2[] = {9, 21, 36, 63, 68, 74, 79, 86, 91, 100, 110, 130, 142, 159, 194, 201, 206, 223, 227, 248, 212, 207, 80};

        int LL = 158, LL2 = 23;

        ull loop = 0;
        double cnt = 0;

        srand48(time(NULL));
        pt = (0x00000001 << 7) ^ (0x00000001 << 19);

        while (loop < 1024 * 1024 * 1024)
        {

                initializeR(x);
                copystate(x1, x);
                x1[15] = x[15] ^ pt;
                copystate(x0, x);
                copystate(x01, x1);

                for (i = 0; i < 5; i++)
                {
                        if ((i % 2) == 0)
                        {
                                roundodd(x);
                                roundodd(x1);
                        }
                        else
                        {
                                roundeven(x);
                                roundeven(x1);
                        }
                }

                diff = x[2] ^ x1[2] ^ (x[6] >> 7) ^ (x1[6] >> 7) ^ (x[6] >> 19) ^ (x1[6] >> 19) ^ (x[10] >> 12) ^ (x1[10] >> 12) ^ (x[14] >> 0) ^ (x1[14] >> 0);

                roundeven(x);
                roundeven(x1);
                roundodd(x);
                roundodd(x1);

                for (i = 0; i < 16; i++)
                {
                        z1[i] = (x[i] + x0[i]);
                        z2[i] = (x1[i] + x01[i]);
                }

                for (j = 0; j < LL; j++)
                {

                        j3 = (A[j] / 32) + 4;
                        j4 = A[j] % 32;

                        if (((x0[j3] >> j4) % 2) != 0)
                        {
                                x0[j3] = x0[j3] ^ (0x00000001 << j4);
                                x01[j3] = x01[j3] ^ (0x00000001 << j4);
                        }
                }

                for (j = 0; j < LL2; j++)
                {

                        j3 = (A2[j] / 32) + 4;
                        j4 = A2[j] % 32;

                        if (((x0[j3] >> j4) % 2) == 0)
                        {
                                x0[j3] = x0[j3] ^ (0x00000001 << j4);
                                x01[j3] = x01[j3] ^ (0x00000001 << j4);
                        }
                }

                for (i = 0; i < 16; i++)
                {
                        x[i] = z1[i] + (MOD - (x0[i]));
                        x1[i] = z2[i] + (MOD - (x01[i]));
                }

                inroundo(x);
                inroundo(x1);
                inrounde(x);
                inrounde(x1);

                pattern = 0x00000001 << 0;
                diff1 = x[2] ^ x1[2] ^ (x[6] >> 7) ^ (x1[6] >> 7) ^ (x[6] >> 19) ^ (x1[6] >> 19) ^ (x[10] >> 12) ^ (x1[10] >> 12) ^ (x[14] >> 0) ^ (x1[14] >> 0);

                if (((diff ^ diff1) & pattern) == 0)
                {
                        cnt = cnt + 1.0;
                }

                loop++;

                if (loop > 0 && loop % (1024 * 256) == 0)
                        printf(" %llu      %0.10f   \n", loop / (1024 * 256), 2 * ((cnt / loop) - 0.5));
        }
}
