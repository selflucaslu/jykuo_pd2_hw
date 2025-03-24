#include <stdio.h>
#include <string.h>

// 幫助函式：將隊伍名稱 A、B、C 對應到索引值 0,1,2
int getTeamIndex(const char *team) {
    if (strcmp(team, "A") == 0) return 0;
    if (strcmp(team, "B") == 0) return 1;
    return 2; // C
}

int main(void) {
    int M;
    scanf("%d", &M);

    // wins[0] 表示 A 的勝場數, wins[1] 表示 B 的勝場數, wins[2] 表示 C 的勝場數
    // scores[0] 表示 A 在所有比賽的累積總得分, 以此類推
    int wins[3] = {0, 0, 0};
    int scores[3] = {0, 0, 0};

    for(int i = 0; i < M; i++){
        char team1[2], team2[2]; // 隊伍名稱只有 A、B、C，長度足夠用 2
        scanf("%s %s", team1, team2);

        int idx1 = getTeamIndex(team1);
        int idx2 = getTeamIndex(team2);

        int sum1 = 0, sum2 = 0; // 紀錄該場比賽兩隊的總得分

        // 連讀4行，每行兩個得分
        for(int q = 0; q < 4; q++){
            int s1, s2;
            scanf("%d %d", &s1, &s2);
            sum1 += s1;
            sum2 += s2;
        }

        // 累計該隊的總得分
        scores[idx1] += sum1;
        scores[idx2] += sum2;

        // 判斷贏家，勝場數+1
        if(sum1 > sum2){
            wins[idx1]++;
        } else {
            wins[idx2]++;
        }
    }

    // 找出誰的勝場數最多
    // 題目保證「必有一隊為贏家」，故不需處理平手
    int maxWinIndex = 0;
    for(int i = 1; i < 3; i++){
        if(wins[i] > wins[maxWinIndex]){
            maxWinIndex = i;
        }
    }

    // 輸出時，需要印出隊伍名稱以及該隊的總得分
    // maxWinIndex = 0 -> "A"
    // maxWinIndex = 1 -> "B"
    // maxWinIndex = 2 -> "C"
    char teamName;
    if(maxWinIndex == 0) teamName = 'A';
    else if(maxWinIndex == 1) teamName = 'B';
    else teamName = 'C';

    printf("%c %d\n", teamName, scores[maxWinIndex]);

    return 0;
}
