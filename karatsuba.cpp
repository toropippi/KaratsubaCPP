//Karatsuba乗算
#include "bits/stdc++.h"
using namespace std;
#define REP(i, n) for(ll i = 0;i < n;i++)
#define ll long long
#define ull unsigned long long
#define uint unsigned int
#define MOD 1000000007
#define llMAX 9223372036854775807
#define llMIN -9223372036854775808
using vi = vector<ll>; // intの1次元の型に vi という別名をつける
using vvi = vector<vi>; // intの2次元の型に vvi という別名をつける

ll n,k,x,y;


//最上位桁のオーバーフロー考慮無し
vector<uint> karatsuba_Add(vector<uint> const &a,vector<uint> const &b){
  int lg=a.size();
  vector<uint> c(lg,0);
  for(int i=0;i<lg-1;i++){
    uint tmp=a[i]+b[i];
    c[i]+=tmp;
    if (tmp<a[i]) c[i+1]++;
    if (c[i]<tmp) c[i+1]++;
  }
  c[lg-1]+=a[lg-1]+b[lg-1];
  return c;
}

//最上位桁のオーバーフロー考慮あり
vector<uint> karatsuba_Add2(vector<uint> const &a,vector<uint> const &b,uint &flag){
  int lg=a.size();
  vector<uint> c(lg,0);
  uint tmp;
  for(int i=0;i<lg-1;i++){
    tmp=a[i]+b[i];
    c[i]+=tmp;
    if (tmp<a[i]) c[i+1]++;
    if (c[i]<tmp) c[i+1]++;
  }
  
  tmp=a[lg-1]+b[lg-1];
  c[lg-1]+=tmp;
  if (tmp<a[lg-1]) flag=1;
  if (c[lg-1]<tmp) flag=1;
  return c;
}

//最上位桁のアンダーフロー考慮無し
vector<uint> karatsuba_Sub(vector<uint> const &a,vector<uint> const &b){
  int lg=a.size();
  vector<uint> c(lg);
  copy(a.begin(), a.end(), c.begin());

  uint flag=0;
  for(int i=0;i<lg;i++)
  {
    if (flag==1){
      c[i]--;
      flag=(c[i]==4294967295);
    }

    if (c[i]<b[i]){
      flag=1;
    }
    c[i]-=b[i];
  }
  return c;
}


//最上位桁のアンダーフロー考慮あり
vector<uint> karatsuba_Sub2(vector<uint> const &a,vector<uint> const &b,uint &flag){
  int lg=a.size();
  vector<uint> c(lg);
  copy(a.begin(), a.end(), c.begin());

  for(int i=0;i<lg;i++)
  {
    if (flag==1){
      c[i]--;
      flag=(c[i]==4294967295);
    }

    if (c[i]<b[i]){
      flag=1;
    }
    c[i]-=b[i];
  }
  return c;
}

//abs(a-b)を返してa<bのときflagをonにする
vector<uint> karatsuba_Sub3(vector<uint> const &a,vector<uint> const &b,uint &flag){
  int lg=a.size();
  vector<uint> c(lg);
  copy(a.begin(), a.end(), c.begin());

  for(int i=0;i<lg;i++)
  {
    if (flag==1){
      c[i]--;
      flag=(c[i]==4294967295);
    }
    if (c[i]<b[i]){
      flag=1;
    }
    c[i]-=b[i];
  }


  if (flag==1)
  {//全体を反転する
    uint kuri=0;
    c[0]=4294967295-c[0];
    c[0]++;
    if (c[0]==0)kuri==1;
    for(int i=1;i<lg;i++)
    {
      c[i]=4294967295-c[i];
      if (kuri==1){
        c[i]++;
        kuri=(c[i]==0);
      }
    }
  }
  
  return c;
}


//参照渡しではない
vector<uint> karatsuba_mul(vector<uint> const &a,vector<uint> const &b)
{
  int lg=a.size();
  if (lg==1){
    vector<uint> c(2);
    ull tmpc=(ull)a[0]*(ull)b[0];
    c[0]=tmpc%4294967296;
    c[1]=tmpc/4294967296;
    return c;
  }else{
    vector<uint> a0(lg/2),a1(lg/2);
    vector<uint> b0(lg/2),b1(lg/2);
    vector<uint> c0,c2,c1;
    copy(a.begin(), a.begin()+lg/2, a0.begin());
    copy(a.begin()+lg/2, a.begin()+lg, a1.begin());
    copy(b.begin(), b.begin()+lg/2, b0.begin());
    copy(b.begin()+lg/2, b.begin()+lg, b1.begin());

    c0=karatsuba_mul(a0,b0);
    c2=karatsuba_mul(a1,b1);
    uint invflaga=0,invflagb=0;
    vector<uint> a10=karatsuba_Sub3(a1,a0,invflaga);
    vector<uint> b10=karatsuba_Sub3(b1,b0,invflagb);
    c1=karatsuba_mul(a10,b10);

    
    uint c02flag=0;
    uint ansc1flag=0;//真のc1の繰り上がり
    vector<uint> c02=karatsuba_Add2(c0,c2,c02flag);
    vector<uint> ansc1;//真のc1の値

    if ((invflaga+invflagb)%2==0){
      //c0+c2-c1;
      ansc1=karatsuba_Sub2(c02,c1,ansc1flag);
      ansc1flag=-ansc1flag+c02flag;
    }else{
      //c0+c2+c1;
      ansc1=karatsuba_Add2(c02,c1,ansc1flag);
      ansc1flag+=c02flag;
    }

    
    //c0,ansc1,c2をまとめる
    vector<uint> c(lg*2);
    for(ll i=0;i<lg;i++){
      c[i]=c0[i];
    }
    for(ll i=0;i<lg;i++){
      c[i+lg]=c2[i];
    }
    
    for(ll i=0;i<lg;i++){
      c[i+lg/2]+=ansc1[i];
      if (c[i+lg/2]<ansc1[i]){
        for(ll j=i+lg/2+1;j<lg*2;j++){
          c[j]++;
          if (c[j]!=0)break;
        }
      }
    }


    if (ansc1flag){
      for(ll j=lg/2+lg;j<lg*2;j++){
        c[j]++;
        if (c[j]!=0)break;
      }
    }

    return c;
  }
}




int main(){
  ll ans=0;
  ll n;
  cin>>n;
  if (n>20)n=20;
  if (n<1)n=2;
  n=1<<n;
  vector<uint> a(n,1);
  vector<uint> b(n,1);
  vector<uint> c(n*2);

  //ここで入力
  /*
  REP(i,n)
    cin>>a[i];
  REP(i,n)
    cin>>b[i];
  */

  c=karatsuba_mul(a,b);
  //配列結果REP cout
  for (auto elem : c) {
    cout<<elem<<endl;
  }

  return 0;
}