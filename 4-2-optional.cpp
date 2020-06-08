#include <bits/stdc++.h>
#define rep(i,a,b) for(int i=int(a);i<int(b);++i)
#define SIZE 200005
#define INF 1000000005LL
#define MOD 1000000007
using namespace std;
typedef long long int ll;
typedef pair <int,int> P;

const int people_number = 100;
vector<vector<bool>> follow_relation(people_number,vector<bool>(people_number));

//自分と興味のある人の共通の友だちを探すプログラム
//Facebookにある機能
//友達の定義:相互フォローしていること
//step1:自分の名前を入力
//step2:興味のある人の名前を入力
//result:自分と興味ある人の共通の友達の人数と、その名前を出力

void find_friends(int ID, queue<int> &friends){//友達を見つけ、queueに入れる
    for(int i=0;i<follow_relation[0].size();i++){
        if(follow_relation[ID][i]&&follow_relation[i][ID]){
            friends.push(i);
        }
    }
    return;
}

void find_common_friends(int interested_ID, queue<int> &my_friends,queue<int> &common_friends){
    //自分の友だちが入ったmy_friendsから１人１人確認して、興味のある人にとっても友達ならcommon_friendsに入れる
    while(!my_friends.empty()){
        if(follow_relation[interested_ID][my_friends.front()]&&follow_relation[my_friends.front()][interested_ID]){
            common_friends.push(my_friends.front());
        }
        my_friends.pop();
    }
    return;
}
bool check_nickname(string nickname, map<string,int> nickname_to_ID){//入力されたニックネームが存在するか確認
    if(!nickname_to_ID.count(nickname)){
        cout << "There is no such nickname"<<endl;
        return true;
    }else{
        return false;
    }
}

int main(){
    ifstream ifs_link("./links.txt");
    ifstream ifs_nickname("./nicknames.txt");
    map<string,int> nickname_to_ID;//nicknameからIDを検索　key:nickname,value:ID
    map<int,string> ID_to_nickname;//IDからnicknameを検索　key:ID,value:nickname

    int follow_people,people;
    while(ifs_link >> people>>follow_people){
        follow_relation.at(people).at(follow_people)=true;
    }

    int nickname_ID;
    string nickname;
    while(ifs_nickname >> nickname_ID>>nickname){
        nickname_to_ID[nickname]= nickname_ID;
        ID_to_nickname[nickname_ID]=nickname;
    }
    string my_name ;
    string interested_name;
    cout <<"Let's find common friends :)"<<endl;
    cout<<"----------------------------"<<endl;
    cout<<"Enter your name"<<endl<< ">";
    cin >> my_name;
    if(check_nickname(my_name,nickname_to_ID)) return 0;
    
    cout<<"Enter the name you are interested in"<<endl <<">";
    cin >> interested_name;
    if(check_nickname(interested_name,nickname_to_ID)) return 0;
    
    if(my_name==interested_name){
        cout<<"You typed same people. Try again"<<endl;
        return 0;
    }
    int my_ID =nickname_to_ID[my_name];
    int interested_ID = nickname_to_ID[interested_name];
    
    queue<int> my_friends;
    find_friends(my_ID,my_friends);

    queue<int> common_friends;
    find_common_friends(interested_ID,my_friends,common_friends);

    cout<<"----------------------------"<<endl;
    if(common_friends.empty()){
        cout<<"You don't have a friend in common"<<endl;
    }else{
        if(common_friends.size()==1){
            cout << "found " << common_friends.size() << " friend in common"<<endl;
        }else{
            cout << "found " << common_friends.size() << " friends in common"<<endl;
        }
        while(!common_friends.empty()){
            cout << ID_to_nickname[common_friends.front()];
            if(common_friends.size()!=1){
                cout<<", ";
            }else{
                cout<<endl;
            }
            common_friends.pop();
        }        
    }   
  return 0;
}
