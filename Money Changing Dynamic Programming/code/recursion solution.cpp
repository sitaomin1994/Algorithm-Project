#include <iostream>
#include <algorithm> 
using namespace std;
int N,M;                         //N-the total number of coins, M-the amount of money Eva has to pay
int *ValueOfCoins=new int[N];    //ValueOfCoins--store the value of all coins eva has
int OutPutPoint=0;              //OutPutPoint--to record the outputarray head 
/*check whether eva can pay the money and record coins eva uses to pay*/
bool calCoins(int n,int m,int *output){
	bool flag;
	if(n==0){
	   if(m==0)                 //if n=0 m!=0,means all possible sum of coins <m so return false
   			return true;
	   else                     //if n=0,m==0 means we have some coins can make change of the m, so return true
	   		return false; 	
	}
	else{
	
		if(m<0)  return false;  //if n!=0 m<0,means all possible sum of coins >m so return false
		else if(m==0) return true;   //if n!=0 m==0,means we have some coins ,their sum is m,so return true
		else{                        // if n£¡=0 m>0,do the recursion 
			for(int i=N-n;i<N;i++){
			
				flag=calCoins(N-i-1,m-ValueOfCoins[i],output);     //traverse all sub problems
			if(flag==true){
				output[OutPutPoint++]=ValueOfCoins[i];              //if find answer ,store the output
				return flag;
					}
				}
		return flag;
			}
	}
}

int main(void){
	cin>>N>>M;
	int *output=new int[N];     //output array
	bool CanPay;                //eva can pay the money
	int j=1;  
	int sum=0;              
	/*InPut*/         
	for(int i=0;i<N;i++)
		cin>>ValueOfCoins[i];
	/*sort the coins eva has*/	
	sort(ValueOfCoins,ValueOfCoins+N);
	//for(int i=0;i<N;i++)
	    //cout<<ValueOfCoins[i]<<' ';
 	//cout<<endl;
	/*check whether eva can pay the money and record coins eva uses to pay*/
		CanPay=calCoins(N,M,output);
	
		
	/*OutPut*/
	if(CanPay==true){
		for(int i=OutPutPoint-1;i>0;i--){
			sum+=output[i];
			cout<<output[i]<<' ';
			j=i;	
		}
		cout<<output[j-1]<<endl;
		sum+=output[j-1];
		//cout<<sum<<endl;
			}	
	else{
		cout<<"No Solution"<<endl;	
	}
	return 0;
}
