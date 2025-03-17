#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 將單一字元轉換成對應的數字 (0 ~ 35)
int charToValue(char c) {
    // 若是 0~9
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    // 若是 A~Z
    else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    // 若是 a~z (題目並未特別指名可能輸入小寫，但可以備用)
    else if (c >= 'a' && c <= 'z') {
        return c - 'a' + 10;
    }
    // 不合法字元
    else {
        return -1; 
    }
}

// 將數字 (0 ~ 35) 轉回對應字元
char valueToChar(int val) {
    // 0~9
    if (val >= 0 && val <= 9) {
        return (char)(val + '0');
    }
    // 10~35 -> A~Z
    else {
        return (char)((val - 10) + 'A');
    }
}

// 將 M 進制字串轉為 10 進制 (使用指標走訪字串)
unsigned long long convertToDecimal(const char *numStr, int base) {
    unsigned long long decimalValue = 0ULL;
    const char *p = numStr;  // 使用指標指向字串開頭

    while (*p != '\0') {
        int digit = charToValue(*p);
        decimalValue = decimalValue * base + digit;
        p++;  // 指標往後移動
    }
    return decimalValue;
}

// 將 10 進制的數值轉為 N 進制字串 (result 為輸出字串)
void convertFromDecimal(unsigned long long decimalValue, int base, char *result) {
    // 若原本就是 0，直接回傳 "0"
    if (decimalValue == 0ULL) {
        result[0] = '0';
        result[1] = '\0';
        return;
    }

    // 暫存轉換結果（倒序）
    char buffer[70];  // 最多 64 bit，base=2 時需要 64 位元，再加結束字元即可
    int index = 0;

    // 不斷取餘數
    while (decimalValue > 0ULL) {
        unsigned long long remainder = decimalValue % base;
        decimalValue /= base;
        buffer[index++] = valueToChar((int)remainder);
    }

    // 反轉（因為上面存的是從低位到高位）
    // 寫進 result
    int i = 0;
    index--;  // 讓 index 指向最後一個有效字元
    while (index >= 0) {
        result[i++] = buffer[index--];
    }
    result[i] = '\0'; // 字串結尾
}

int main(void) {
    int M, N;
    char numStr[50];  // 存輸入的 M 進制數字

    // 輸入
    scanf("%d", &M);      // 原本的進制
    scanf("%s", numStr);  // M 進制數字（以字串表示）
    scanf("%d", &N);      // 要轉換的進制

    // 先把 M 進制轉為 10 進制
    unsigned long long decimalValue = convertToDecimal(numStr, M);

    // 再把 10 進制轉為 N 進制字串
    char result[70];
    convertFromDecimal(decimalValue, N, result);

    // 輸出結果
    printf("%s\n", result);

    return 0;
}
