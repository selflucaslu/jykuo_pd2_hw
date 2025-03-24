#include <stdio.h>
#include <string.h>

int main(void) {
    int m, arr[20], n = 0;          // m分散度, arr[]序列, n為讀入長度
    int subs[400][10], c = 0;       // subs存所有合格子序列, c為計數
    scanf("%d", &m);
    // 讀入整數序列（長度最大20）
    while (scanf("%d", &arr[n]) == 1) n++;

    // 尋找所有長度為m、不含重複數字的連續子序列
    for (int i = 0; i <= n - m; i++) {
        char used[10] = {0};        // 用於檢查0~9是否重複
        int flag = 1;
        // 檢查該段子序列有無重複
        for (int j = 0; j < m; j++) {
            if (used[arr[i + j]]) {
                flag = 0;
                break;
            }
            used[arr[i + j]] = 1;
        }
        // 若無重複就存下來
        if (flag) {
            for (int j = 0; j < m; j++) {
                subs[c][j] = arr[i + j];
            }
            c++;
        }
    }

    // 第一行：所有符合條件子序列（含重複者）的總數
    printf("%d\n", c);

    // 對 subs 做字典序排序 (bubble sort 簡易實作)
    for (int i = 0; i < c; i++) {
        for (int j = i + 1; j < c; j++) {
            for (int k = 0; k < m; k++) {
                // 一旦發現 subs[i] > subs[j]，就整列交換
                if (subs[i][k] < subs[j][k]) break;
                if (subs[i][k] > subs[j][k]) {
                    int temp[10];
                    memcpy(temp, subs[i], sizeof(temp));
                    memcpy(subs[i], subs[j], sizeof(temp));
                    memcpy(subs[j], temp,      sizeof(temp));
                    break;
                }
            }
        }
    }

    // 輸出去重後的結果
    for (int i = 0; i < c; i++) {
        // 若與上一個相同則跳過
        if (i > 0 && memcmp(subs[i], subs[i-1], m * sizeof(int)) == 0) continue;
        // 印出該子序列
        for (int j = 0; j < m; j++) {
            printf("%d%s", subs[i][j], (j < m-1) ? " " : "\n");
        }
    }

    return 0;
}
