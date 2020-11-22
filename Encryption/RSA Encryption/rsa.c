#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<string.h>
#include <unistd.h>
#include<stdlib.h>
typedef long long ll;
ll e_key,d_key,n;


ll power(ll a,ll b,ll c)
{
    ll res=1;
    a=a%c;
    while(b>0)
    {
        if(b%2!=0)
        res=(res*a)%c;

        b/=2;
        a=(a*a)%c;
    }
    return res;
}

ll gcd(ll a,ll b)
{
    if(a==0)
    return b;

    else
    {
        return gcd(b%a,a);
    }
    
}
ll encrypt(ll m)
{

    //  ll p=1000004249;
    //  ll q=1000004233;

    ll p=42101;
    ll q=42089;
    n=p*q;
    ll a  = (p-1)*(q-1);
    for(ll i=2;i<=a;i++)
    {
        if(gcd(i,a)==1)
        {
            e_key=i;
            break;
        }
    }

    for(ll i=2;i<=a;i++)
    {
        ll z=1+a*i;
        if(z%e_key==0)
        {
            d_key=z/e_key;
            break;
        }    
    }
    ll test = (d_key * e_key)%a;
    ll ct = power(m,e_key,n);
    return ct;

}
ll decrypt(ll a , ll b , ll c)
{
    ll mes = power(a,b,c);
    return mes;
}
int main(int argc , char **argv)
{
    
    ll message;

    if(strcmp(argv[1],"encrypt")==0)
    {
        printf("Enter the message : ");
        scanf("%lld",&message);
        ll d = encrypt(message);
        printf("Encrypted message is : %lld\n",d);
        printf("public key is {%lld,%lld}\n",e_key,n);
        printf("private key is {%lld,%lld}\n",d_key,n);

    }
    else if (strcmp(argv[1],"decrypt")==0)
    {
        printf("Enter the message : ");
        scanf("%lld",&message);
        if(argc!=4)
        {
            printf("provide key\n");
            return 0;
        }
        ll p = atoll(argv[2]);
        ll q = atoll(argv[3]);
        ll d = decrypt(message , p , q);
        printf("Decrypted message is : %lld\n",d);
    }
    else
    {
        printf("invalid command\n");
    }
    return 0;
}