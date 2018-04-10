#include<stdio.h>
#include<stdlib.h>
/*
compare: �����������躯��
*/
int compare(const void *a, const void *b)
{
	return *(int*)b - *(int*)a;
}
/*����ϴ󣬷���������ջ���������*/
/*
coins:��Ÿ�����Ӳ��
ans:��Ŵ�
*/
int coins[10001], ans[10001];
/*
can[i][lm]:��¼ǰi��Ӳ���ܷ�ճ�lmԪ
is[i][lm]:��¼��i��Ӳ����û�б����ڴճ�lmԪ
*/
bool can[10001][101], is[10001][101];
int main(){
	/*can��is���õ���i��lm*/
	int i, lm;
	/*Eva����Ӳ�ҵ�����n,Ŀ����m*/
	int n, m;
	/*�����������forѭ���ı���k*/
	int k;
	/*д������Ӳ��������Ŀ����͸�öӲ�ҵĽ��*/
	scanf("%d %d", &n, &m);
	for (i = 1; i <= n; i++){
		scanf("%d", &coins[i]);
	}
	/*Ϊ�˵õ���С�ֵ���Ĵ𰸣��Ը�����Ӳ�����н��н�������*/
	qsort(coins + 1, n, sizeof(int), compare);
	/*�����ʼ��Ϊ0*/
	can[10001][101] = { 0 };
	is[10001][101] = { 0 };
	/*Ŀ����Ϊ0ʱ��ǰi��Ӳ����������*/
	for (i = 0; i <= n; i++){
		can[i][0] = 1;
	}
	/*
	�ж�ǰi��Ӳ���ܷ�ճ�lmԪ��
	���ǰi-1��Ӳ�ҿ��Դճ�lm-��iöӲ�ҵĽ���ôǰi��Ӳ�ҿ��Դճ�lmԪ�����ҵ�i��Ӳ�ұ������ˣ�
	������ܣ���ǰi��Ӳ���ܷ�ճ�lmԪȡ����ǰi-1��Ӳ���ܷ�ֱ�Ӵճ�mԪ��
	ÿ���жϵĽ���������can��is������
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
	�������n��Ӳ�ҿ��Դճ�mԪ������𰸡�
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

