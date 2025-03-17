#include <stdio.h>
#include <stdlib.h>

//
// 計算兩數的最大公因數 (GCD)
//
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

//
// 將分子 (num) 與分母 (den) 簡化為最簡分數 (保留符號於分子)
//
void reduceFraction(int *num, int *den) {
    if (*den == 0) {
        // 題目假設 x1 != x2，不考慮直線垂直無法以 y=mx+b 表示的情況
        // 如果實務上需處理 x1==x2，則需另外處理
        return;
    }

    // 判斷正負號
    int sign = 1;
    if ((*num < 0 && *den > 0) || (*num > 0 && *den < 0)) {
        sign = -1;
    }

    int n = abs(*num);
    int d = abs(*den);
    int g = gcd(n, d);

    // 約分
    n /= g;
    d /= g;

    // 將正負號放回分子
    n = sign < 0 ? -n : n;

    *num = n;
    *den = d;
}

//
// 輸出分數或整數 (若分母=1, 只印分子; 否則印 "num/den")
//
void printFraction(int num, int den) {
    if (den == 1) {
        printf("%d", num);
    } else {
        printf("%d/%d", num, den);
    }
}

int main(void) {
    int N;
    scanf("%d", &N);

    while (N--) {
        int x1, y1, x2, y2;
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

        // m = (y1 - y2) / (x1 - x2)
        int m_num = y1 - y2;   // 斜率分子
        int m_den = x1 - x2;   // 斜率分母
        reduceFraction(&m_num, &m_den);

        // b = (x2*y1 - x1*y2) / (x2 - x1)
        int b_num = x2 * y1 - x1 * y2;
        int b_den = x2 - x1;
        reduceFraction(&b_num, &b_den);

        // 開始組合輸出格式： y = mx + b
        // 先處理 "y = "
        printf("y = ");

        // 特殊情況一：m = 0
        if (m_num == 0) {
            // 整條式只剩下 y = b
            // 根據題目要求，若 b>0 -> y = b；若 b<0 -> y = -b (其實就是印出負數)
            if (b_num == 0) {
                // m=0 且 b=0, 其實是 y=0
                printf("0\n");
            } else {
                // 單純印 b(可能為正或負)
                // 若 b_den=1 -> 只印b_num，否則印分數
                printFraction(b_num, b_den);
                printf("\n");
            }
            continue;
        }

        // 若 m != 0，處理斜率 m
        // 題目要求：m=1 -> x；m=-1 -> -x；否則印簡化後的分數/整數
        if (m_num == 1 && m_den == 1) {
            // m = 1
            printf("x");
        } else if (m_num == -1 && m_den == 1) {
            // m = -1
            printf("-x");
        } else {
            // 一般情況: 印 m_num/m_den，再接上 "x"
            printFraction(m_num, m_den);
            printf("x");
        }

        // 接著處理 b
        if (b_num == 0) {
            // b=0 -> 不輸出
            printf("\n");
            continue;
        }

        // b != 0: 判斷正負號，決定要印 " + " 或 " - " 或直接印負值
        // 為方便處理，把 b_num/b_den 整理成最簡，並手動輸出 +/-。
        // 先判斷 b 本身的 sign
        int sign_b = (b_num < 0) ? -1 : 1;
        if (sign_b > 0) {
            // b>0 -> 輸出 " + ..."
            printf(" + ");
        } else {
            // b<0 -> 輸出 " - ..."
            printf(" - ");
        }

        // 接著印出 b 的絕對值部分 (因為符號已經上面印了)
        int abs_num = (b_num < 0) ? -b_num : b_num;
        int abs_den = b_den; // b_den一定是正數嗎? 不一定，但 reduceFraction後通常會讓分母為正
                             // 不過為安全仍以 sign_b 處理
        // 若 b_den < 0，則要再做一次
        if (abs_den < 0) {
            abs_den = -abs_den;
        }

        // 若 abs_den=1 -> 只印 abs_num，否則印 分數
        if (abs_den == 1) {
            printf("%d\n", abs_num);
        } else {
            printf("%d/%d\n", abs_num, abs_den);
        }
    }

    return 0;
}
