#include <bits/stdc++.h>
#define rep(i,a,b) for(int i=int(a);i<int(b);++i)
#define SIZE 200005
#define INF 1000000005LL
#define MOD 1000000007
using namespace std;
typedef long long int ll;
typedef pair <int,int> P;
vector<string> dictionary;
vector<pair<string,string> > new_dictionary;
//sorted
vector<char> data(16);
//input
vector<string> made_words;
//inputから生成するwords
string sort_word(string word){
    int size = word.size();
    vector<char> chars;
    string sorted_word=word;
    for(int i=0;i<size;i++){
        if(i==0){
            word[0]=tolower(word[0]);
        }
        chars.push_back(word[i]);
    }
    sort(chars.begin(),chars.end());
    for(int i=0;i<size;i++){
        sorted_word.at(i) = chars.at(i);
    }
    return sorted_word;
}
//　大まかな手法は正しいと思いますが、同じ文字が複数回　word に　push_back　されてしまいます。
//　例えば、potato -> aooptt で make_word を使うと aoptt は二回生成されます。 
void make_word(int i,string word){
    // nit: ここの 15 は静的変数で保管すると読みやすくなります。
    if(i==15){
        string sorted;
        //　ソートされた string の substring は既にソートされているので make_word　をコールする前にあらかじめ word をソートしておくとここで毎回  sort せずにすみます。
        sorted = sort_word(word);
        made_words.push_back(sorted);
        //cout << word<<endl;
        return;
    }else{
        make_word(i+1,word);
        word.push_back(data[i]);
        make_word(i+1,word);
    }
    return;
}
int binary_search(string new_word,int length){
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
    int qucheck=0;
    // nit: cin の前に何かメッセージを cout すると使いやすくなります。
    cin >> qucheck;
    if(qucheck==1){
        // nit: ここの 17 は静的変数で保管すると読みやすくなります。
        for(int i=0;i<17;i++){
            cin >> data[i];
        }
    }else{
        // nit: 上と同じく。
        for(int i=0;i<16;i++){
            cin >> data[i];
        }
    }
    string a="";
    make_word(0,a);
    ifstream ifs("./dictionary.words");
    string str;
    int length=0;

    while (getline(ifs, str)){
        // dictionaryはどこにも使われてないのでわざわざ構築しなくてもいいです。
        dictionary.push_back(str);
        new_dictionary.push_back(make_pair(sort_word(str),str));
        length++;
    }
    sort(new_dictionary.begin(),new_dictionary.end());
    int M=0;
    int point = 0;
    string ans;
    for(string s:made_words){
        int check = binary_search(s,length);
        
        if(check!=-1){
            point = 0;
            string final = new_dictionary[check].second;
            // nit: ここで final_size　を別に保管せずに、final.size() を直接使用してもいいと思います。
            int final_size = final.size();
            final[0]=tolower(final[0]);
            if(qucheck==0){
                for(int i=0;i<final_size;i++){
                    //　nit: ここで何かのデータ構造を使い char を入れておくと少し読みやすくなります。
                    if(final[i]=='c'||final[i]=='f'||final[i]=='h'||final[i]=='l'||final[i]=='m'||final[i]=='p'||final[i]=='v'||final[i]=='w'||final[i]=='y'){
                        point += 2;
                    }else if(final[i]=='j'||final[i]=='k'||final[i]=='q'||final[i]=='x'||final[i]=='z'){
                        point += 3;
                    }else{
                        point += 1;
                    } 
                }
            }else if(qucheck==1){
                for(int i=0;i<final_size;i++){
                    if(final[i]=='c'||final[i]=='f'||final[i]=='h'||final[i]=='l'||final[i]=='m'||final[i]=='p'||final[i]=='v'||final[i]=='w'||final[i]=='y'){
                        point += 2;
                    }else if(final[i]=='j'||final[i]=='k'||final[i]=='x'||final[i]=='z'){
                        point += 3;
                    }else if(final[i]=='q'){
                        point += 3;      
                        i=i+1;
                    }else{
                        point += 1;
                    } 
                }
            }
            if(M<point){
                M = max(M,point);
                ans = final;
            }           
        }        
    }
    cout << ans << endl << M << endl;  
  return 0;
}