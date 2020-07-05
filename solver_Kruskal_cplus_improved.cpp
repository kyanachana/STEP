#include <bits/stdc++.h>
using namespace std;
#include <sys/time.h>
#include <tuple>
vector<pair<double,double>> cities;//都市をx座標、y座標をペアにして保管する

double get_time(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

void input_data(){//データの入力
    ifstream ifs("./input_6.csv");//***ファイル名は毎回変える***//
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

double distance(int city1,int city2){//都市と都市の距離を求める
    assert(city1<cities.size());
    assert(city2<cities.size());
    return sqrt(pow(cities[city1].first-cities[city2].first,2)+pow(cities[city1].second-cities[city2].second,2));
}

double tour_distance(vector<int> tour){//一周分の距離を求める(始点と終点を結んだ距離も含む)
    double total_distance;
    for(int i=0;i<tour.size();i++){
        total_distance += distance(tour[i],tour[(i+1)%tour.size()]);
    }
    return total_distance;
}



double route_distance(vector<int> route){//与えられたルート通りに行くときの距離を求める(始点と終点を結んだ距離を含まない)
    double total_distance;
    for(int i=0;i<route.size()-1;i++){
        total_distance += distance(route[i],route[i+1]);
    }
    return total_distance;
}

///////////////////ここから2_opt/////////////////

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



void alter_tour_2opt(vector<int> &tour){
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
        alter_tour_2opt(tour);
    }
    return;
}
///////////////////ここから15_opt/////////////////

void change_tour_segment_part1(vector<int> &tour){//都市を入れ替えていく
    int n = cities.size();
    for(int i=0;i<n-2;i++){
        for(int j=i+2;j<n-2;j++){
            double before = distance(tour[i],tour[(i+1)%tour.size()])+distance(tour[j%tour.size()],tour[(j+1)%tour.size()])+distance(tour[(j+1)%tour.size()],tour[(j+2)%tour.size()]);
            double after =  distance(tour[i],tour[(j+1)%tour.size()])+distance(tour[(j+1)%tour.size()],tour[(i+1)%tour.size()])+distance(tour[j%tour.size()],tour[(j+2)%tour.size()]);
            if(before>after){
                tour.insert(tour.begin()+i+1,tour[j+1]);
                tour.erase(tour.begin()+j+2);    
            }
        }
    }
}

void change_tour_segment_part2(vector<int> &tour){//都市を入れ替える
    int n = cities.size();
    for(int i=0;i<n-2;i++){
        for(int j=i+2;j<n-2;j++){

            double before = distance(tour[i],tour[(i+1)%tour.size()])+distance(tour[j%tour.size()],tour[(j+1)%tour.size()])+distance(tour[(j+1)%tour.size()],tour[(j+2)%tour.size()]);
            double after =  distance(tour[i],tour[(j+1)%tour.size()])+distance(tour[(j+1)%tour.size()],tour[(i+1)%tour.size()])+distance(tour[j%tour.size()],tour[(j+2)%tour.size()]);
            if(before>after){
                tour.insert(tour.begin()+i+1,tour[j+1]);
                tour.erase(tour.begin()+j+1); 
                  
            }
        }
    }
}

void alter_tour_15opt_part1(vector<int> &tour){//都市を入れ替えをこれ以上短くならないまで行う
    double before_distance = tour_distance(tour);
    int n = cities.size();
    change_tour_segment_part1(tour);
    double current_distance = tour_distance(tour);
    if(before_distance>current_distance){
        alter_tour_15opt_part1(tour);
    }
    return;
}
void alter_tour_15opt_part2(vector<int> &tour){//都市を入れ替えをこれ以上短くならないまで行う
    double before_distance = tour_distance(tour);
    int n = cities.size();
    change_tour_segment_part2(tour);
    double current_distance = tour_distance(tour);
    if(before_distance>current_distance){
        alter_tour_15opt_part2(tour);
    }
    return;
}

///////////////////ここから2_path_opt/////////////////

double make_each_shortest_route(vector<int> side,vector<int> &min_route){
    //最初side[0]と最後side[1]は固定し、それ以外の都市の移動の全てのパターンを試し、最短距離を返す
    int n = side.size();

    vector<int> order(n-2);
    for(int i=0;i<n-2;i++){
        order[i]=i;
    }
    double min_dis = 10000000;
    do{
        vector<int> this_tour;
        this_tour.push_back(side[0]);
        for(int i=0;i<n-2;i++){
            assert(order[i]>-1);
            assert(order[i]<n);
            this_tour.push_back(side[order[i]+2]);
        }
        this_tour.push_back(side[1]);
        if(min_dis>route_distance(this_tour)){
            min_dis = route_distance(this_tour);
            min_route = this_tour;
        }
    }while(next_permutation(order.begin(),order.end()));  

    return min_dis; 
}

tuple<double,vector<int>,vector<int>> make_whole_shortest_route(vector<int> ac_side,vector<int> bd_side){
    //ac間の最短距離とbd間の最短距離の和と、そのルートを返す
    vector<int> ac_shortest_route;
    vector<int> bd_shortest_route;
    double total_distance = make_each_shortest_route(ac_side,ac_shortest_route)+make_each_shortest_route(bd_side,bd_shortest_route);

    return forward_as_tuple(total_distance,ac_shortest_route,bd_shortest_route);

}

tuple<double,vector<int>,vector<int>> choose_cities(vector<int> tour,vector<int> choose,vector<int> &ac_side,vector<int> &bd_side,int cnt){
    //chooseの中の都市をac側とbd側のどちらに分けるかを考える
    vector<int> ac_best;
    vector<int> bd_best;
    if(cnt==choose.size()){
        vector<int> ac_shortest_route;
        vector<int> bd_shortest_route;
        double total_distance;
        tie(total_distance,ac_shortest_route,bd_shortest_route) = make_whole_shortest_route(ac_side,bd_side);
        return forward_as_tuple(total_distance,ac_shortest_route,bd_shortest_route);
    }else{
        vector<int> ac_side_copy = ac_side;
        ac_side_copy.push_back(choose[cnt]);
        vector<int> bd_side_copy = bd_side;
        bd_side_copy.push_back(choose[cnt]);
        vector<int> ac_shortest_route_1;
        vector<int> bd_shortest_route_1;
        double total_distance_1;
        vector<int> ac_shortest_route_2;
        vector<int> bd_shortest_route_2;
        double total_distance_2;
        tie(total_distance_1,ac_shortest_route_1,bd_shortest_route_1) = choose_cities(tour,choose,ac_side_copy,bd_side,cnt+1);
        tie(total_distance_2,ac_shortest_route_2,bd_shortest_route_2) = choose_cities(tour,choose,ac_side,bd_side_copy,cnt+1);
        if(total_distance_1<total_distance_2){
            return forward_as_tuple(total_distance_1,ac_shortest_route_1,bd_shortest_route_1);
        }else{
            return forward_as_tuple(total_distance_2,ac_shortest_route_2,bd_shortest_route_2);
        }
    }

        
    
}

void alter_tour_2path_opt(vector<int> &tour,int length){
    vector<int> tour_copy  = tour;

    double before_tour_distance = tour_distance(tour);
    int n=cities.size();

    for(int i=0;i<n-length;i++){                            

        for(int j=i+length;j<n-length;j++){
            int a = tour[i];
            int b = tour[i+length-1];
            int c = tour[j];
            int d = tour[j+length-1];
            
            if(distance(a,b)+distance(c,d)>distance(a,c)+distance(b,d)){
                vector<int> ac_side={a,c};               
                vector<int> bd_side={b,d};
                vector<int> choose;//選ぶ対象になる都市(a,b,c,d以外)
                for(int k=1;k<length-1;k++){
                    choose.push_back(tour[i+k]);
                    choose.push_back(tour[j+k]);
                }

                vector<int> tour_a_b;
                vector<int> tour_c_d;

                for(int k=0;k<length;k++){
                    tour_a_b.push_back(tour[i+k]);
                    tour_c_d.push_back(tour[j+k]);
                }

                double before_distance = route_distance(tour_a_b) + route_distance(tour_c_d);

                int cnt = 0;
                vector<int> ac_route;
                vector<int> bd_route;
                double after_distance;
                tie(after_distance,ac_route,bd_route) = choose_cities(tour,choose,ac_side,bd_side,cnt);

                if(before_distance>after_distance){
                    //ルートの入れ替え
                    for(int k=0;k<length;k++){
                        tour.erase(tour.begin()+j);
                    }
                    for(int k=0;k<length;k++){
                        tour.erase(tour.begin()+i);
                    }
                    for(int k=0;k<bd_route.size();k++){
                        tour.insert(tour.begin()+j-length+k,bd_route[k]);
                    }
                    for(int k=0;k<ac_route.size();k++){
                        tour.insert(tour.begin()+i+k,ac_route[k]);
                    }
                    int first = i+ac_route.size();
                    int end = j-length+ac_route.size()-1;
                    int count =  (end-first+1)/2;
                    for(int k=0;k<count;k++){
                        swap(tour[first+k],tour[end-k]);
                    } 
                    
                
                }
            }
        }
    }
    double after_tour_distance = tour_distance(tour);
    if(before_tour_distance>after_tour_distance){//これ以上短くならないまで行う
        alter_tour_2path_opt(tour,length);
    }
}


///////////////////ここから6_opt/////////////////
void alter_tour_6_opt_change_segement(vector<int> &tour,int length){
    int n=cities.size();
    for(int i=0;i<n-length;i++){
        vector<int> this_tour;
        //最初に訪れる都市と最後に訪れる都市は固定
        this_tour.push_back(tour[i]);
        this_tour.push_back(tour[i+length-1]);
        for(int j=1;j<length-1;j++){
            this_tour.push_back(tour[i+j]);
        }
        vector<int> before_route;
        for(int j=0;j<length;j++){
            before_route.push_back(tour[i+j]);
        }
        double before = route_distance(before_route);
        vector<int> min_route;
        
        double after = make_each_shortest_route(this_tour,min_route);//最短ルートをとってくる
        if(before>after){
            for(int j=0;j<length;j++){
                tour[i+j]=min_route[j];
            }
        }

    }
}

void alter_tour_6_opt(vector<int> &tour,int length){
    double before = tour_distance(tour);
    alter_tour_6_opt_change_segement(tour,length);
    double after = tour_distance(tour);
    if(before>after){
        alter_tour_6_opt(tour,length);
    }
}
///////////////////ここからはルート作成/////////////////

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



double kruskal(vector<pair<int,int>> &used_way){
    int n=cities.size();
    set<int> visited_city;
    vector<pair<double,pair<int,int>>> way;
    vector<double> ave_distance(n);

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
        visited_city.insert(start);
        visited_city.insert(end);
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

////ルート作成とルート改善の全体の動きを管理する
void solve(vector<int> &tour){

    //ルート作成
    //kruskal()で行うこと
    //①全ての都市と都市をつなぐ道の距離を計算する
    //②道が短い順に、採用しても閉路ができない、各都市に繋がれた都市の数が2を超えないように道を採用する

    //find_route()で行うこと
    //採用した道を頼りにルートを作成する

    vector<pair<int,int>> used_way;//採用した道を始点と終点をペアにして保管する。

    int whole_start = kruskal(used_way);//どの道を採用するか決める

    find_route(used_way,whole_start,tour); //ルートを作り出す 

    //ルートの改善
    //2-opt:任意の２点を入れ替えて距離が短くなったら入れ替える

    //2-path-opt
    //①a→bの道とc→dの道上の都市をa→cとb→dの二組に分ける。(a→bとc→dの都市数はlengthで定義する)
    //②その全ての分け方に対して最短ルートを見つけ、最も短くなる二組の分け方とそのルートを探す
    //③求めた最短ルートがもとよりも短くなれば入れ替える

    //15_opt
    //ある１つの都市に対して訪れる順番を入れ替えたら短くなる場合は入れ替える
    //part_1：入れ替えた先が今の地点より前の場合
    //part_2：入れ替えた先が今の地点より後ろの場合

    //6_opt
    //連続した都市(都市数はlengthで定義する)において訪れる順番を入れ替えたことでより短くなったら入れ替える


    double before_distance ;
    double after_distance ;
    

    //// challenge7の場合はここのコメントアウトを外す(challenge6のときはコメントアウトにする)
    //// challenge7は50分くらい実行時間がかかります。
    // alter_tour_2opt(tour);//2点の入れ替えで短くなったら入れ替える
    // alter_tour_6_opt(tour,9);
    // do{//ルートの改善
    //     before_distance = tour_distance(tour);
    //     alter_tour_15opt_part1(tour);//ある一点に訪れる順番を変えることで短くなったら、入れ替える(説明が難しいのでドキュメントの図を見てください)
    //     alter_tour_2opt(tour);//2点の入れ替えで短くなったら入れ替える
    //     alter_tour_15opt_part2(tour);//ある一点に訪れる順番を変えることで短くなったら、入れ替える(説明が難しいのでドキュメントの図を見てください)
    //     alter_tour_2opt(tour);//2点の入れ替えで短くなったら入れ替える

    //     after_distance = tour_distance(tour);
    // }while(before_distance>after_distance);//どちらを行っても短くならなくなるまで行う
    // // ////challenge7ではlength=5で行いました。
    // alter_tour_2path_opt(tour,5);
    // alter_tour_2opt(tour);
    // alter_tour_6_opt(tour,9);


    ////ここまでコメントアウト

    
    //challenge6の場合はここのコメントアウトを外す(challenge7のときはコメントアウトにする)
    //challenge6の実行は1分程度です。
    do{//ルートの改善
        before_distance = tour_distance(tour);
        alter_tour_15opt_part1(tour);//ある一点に訪れる順番を変えることで短くなったら、入れ替える(説明が難しいのでドキュメントの図を見てください)

        alter_tour_2opt(tour);//2点の入れ替えで短くなったら入れ替える
        
        after_distance = tour_distance(tour);
    }while(before_distance!=after_distance);//どちらを行っても短くならなくなるまで行う

    do{//ルートの改善
        before_distance = tour_distance(tour);
        alter_tour_15opt_part2(tour);//ある一点に訪れる順番を変えることで短くなったら、入れ替える(説明が難しいのでドキュメントの図を見てください)

        alter_tour_2opt(tour);//2点の入れ替えで短くなったら入れ替える

        after_distance = tour_distance(tour);
    }while(before_distance>after_distance);//どちらを行っても短くならなくなるまで行う

    alter_tour_2path_opt(tour,5);
    alter_tour_2opt(tour);

    alter_tour_6_opt(tour,8);
    //ここまでコメントアウト

    
    


    return;
}



int main(){
    double begin = get_time();//開始時間
    input_data();//データの読み込み

    //解く
    vector<int> tour;
    solve(tour);

    //結果の表示
    set<int> tour_count;
    int n= tour.size();
    for(int i=0;i<n;i++){
        tour_count.insert(tour[i]);
        cout<<tour[i]<<endl;
    }
    
    cout<<"都市"<<tour_count.size()<<endl;//チェック用

    double end = get_time();//終了時間
    cout<<"実行時間："<<end-begin<<endl;

    //データの出力
    ofstream ofs("output_6.csv"); //***ファイル名は毎回変える***//
    ofs <<"index"<<endl;
    for(int i=0;i<n;i++){
        ofs<<tour[i]<<endl;
    }
    return 0;
}