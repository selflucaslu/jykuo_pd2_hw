#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 計算最大公因數 (GCD)
long long gcd(long long a, long long b) {
    if (b == 0) return a > 0 ? a : -a; // gcd 要回傳正值
    return gcd(b, a % b);
}

// 將輸入的分數字串解析為分子與分母 (以不帶分數形式表示)
void parseFraction(const char *str, long long *num, long long *den) {
    // 預設值
    *num = 0;
    *den = 1;

    // 判斷是否有負號
    int isNegative = 0;
    if (str[0] == '-') {
        isNegative = 1;
        str++;  // 去掉負號, 之後再處理
    }

    // 檢查是否為帶分數形式 (例如 10(3/5)) 或只是 x/y
    // 先找 '(' 符號
    const char *pParen = strchr(str, '(');
    if (pParen) {
        // 帶分數形式: 整數部分 + (分子/分母)
        // 範例: "10(3/5)" or "2(1/2)"
        // 整數部分：str ~ pParen-1
        // 括號內部：pParen+1 ~ 尋找 ')'
        long long integerPart = 0;
        // 先解析整數部分
        {
            char temp[128];
            size_t len = pParen - str;
            strncpy(temp, str, len);
            temp[len] = '\0';
            integerPart = atoll(temp);  // 轉整數
        }

        // 解析括號內部的 "x/y"
        const char *pClose = strchr(pParen, ')');
        if (!pClose) {
            // 格式錯誤，直接給分母=0 讓後續判斷 Error
            *den = 0;
            return;
        }

        // 括號內部形如 "3/5"
        // 跳過 '('
        pParen++;
        // 取中間字串
        char fracPart[128];
        size_t length = pClose - pParen;
        strncpy(fracPart, pParen, length);
        fracPart[length] = '\0';

        // 找 '/'
        char *slashPos = strchr(fracPart, '/');
        if (!slashPos) {
            // 格式錯誤
            *den = 0;
            return;
        }
        *slashPos = '\0';
        slashPos++;

        long long subNum = atoll(fracPart);
        long long subDen = atoll(slashPos);

        // 若分母為 0，後面會直接判定 Error
        if (subDen == 0) {
            *den = 0;
            return;
        }

        // 將帶分數化為假分數: integerPart + (subNum/subDen)
        // => (integerPart * subDen + subNum) / subDen
        long long totalNum = integerPart * subDen + subNum;
        long long totalDen = subDen;

        *num = totalNum;
        *den = totalDen;
    } else {
        // 只可能是 x/y 格式 (真分數或假分數)
        // 找 '/'
        char *slashPos = strchr(str, '/');
        if (!slashPos) {
            // 格式錯誤 (沒有 '/')，可能只輸入整數？
            // 題目沒有明確說只輸入整數(如 "3")是否存在，但若只有整數
            // 也可視作 3/1
            // 先嘗試轉換整數
            long long val = atoll(str);
            *num = val;
            *den = 1;
        } else {
            // x/y
            // 先把字符串用 / 分割
            char temp[128];
            strcpy(temp, str);
            slashPos = strchr(temp, '/');
            *slashPos = '\0';
            slashPos++;

            long long x = atoll(temp);
            long long y = atoll(slashPos);

            *num = x;
            *den = y;
        }
    }

    // 若有負號，調整分子符號即可
    if (isNegative) {
        *num = -(*num);
    }
}

// 輸出分數 (最簡形式)。若錯誤(分母=0)或需帶分數或整數，這裡不做判定，
// 建議外部先判定分母是否為 0。
void printFraction(long long num, long long den) {
    // 先處理分子=0 的情形 => 輸出 0
    if (num == 0) {
        printf("0\n");
        return;
    }

    // 化為正分子正分母，額外記負號
    int isNegative = (num < 0) ^ (den < 0);
    if (num < 0) num = -num;
    if (den < 0) den = -den;

    // 先化最簡
    long long g = gcd(num, den);
    num /= g;
    den /= g;

    // 取得整數部分
    long long integerPart = num / den;
    long long remainder = num % den;

    // 輸出負號
    if (isNegative && (integerPart > 0 || remainder > 0)) {
        printf("-");
    }

    // 若有整數部分
    if (remainder == 0) {
        // 沒有小數餘分 => 為整數
        printf("%lld\n", integerPart);
    } else {
        // 有餘分 => 帶分數或真分數
        if (integerPart == 0) {
            // 純分數
            printf("%lld/%lld\n", remainder, den);
        } else {
            // 帶分數
            printf("%lld(%lld/%lld)\n", integerPart, remainder, den);
        }
    }
}

// 進行四則運算
// op 為 '+', '-', '*', '/'
void compute(long long n1, long long d1, long long n2, long long d2, char op,
             long long *rn, long long *rd, int *errorFlag) {
    *errorFlag = 0;  // 預設無錯
    // 若分母為 0, 直接Error
    if (d1 == 0 || d2 == 0) {
        *errorFlag = 1;
        return;
    }

    long long num = 0, den = 1;

    switch (op) {
        case '+':
            // n1/d1 + n2/d2 = (n1*d2 + n2*d1)/(d1*d2)
            num = n1 * d2 + n2 * d1;
            den = d1 * d2;
            break;
        case '-':
            // n1/d1 - n2/d2 = (n1*d2 - n2*d1)/(d1*d2)
            num = n1 * d2 - n2 * d1;
            den = d1 * d2;
            break;
        case '*':
            // n1/d1 * n2/d2 = (n1*n2)/(d1*d2)
            num = n1 * n2;
            den = d1 * d2;
            break;
        case '/':
            // n1/d1 ÷ n2/d2 = (n1*d2)/(d1*n2)
            // 注意若 n2=0 => 錯誤
            if (n2 == 0) {
                *errorFlag = 1;
                return;
            }
            num = n1 * d2;
            den = d1 * n2;
            break;
        default:
            // 不明運算子
            *errorFlag = 1;
            return;
    }

    // 若結果分母=0 => Error
    if (den == 0) {
        *errorFlag = 1;
        return;
    }

    // 最後回傳
    *rn = num;
    *rd = den;
}

int main(void) {
    while (1) {
        char frac1[128], frac2[128], op[8], cont[8];
        long long n1, d1, n2, d2;
        int errorFlag = 0;

        // 讀取第一個分數
        if (!fgets(frac1, sizeof(frac1), stdin)) break;
        // 移除換行符
        frac1[strcspn(frac1, "\r\n")] = '\0';

        // 讀取運算子
        if (!fgets(op, sizeof(op), stdin)) break;
        op[strcspn(op, "\r\n")] = '\0';

        // 讀取第二個分數
        if (!fgets(frac2, sizeof(frac2), stdin)) break;
        frac2[strcspn(frac2, "\r\n")] = '\0';

        // 讀取是否繼續
        if (!fgets(cont, sizeof(cont), stdin)) break;
        cont[strcspn(cont, "\r\n")] = '\0';

        // 解析分數1
        parseFraction(frac1, &n1, &d1);
        // 若分母=0 => 錯誤
        if (d1 == 0) {
            printf("Error\n");
            // 若使用者輸入 n 就結束，若 y 繼續下一輪
            if (cont[0] == 'n' || cont[0] == 'N') {
                break;
            } else {
                continue;
            }
        }

        // 解析分數2
        parseFraction(frac2, &n2, &d2);
        if (d2 == 0) {
            printf("Error\n");
            if (cont[0] == 'n' || cont[0] == 'N') {
                break;
            } else {
                continue;
            }
        }

        // 進行運算
        long long rn, rd;
        compute(n1, d1, n2, d2, op[0], &rn, &rd, &errorFlag);

        // 檢查運算是否錯誤
        if (errorFlag || rd == 0) {
            printf("Error\n");
        } else {
            // 正常 => 輸出結果 (化簡並轉格式)
            printFraction(rn, rd);
        }

        // 判斷是否繼續
        if (cont[0] == 'n' || cont[0] == 'N') {
            break;
        }
    }

    return 0;
}
