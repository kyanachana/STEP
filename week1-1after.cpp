#include <bits/stdc++.h>
#define rep(i,a,b) for(int i=int(a);i<int(b);++i)
#define SIZE 200005
#define INF 1000000005LL
#define MOD 1000000007
using namespace std;
typedef long long int ll;
typedef pair <int,int> P;
vector<pair<string,string> > new_dictionary;

int binary_search(string new_word){
    int length = new_dictionary.size();
    int left=0;
    int right = length-1;
    while(left<=right){
        int middle = (left+right)/2;
        if(new_dictionary[middle].first==new_word){
            return middle;
        }
        if(new_dictionary[middle].first<new_word){
            left = middle+1;
        }else{
            right = middle-1;
        }
    }
    return -1;
}
int main(){
    ifstream ifs("./dictionary.words");
    string str;
    while (getline(ifs, str)){
        string str_before=str;
        str[0]=tolower(str[0]);
        sort(str.begin(),str.end());
        //stringはソートすることでアルファベット順に並べられる
        new_dictionary.push_back(make_pair(str,str_before));
    }
    sort(new_dictionary.begin(),new_dictionary.end());
    string random_word;
    while(cin>>random_word&&random_word!="!"){
        //　ループで入力できるようにした
        sort(random_word.begin(),random_word.end());
        int check = binary_search(random_word);
        if(check==-1){
            cout << -1 << endl;
        }else{
            cout << new_dictionary[check].second << endl;
        }
    }
    
  return 0;
}