#include <bits/stdc++.h>
#define rep(i,a,b) for(int i=int(a);i<int(b);++i)
#define SIZE 200005
#define INF 1000000005LL
#define MOD 1000000007
using namespace std;
typedef long long int ll;
typedef pair <int,int> P;
#include <sys/time.h>

vector<pair<double,double>> cities;//都市をx座標、y座標をペアにして保管する

double get_time(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

void input_data(){
    ifstream ifs("./input_5.csv");//***ファイル名は毎回変える***//
    string str;

    getline(ifs, str);//１行目(x,y)はいらないので飛ばして読む
    int input_count = 0;

    while (getline(ifs, str)){
        string tmp;
        istringstream stream(str);
        double x,y;

        // 区切り文字がなくなるまで文字を区切っていく
        while (getline(stream, tmp, ',')){
            if(input_count%2==0){
                x = stod(tmp);
            }else{
                y = stod(tmp);
                cities.push_back(make_pair(x,y));
            }

            input_count++;
        }
       
    }
}

double distance(int city1,int city2){
    assert(city1<cities.size());
    assert(city2<cities.size());
    return sqrt(pow(cities[city1].first-cities[city2].first,2)+pow(cities[city1].second-cities[city2].second,2));
}

double tour_distance(vector<int> tour){
    double total_distance;
    for(int i=0;i<tour.size();i++){
        total_distance += distance(tour[i],tour[(i+1)%tour.size()]);
    }
    return total_distance;
}

void reverse_segment(vector<int> &tour,int i,int j){
    int a = tour[i-1];
    int b = tour[i];
    int c = tour[j-1];
    int d = tour[j%tour.size()];
    if(distance(a,b)+distance(c,d)>distance(a,c)+distance(b,d)){
        int first_end = i;
        int second_start = j-1;
        int count = (second_start-first_end+1)/2;
        for(int k=0;k<count;k++){

            swap(tour[first_end+k],tour[second_start-k]);
        }
    }

}



void alter_tour(vector<int> &tour){
    double before_distance = tour_distance(tour);
    int n = cities.size();
    for(int i=1;i<n;i++){
        for(int j=i+1;j<n+1;j++){
            int start = i;
            int end = j;
            reverse_segment(tour,i,j);
        }
    }
    if(before_distance>tour_distance(tour)){
        //これ以上2点を入れ替えても距離が短くならなくなるまで行う
        alter_tour(tour);
    }
    return;
}


int root(int city,vector<int> &parent){
    if(parent[city]==city){
        return city;
    }else{
        parent[city]=root(parent[city],parent);
        return parent[city];
    }
}

bool is_same_root(int city1,int city2,vector<int> parent){
    return root(city1,parent)==root(city2,parent);
}

void unite(int city1,int city2,vector<int> &parent){
    parent[root(city1,parent)]=root(city2,parent);
    return;
}



int kruskal(vector<pair<int,int>> &used_way){
    int n=cities.size();
    vector<pair<double,pair<int,int>>> way;
    //wayは全てのn(n-1)/2通りの道をまとめた配列
    //距離と<始点と終点のペア>をペアとして持っている
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            way.push_back(make_pair(distance(i,j),make_pair(i,j)));
        }
    }
    sort(way.begin(),way.end());//wayを距離が短い順にソートする

    vector<int> parent(n);//union_find木の作成
    for(int i=0;i<n;i++){
        parent[i]=i;
    }
    vector<int> connected_count(n);
    for(int i=0;i<way.size();i++){
        int start = way[i].second.first;
        int end = way[i].second.second;
        if(is_same_root(start,end,parent)){
            //道を採用すると閉路ができてしまうので、採用しない
            continue;
        }
        if(connected_count[start]==2||connected_count[end]==2){
            //既に２つの都市とつながっている場合も道を採用しない
            continue;
        }

        //この２つの条件をクリアしたら道を採用する
        unite(start,end,parent);
        used_way.push_back(make_pair(start,end));
        connected_count[start]++;
        connected_count[end]++;
    }
    int whole_start;//つながった道の数が１ならそれが始点(or終点)
    for(int i=0;i<n;i++){
        if(connected_count[i]!=2){
            whole_start=i;
            break;
        }
    }
    return whole_start;

}

void find_route(vector<pair<int,int>> used_way,int whole_start,vector<int> &tour){
    int n = cities.size();
    tour.push_back(whole_start);
    int current_city = whole_start;
    for(int i=0;i<n-1;i++){
        for(int j=0;j<used_way.size();j++){
            int start = used_way[j].first;
            int end = used_way[j].second;
            //道の片方が今の都市なら、もう片方の都市が次の都市
            if(current_city==start){
                used_way.erase(used_way.begin()+j);
                current_city = end;
                tour.push_back(current_city);
                break;
            }else if(current_city==end){
                used_way.erase(used_way.begin()+j);
                current_city = start;
                tour.push_back(current_city);
                break;
            }
        }
    }

}

void solve(vector<int> &tour){
    vector<pair<int,int>> used_way;//採用した道を始点と終点をペアにして保管する。

    int whole_start = kruskal(used_way);//どの道を採用するか決める

    find_route(used_way,whole_start,tour); //ルートを作り出す   

    alter_tour(tour);//ルートの改善

    return;
}



int main(){
    double begin = get_time();//開始時間
    input_data();//データの読み込み

    vector<int> tour;
    solve(tour);

    //結果の表示
    int n= tour.size();
    for(int i=0;i<n;i++){
        cout<<tour[i]<<endl;
    }

    double end = get_time();//終了時間
    cout<<"実行時間："<<end-begin<<endl;

    //データの出力
    ofstream ofs("output_5.csv"); //***ファイル名は毎回変える***//
    ofs <<"index"<<endl;
    rep(i,0,n){
        ofs<<tour[i]<<endl;
    }
    return 0;
}