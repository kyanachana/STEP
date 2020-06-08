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

int check(int start_ID,int target_ID){
    //BFSで解きました
    queue<pair<int,int>> linked_people_queue;//pair<ID,辿り着くまでの回数(depth)>
    set<int> visited_ID;//既に確認したID
    linked_people_queue.push(make_pair(start_ID,0));
    visited_ID.insert(start_ID);
    while(!linked_people_queue.empty()){
        int queue_top_ID = linked_people_queue.front().first;
        int depth = linked_people_queue.front().second;
        linked_people_queue.pop();
        if(queue_top_ID==target_ID){
            //found
            return depth;
        }
        for(int i=0;i<follow_relation[0].size();i++){
            if(follow_relation[queue_top_ID][i]){
                if(visited_ID.count(i)){//既に訪れた
                    continue;
                }else{
                    linked_people_queue.push(make_pair(i,depth+1));
                    visited_ID.insert(i);
                }

            }
        }
    }
    return -1;
}

int main(){
    ifstream ifs("./links.txt");
    ifstream ifs2("./nicknames.txt");
    map<string,int> nickname_data;

    int follow_people = 0;
    int people = 0;
    while(ifs >> people>>follow_people){
        follow_relation.at(people).at(follow_people)=true;
    }

    int nickname_ID;
    string nickname;
    while(ifs2 >> nickname_ID>>nickname){
        nickname_data[nickname]= nickname_ID;
    }
    
    string start_name = "adrian";
    string target_name = "jon";//myname
    int start_ID =nickname_data[start_name];
    int target_ID = nickname_data[target_name];
    
    if(check(start_ID,target_ID)!=-1){
        cout<<"reached!"<<endl;
        cout<<"times:"<<check(start_ID,target_ID)<<endl;
    }else{
        cout<<"failed";
    }

  return 0;
}