#include<iostream>
using namespace std;
int main()
{
        float input=1.7;
        float num=input/10.0;
        cout << num << endl;
       
        num*=10.0;
        
        cout << num << endl;
        bool stat = num == 1.7;
        cout << stat << endl;
        if(num <= 1.7 + FLT_EPSILON && num >= 1.7 - FLT_EPSILON)
        {
                cout << "num is equal to 1.7." << endl;
        }
}
