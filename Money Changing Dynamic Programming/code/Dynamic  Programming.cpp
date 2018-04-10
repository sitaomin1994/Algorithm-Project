#include<stdio.h>
#include<stdlib.h>
/*
compare: 降序排列所需函数
*/
int compare(const void *a, const void *b)
{
	return *(int*)b - *(int*)a;
}
/*数组较大，放在主函数栈外以免溢出*/
/*
coins:存放给出的硬币
ans:存放答案
*/
int coins[10001], ans[10001];
/*
can[i][lm]:记录前i个硬币能否凑成lm元
is[i][lm]:记录第i个硬币有没有被用于凑成lm元
*/
bool can[10001][101], is[10001][101];
int main(){
	/*can和is中用到的i和lm*/
	int i, lm;
	/*Eva所带硬币的数量n,目标金额m*/
	int n, m;
	/*输出答案中用于for循环的变量k*/
	int k;
	/*写入所给硬币数量、目标金额和各枚硬币的金额*/
	scanf("%d %d", &n, &m);
	for (i = 1; i <= n; i++){
		scanf("%d", &coins[i]);
	}
	/*为了得到最小字典序的答案，对给出的硬币序列进行降序排列*/
	qsort(coins + 1, n, sizeof(int), compare);
	/*数组初始化为0*/
	can[10001][101] = { 0 };
	is[10001][101] = { 0 };
	/*目标金额为0时，前i个硬币总能满足*/
	for (i = 0; i <= n; i++){
		can[i][0] = 1;
	}
	/*
	判断前i个硬币能否凑出lm元：
	如果前i-1个硬币可以凑出lm-第i枚硬币的金额，那么前i个硬币可以凑出lm元，而且第i个硬币被征用了；
	如果不能，则前i个硬币能否凑出lm元取决于前i-1个硬币能否直接凑出m元。
	每次判断的结果都存放在can和is数组中
	*/
	for (i = 1; i <= n; i++){
		for (lm = coins[i]; lm <= m; lm++){
			if (can[i - 1][lm - coins[i]]){
				can[i][lm] = 1;
				is[i][lm] = 1;
			}
			else{
				can[i][lm] = can[i - 1][lm];
			}
		}
	}
	/*
	如果给出n个硬币可以凑出m元，输出答案。
	*/
	if (can[n][m]){
		int total = 0;
		while (n > 0){
			if (is[n][m]){
				ans[total] = coins[n];
				m = m - coins[n];
				n--;
				total++;
			}
			else{
				n--;
			}
		}
		for (k = 0; k < total - 1; k++){
			printf("%d ", ans[k]);
		}
		printf("%d\n", ans[total - 1]);
	}
	else{
		printf("No Solution\n");
	}

	return 0;
}

