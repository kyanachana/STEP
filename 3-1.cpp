#include <bits/stdc++.h>
#define rep(i,a,b) for(int i=int(a);i<int(b);++i)
#define SIZE 200005
#define INF 1000000005LL
#define MOD 1000000007
using namespace std;
typedef long long int ll;
typedef pair <int,int> P;
int index_number = 0;
vector<pair<string,double>> tokens;//数だったらex:<number,12>四則演算ならex:<calcu,1>plus:1 minus:2 times:3 divide:4
void readNumber(string line){//数字を読み取る
    double number=0;
    while(index_number<line.size()&&isdigit(line.at(index_number))){
        number = number*10 + int(line.at(index_number)-'0');
        
        index_number++;
    }
    if(index_number<line.size()&&line.at(index_number)=='.'){
        index_number++;
        double keta = 0.1;
        while(index_number<line.size()&&isdigit(line.at(index_number))){
            number += int(line.at(index_number)-'0')*keta;
            index_number++;
            keta/=10;
        }
    }
    tokens.push_back(make_pair("number",number));
    return;
}
void readPlus(string line){
    tokens.push_back(make_pair("calc",1));
    index_number++;
    return ;
}
void readMinus(string line){
    tokens.push_back(make_pair("calc",2));
    index_number++;
    return ;
}
void readTimes(string line){
    tokens.push_back(make_pair("calc",3));
    index_number++;
    return ;
}
void readDivide(string line){
    tokens.push_back(make_pair("calc",4));
    index_number++;
    return ;
}
void tokenize(string line){
    
    while(index_number<line.size()){
        if(isdigit(line.at(index_number))){
            readNumber(line);
        }else if(line.at(index_number)=='+'){
            readPlus(line);
        }else if(line.at(index_number)=='-'){
            readMinus(line);
        }else if(line.at(index_number)=='*'){
            readTimes(line);
        }else if(line.at(index_number)=='/'){
            readDivide(line);
        }else{
            cout<<"Invalid character found:"<<line.at(index_number)<<endl;
            index_number++;
        }
    }
    return;
}
double evaluate(){
    double answer = 0;
    tokens.insert(tokens.begin(),make_pair("calc",1));
    for(int i=1;i<tokens.size();i++){//一周目で*と/を計算する。
        if(tokens[i].first=="calc"&&tokens[i].second==3){
            tokens[i+1].second=tokens[i-1].second*tokens[i+1].second;//4*5を入力したら5の部分を20にし、4,*の部分は消す
            tokens.erase(tokens.begin()+i);
            tokens.erase(tokens.begin()+i-1);
            i--;
        }else if(tokens[i].first=="calc"&&tokens[i].second==4){
            tokens[i+1].second=tokens[i-1].second/tokens[i+1].second;
            tokens.erase(tokens.begin()+i);
            tokens.erase(tokens.begin()+i-1);
            i--;
        }
    }
    for(int i=1;i<tokens.size();i++){//2周目で+と-を計算する。
        if(tokens[i].first=="number"){
            if(tokens[i-1].first=="calc"&&tokens[i-1].second==1){
                answer += tokens[i].second;
            }else if(tokens[i-1].first=="calc"&&tokens[i-1].second==2){
                answer -= tokens[i].second;
            }
        }
    }
    return answer;
}
vector<double> expectedAnswer = {1,3,0.1,-19,101,2,0.5,5,3.5,2.5,0.04,0.333333333};
//testの答え
void test(string line,int order){
    tokenize(line);
    double actualAnswer = evaluate();
    tokens.clear();
    index_number=0;
    if (abs(actualAnswer - expectedAnswer[order]) < 1e-8){
        cout<<"PASS! ("<<line<<"="<<expectedAnswer[order]<<")"<<endl;
    }else{
        cout<<"FAIL! ("<<line<<" should be "<<expectedAnswer[order]<<" but was "<<actualAnswer<<")"<<endl;
    }
}

void runTest(){//test case
    printf("==== Test started! ====\n");
    int i=0;
    test("1",i++);
    test("1+2",i++);
    test("1.0+2.1-3",i++);
    test("1-20",i++);
    test("111-10",i++);
    test("1*2",i++);
    test("1/2",i++);
    test("1*2+1*3",i++);
    test("1+2*5/4",i++);
    test("1/2*5",i++);
    test("0.5*0.08",i++);
    test("1/3",i++);
    printf("==== Test finished! ====\n");
    return;
}
int main(){
    runTest();
    cout<<'>';
    string input;
    cin >> input;
    tokenize(input);
    double answer = evaluate();
    cout<<"answer:"<<answer<<endl;
  return 0;
}