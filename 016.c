#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN 110  // 預留一些空間，處理最多100位數字及字串結尾

// 移除字串前置的 '0'，同時保留單個 '0'（若結果本身為 0）
void removeLeadingZeros(char *str) {
    int i = 0, j = 0;
    // 跳過所有前置 '0'
    while (str[i] == '0') {
        i++;
    }
    // 如果整個字串都是 '0'，則保留一個 '0'
    if (str[i] == '\0') {
        // 保留一個 '0'
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    // 將非前置 '0' 的部分往前搬移
    while (str[i] != '\0') {
        str[j++] = str[i++];
    }
    str[j] = '\0';
}

// 比較兩個「不帶符號」的大數字字串的大小 (只比較絕對值)
// 回傳值：
//    0 -> abs(a) == abs(b)
//    1 -> abs(a) > abs(b)
//   -1 -> abs(a) < abs(b)
int compareAbs(const char *a, const char *b) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    if (lenA > lenB) return 1;
    if (lenA < lenB) return -1;
    // 位數相同時，逐位比較
    return strcmp(a, b) == 0 ? 0 : (strcmp(a, b) > 0 ? 1 : -1);
}

// 字串大數加法: res = a + b (a, b皆為「不帶負號」的字串)
// 傳入前須確保 a 與 b 都已移除前導零
// 最長可達 (MAX_LEN-1) 位數，但要留意需要的空間
void stringAdd(const char *a, const char *b, char *res) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    int carry = 0;
    int idxA = lenA - 1, idxB = lenB - 1;
    int idxRes = 0;
    char tempRes[MAX_LEN * 2] = {0};

    // 從尾巴開始加
    while (idxA >= 0 || idxB >= 0 || carry) {
        int x = (idxA >= 0) ? (a[idxA] - '0') : 0;
        int y = (idxB >= 0) ? (b[idxB] - '0') : 0;
        int sum = x + y + carry;
        carry = sum / 10;
        sum = sum % 10;
        tempRes[idxRes++] = (char)(sum + '0');
        if (idxA >= 0) idxA--;
        if (idxB >= 0) idxB--;
    }

    // 反轉 tempRes
    for (int i = 0; i < idxRes; i++) {
        res[i] = tempRes[idxRes - 1 - i];
    }
    res[idxRes] = '\0';
}

// 字串大數減法: res = a - b (假設 a >= b, 且皆為「不帶負號」字串)
// 傳入前須確保 a >= b
void stringSub(const char *a, const char *b, char *res) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    int borrow = 0;
    int idxA = lenA - 1, idxB = lenB - 1;
    int idxRes = 0;
    char tempRes[MAX_LEN * 2] = {0};

    while (idxA >= 0 || idxB >= 0) {
        int x = (idxA >= 0) ? (a[idxA] - '0') : 0;
        int y = (idxB >= 0) ? (b[idxB] - '0') : 0;
        int diff = x - y - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        tempRes[idxRes++] = (char)(diff + '0');
        if (idxA >= 0) idxA--;
        if (idxB >= 0) idxB--;
    }

    // 反轉 tempRes
    for (int i = 0; i < idxRes; i++) {
        res[i] = tempRes[idxRes - 1 - i];
    }
    res[idxRes] = '\0';
    removeLeadingZeros(res);
}

// 字串大數乘法: res = a * b (a, b 皆為「不帶負號」字串)
// 最多可達 200 位數 (若 a,b 各 100 位數)
void stringMul(const char *a, const char *b, char *res) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    // 暫存每一位的結果
    int *temp = (int *)calloc(lenA + lenB, sizeof(int));
    // 從尾部開始逐位乘法
    for (int i = lenA - 1; i >= 0; i--) {
        int carry = 0;
        int x = a[i] - '0';
        for (int j = lenB - 1; j >= 0; j--) {
            int y = b[j] - '0';
            int sum = temp[i + j + 1] + x * y + carry;
            temp[i + j + 1] = sum % 10;
            carry = sum / 10;
        }
        temp[i] += carry;
    }
    // 轉成字串
    int idx = 0;
    while (idx < (lenA + lenB - 1) && temp[idx] == 0) {
        idx++;
    }
    int pos = 0;
    while (idx < lenA + lenB) {
        res[pos++] = (char)(temp[idx++] + '0');
    }
    res[pos] = '\0';
    free(temp);

    // 如果都被清空，就要放 '0'
    if (strlen(res) == 0) {
        strcpy(res, "0");
    }
}

// 讀入字串後，解析出正負號與真正的數字部分
// 返回值: +1 -> 正數, -1 -> 負數
int parseNumber(char *input, char *absPart) {
    int sign = 1;
    int startIndex = 0;

    // 若第一個字元是負號
    if (input[0] == '-') {
        sign = -1;
        startIndex = 1;
    }
    // 若第一個字元是正號
    else if (input[0] == '+') {
        sign = 1;
        startIndex = 1;
    }

    // 複製不含正負號的數字部分到 absPart
    strcpy(absPart, input + startIndex);

    // 移除 absPart 的前置零
    removeLeadingZeros(absPart);

    // 如果 absPart 為 "0"，則該數字即為 0，視為正數
    if (strcmp(absPart, "0") == 0) {
        sign = 1;
    }

    return sign;
}

int main(void) {
    char num1[MAX_LEN], num2[MAX_LEN];
    char a[MAX_LEN], b[MAX_LEN]; // 只存絕對值
    int signA, signB;

    // 讀入兩行
    scanf("%s", num1);
    scanf("%s", num2);

    // 解析兩個大數，分離出符號與絕對值
    signA = parseNumber(num1, a);
    signB = parseNumber(num2, b);

    // ========== 求和 ==========
    // sum = a (+/-) b
    // case1: 符號相同 -> abs(a) + abs(b)，符號同 signA
    // case2: 符號不同 -> abs(a) - abs(b)，符號取絕對值較大者
    char sumRes[MAX_LEN * 2];
    if (signA == signB) {
        // 同號 -> 直接做加法
        stringAdd(a, b, sumRes);
        // 決定正負號
        if (strcmp(sumRes, "0") == 0) {
            // 結果是 0，不帶負號
            printf("0\n");
        } else {
            if (signA < 0) {
                printf("-%s\n", sumRes);
            } else {
                printf("%s\n", sumRes);
            }
        }
    } else {
        // 異號 -> 判斷誰大誰小，做減法
        int cmp = compareAbs(a, b);
        if (cmp == 0) {
            // abs(a) == abs(b)
            printf("0\n");
        } else if (cmp > 0) {
            // abs(a) > abs(b) => 結果符號跟 a 相同
            stringSub(a, b, sumRes);
            if (signA < 0) {
                printf("-%s\n", sumRes);
            } else {
                printf("%s\n", sumRes);
            }
        } else {
            // abs(a) < abs(b) => 結果符號跟 b 相反
            stringSub(b, a, sumRes);
            if (signB < 0) {
                // 原本 b 為負 -> sumRes 需帶負號
                printf("-%s\n", sumRes);
            } else {
                // 原本 b 為正 -> sumRes 為正
                printf("%s\n", sumRes);
            }
        }
    }

    // ========== 相減 (num1 - num2) ==========
    // 令 diff = a - b (考慮實際 signA, signB)
    // num1 - num2 = (signA * abs(a)) - (signB * abs(b))
    // 轉換：= signA * abs(a) + (-signB) * abs(b)
    // 就跟上面加法的概念類似，只不過 b 的 sign 變成 -signB
    // 所以：若 signA == -signB -> abs(a)+abs(b) 否則做減法...
    char diffRes[MAX_LEN * 2];
    int newSignB = -signB;  // 第二個數換負號
    if (signA == newSignB) {
        // 同號 -> 直接做加法 (abs(a) + abs(b))，帶上 signA
        stringAdd(a, b, diffRes);
        if (strcmp(diffRes, "0") == 0) {
            printf("0\n");
        } else {
            if (signA < 0) {
                printf("-%s\n", diffRes);
            } else {
                printf("%s\n", diffRes);
            }
        }
    } else {
        // 異號 -> 做減法
        int cmp = compareAbs(a, b);
        if (cmp == 0) {
            // abs(a) == abs(b)
            printf("0\n");
        } else if (cmp > 0) {
            // a 大 -> 結果符號跟 a 相同
            stringSub(a, b, diffRes);
            if (signA < 0) {
                printf("-%s\n", diffRes);
            } else {
                printf("%s\n", diffRes);
            }
        } else {
            // b 大 -> 結果符號跟 b 相反
            stringSub(b, a, diffRes);
            // 但這時比較的是 abs(b) - abs(a)
            // 真正的符號要考慮 ( -signB ) ?
            // 其實可以直接想: num1 - num2 = signA*a - signB*b
            // 若 abs(b) > abs(a), 則整體符號 = -signB
            // 不過我們要的是 b 的絕對值 -> diffRes
            // 所以符號是 (-signB)?
            // signB 若原本是 +, 這裡就變成 -, 反之亦然
            // 簡化：(-signB) == signB * -1
            if (newSignB < 0) {
                printf("-%s\n", diffRes);
            } else {
                printf("%s\n", diffRes);
            }
        }
    }

    // ========== 相乘 ==========
    // product = (signA * abs(a)) * (signB * abs(b))
    //         = (signA * signB) * (abs(a) * abs(b))
    char mulRes[MAX_LEN * 2];
    stringMul(a, b, mulRes);
    // 判斷結果是否為 0
    if (strcmp(mulRes, "0") == 0) {
        // 0
        printf("0\n");
    } else {
        // 若 signA 與 signB 不同，則結果為負
        if ((signA * signB) < 0) {
            printf("-%s\n", mulRes);
        } else {
            printf("%s\n", mulRes);
        }
    }

    return 0;
}
