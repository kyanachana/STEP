#include <bits/stdc++.h>
#define rep(i,a,b) for(int i=int(a);i<int(b);++i)
#define SIZE 200005
#define INF 1000000005LL
#define MOD 1000000007
using namespace std;
typedef long long int ll;
typedef pair <int,int> P;

//-----------概要---------------
//自分の名前を打ち込む
//名前の１文字目から始まるwikiページ→名前の２文字目から始まるwikiページ→名前のn文字目から始まるwikiページの遷移がn-1回で済むか判定
//もしあれば一例としてそのwikiページ名を表示する
//漢字対応しているので、漢字でも試してください:)

void change_to_wchar_t(char word[],wchar_t* &word_changed, int &word_size){
    //名前の漢字変換への対応 c++で漢字を扱うのは難しい
    setlocale(LC_CTYPE, "ja_JP.UTF-8");
    // 十分なメモリ領域を確保
    size_t capacity = strlen(word) + 1;
    word_changed = (wchar_t *)malloc(sizeof(wchar_t) * capacity);
    // char -> wchar_tの変換
    word_size = mbstowcs(word_changed,word, capacity);
    return;
}

void read_ifs_pagename(map<int,string> &ID_to_pagename,vector<set<int>> &page_start_with,int my_name_size,wchar_t* my_name_changed){
    //ID_to_pagenameとpage_start_withのデータを作る関数
    ifstream ifs_pagename("./pages.txt");
    int page_ID;
    char page_name[1000];
    while(ifs_pagename >> page_ID>>page_name){//全てのwikiページとIDを読み取る
        ID_to_pagename[page_ID]=page_name;//IDからページ名の対応 最後に一例を示す時に使う

        wchar_t *page_name_changed;
        int page_name_size;//使用しない
        change_to_wchar_t(page_name,page_name_changed,page_name_size);

        for(int i=0;i<my_name_size;i++){
          if(page_name_changed[0]==my_name_changed[i]){//wikiページの頭文字がmy_nameのi文字目と同じ場合
            page_start_with[i].insert(page_ID);//page_start_with[i]にpageのIDを入れる
          }
        }
    }
    return;
}

void read_ifs_link(vector<map<int,queue<int>>> &follow_page_data,vector<set<int>> page_start_with,int my_name_size){
    //follow_page_dataのデータを作る関数
    ifstream ifs_link("./links.txt");
    int page,follow_page;
    while(ifs_link >> page>>follow_page){
        for(int i=0;i<my_name_size-1;i++){
          if(page_start_with[i].count(page)&&page_start_with[i+1].count(follow_page)){
            //pageがmy_nameのi番目の文字から始まるページかつfollow_pageがmy_nameのi+1番目の文字から始まるページの場合
            follow_page_data[i][page].push(follow_page);
          }
        }
    }
    return;
}
int main(){
    char my_name[40];
    cout << "-------------------------"<<endl;
    cout << "Enter your name" << endl << ">>";
    cin >> my_name;
    wchar_t *my_name_changed;
    int my_name_size;
    change_to_wchar_t(my_name,my_name_changed,my_name_size);

    if(my_name_size<=1){
        cout<<"Please type longer name"<<endl;
        return 0;
    }
    cout << "Wait a minute..."<<endl;


    map<int,string> ID_to_pagename;//IDからpagenameを検索　key:ID,value:pagename
    vector<set<int>> page_start_with(my_name_size);
    //page_start_with[i]はmy_nameのi文字目から始まるwikiページの集合

    read_ifs_pagename(ID_to_pagename,page_start_with,my_name_size,my_name_changed);
    //ID_to_pagenameとpage_start_withのデータを作る関数

    vector<map<int,queue<int>>> follow_page_data(my_name_size);
    //follow_page_data[i][page]はmy_nameのi番目の文字から始まるページのpageがフォローしているmy_nameのi+1番目の文字から始まるページの集合queue
    read_ifs_link(follow_page_data,page_start_with,my_name_size);
    //follow_page_dataのデータを作る関数


    vector<map<int,int>> possible_to_reach(my_name_size);
    //possible_to_reach[i]はmapの集合になっている
    //key:my_nameのi番目の文字から始まるページ　value:そのページに辿りつくための１つ前のページ(のうちの１つ)
    //このvectorに入っているページはmy_nameの0文字目のページから連続で辿り着くページに限られている。

    //possible_to_reachを作る
    for(int name_number=0;name_number<my_name_size-1;name_number++){
      set<int> visited;//既に訪れたページには訪れない
      if(name_number==0){
        for(int old_page : page_start_with[name_number]){//名前の0番目の文字で始まるページの集合、0番目のときは全てのページで試せる
          while(!follow_page_data[name_number][old_page].empty()){//ページiがフォローしているページのうちmy_nameの1番目の文字で始まるページの集合
            int new_page = follow_page_data[name_number][old_page].front();
            if(visited.count(new_page)){
              follow_page_data[name_number][old_page].pop();
              continue;
            }else{
              visited.insert(new_page);
              possible_to_reach[name_number+1][new_page]=old_page;
              follow_page_data[name_number][old_page].pop();
            }
          }
        }
      }else{//1文字目以降はmy_nameの0文字目のページから連続で辿り着くページのみ
        for(pair<int,int> p:possible_to_reach[name_number]){
        int old_page = p.first;
          while(!follow_page_data[name_number][old_page].empty()){
            int new_page = follow_page_data[name_number][p.first].front();

            if(visited.count(new_page)){
              follow_page_data[name_number][old_page].pop();
              continue;
            }else{
              visited.insert(new_page);
              possible_to_reach[name_number+1][new_page]=old_page;
              //key:my_nameのname_number+1番目の文字から始まるページ　value:そのページに辿りつくための１つ前のページ
              follow_page_data[name_number][old_page].pop();
            }
          }
        }        
      }
    }

    cout << "-------------------------"<<endl;
    if(possible_to_reach[my_name_size-1].empty()){//最後まで辿りついたページがなかった場合
      cout<<"not found :("<<endl;
      cout<<"Let's try another name notation!"<<endl;
    }else{
      cout<<"found！Congratulations:)"<<endl;

      //一例を示していく
      int previous_page_ID;
      vector<int> page_data;
      for(pair<int,int> p:possible_to_reach[my_name_size-1]){//最後まで辿りついたページのうち１つを適当に選ぶ
        page_data.push_back(p.first);
        page_data.push_back(p.second);
        previous_page_ID = p.second;
        break;
      }
      for(int i=my_name_size-2;i>0;i--){//my_nameのi番目の文字から始まるページ→my_nameのi-1番目の文字から始まるページと辿る
        previous_page_ID = possible_to_reach[i][previous_page_ID];
        page_data.push_back(previous_page_ID);
      }
      reverse(page_data.begin(),page_data.end());//最後からになっているので順番を逆にする
      for(int i=0;i<page_data.size();i++){
        cout<<ID_to_pagename[page_data[i]]<<endl;
        
      }
      
    }

  return 0;
}