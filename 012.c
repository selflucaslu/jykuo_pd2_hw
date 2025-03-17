#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// 檢查字元是否為英文字母或底線
int isValidChar(char c) {
    return ( (c >= 'a' && c <= 'z') ||
             (c >= 'A' && c <= 'Z') ||
             (c == '_') );
}

// 使用指標反轉字串
void reverseString(char *str) {
    char *start = str;
    char *end   = str + strlen(str) - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

// 計算不含空白的長度
int lengthWithoutSpace(const char *str) {
    int count = 0;
    while (*str) {
        if (*str != ' ')
            count++;
        str++;
    }
    return count;
}

int main(void) {
    char str1[200], str2[200];

    // 讀取第一個字串
    if (!fgets(str1, sizeof(str1), stdin)) {
        printf("Error\n");
        return 0;
    }
    // 移除換行符號
    str1[strcspn(str1, "\n")] = '\0';

    // 讀取第二個字串
    if (!fgets(str2, sizeof(str2), stdin)) {
        printf("Error\n");
        return 0;
    }
    // 移除換行符號
    str2[strcspn(str2, "\n")] = '\0';

    // 檢查 str1 長度是否足以檢查首尾
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len1 == 0 || len2 == 0) {
        printf("Error\n");
        return 0;
    }

    // 檢查 str1 首尾字元
    if (!isValidChar(str1[0]) || !isValidChar(str1[len1 - 1])) {
        printf("Error\n");
        return 0;
    }
    // 檢查 str2 首尾字元
    if (!isValidChar(str2[0]) || !isValidChar(str2[len2 - 1])) {
        printf("Error\n");
        return 0;
    }

    // 暫存原始字串(因為之後要反轉後還要輸出原字串)
    char original1[200], original2[200];
    strcpy(original1, str1);
    strcpy(original2, str2);

    // 反轉字串
    reverseString(str1);
    reverseString(str2);

    // 輸出反轉後的字串
    printf("%s\n", str1);
    printf("%s\n", str2);

    // 比較不含空白字元的長度
    int length1 = lengthWithoutSpace(original1);
    int length2 = lengthWithoutSpace(original2);

    // 若第一個字串長度 >= 第二個字串長度，印第一個；否則印第二個
    if (length1 >= length2) {
        printf("%s\n", original1);
    } else {
        printf("%s\n", original2);
    }

    return 0;
}
