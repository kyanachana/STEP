#include <bits/stdc++.h>
#define rep(i,a,b) for(int i=int(a);i<int(b);++i)
#define SIZE 200005
#define INF 1000000005LL
#define MOD 1000000007
using namespace std;
typedef long long int ll;
typedef pair <int,int> P;
int index_number = 0;
stack<double> stack_number;//数字を入れる
stack<char> stack_operator;//演算子を入れる
vector<pair<char,double>> tokens;//数だったらex:<'n',12>四則演算ならex:<'+',0>　0に意味はなく、使わない
//逆ポーランド法で解いたプログラム
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
    tokens.push_back(make_pair('n',number));
    return;
}
void readPlus(string line){
    tokens.push_back(make_pair('+',0));
    index_number++;
    return ;
}
void readMinus(string line){
    tokens.push_back(make_pair('-',0));
    index_number++;
    return ;
}
void readTimes(string line){
    tokens.push_back(make_pair('*',0));
    index_number++;
    return ;
}
void readDivide(string line){
    tokens.push_back(make_pair('/',0));
    index_number++;
    return ;
}
void readBracket_begin(string line){
    tokens.push_back(make_pair('(',0));
    index_number++;
    return;
}
void readBracket_end(string line){
    tokens.push_back(make_pair(')',0));
    index_number++;
    return;
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
        }else if(line.at(index_number)=='('){
            readBracket_begin(line);
        }else if(line.at(index_number)==')'){
            readBracket_end(line);
        }else{
            cout<<"Invalid character found:"<<line.at(index_number)<<endl;
            index_number++;
        }
    }
    return;
}
int operator_primary(char c){
    if(c=='+'){
        return 1;
    }else if(c=='-'){
        return 1;
    }else if(c=='*'){
        return 2;
    }else if(c=='/'){
        return 2;
    }else if(c=='('){
        return 0;
    }else if(c==')'){
        return 0;
    }
    return 0;
}
double calculate(double first_number,double second_number,char operator_char){
    if(operator_char=='+'){
        return first_number+second_number;
    }else if(operator_char=='-'){
        return first_number-second_number;
    }else if(operator_char=='*'){
        return first_number*second_number;
    }else if(operator_char=='/'){
        return first_number/second_number;
    }
    return 0;
}
double evaluate(){
    double first_number;//stackから取り出す２つの数
    double second_number;
    double after_number;//計算結果を求め、stackに入れる
    for(int i=0;i<tokens.size();i++){
        if(tokens[i].first=='n'){//数字ならstack_numberに追加
            stack_number.push(tokens[i].second);
        }else if(tokens[i].first==')'){//")"なら"("が出るまで演算を行う
            while(stack_operator.top()!='('){
                second_number=stack_number.top();
                stack_number.pop();
                first_number = stack_number.top();
                stack_number.pop();
                after_number=calculate(first_number,second_number,stack_operator.top());
                stack_number.push(after_number);
                stack_operator.pop();
                
            }
            stack_operator.pop();//"("を消す
            
        }else if(tokens[i].first=='('){
            stack_operator.push(tokens[i].first);
        }else{
            if(!stack_operator.empty()&& operator_primary(stack_operator.top())>=operator_primary(tokens[i].first)){
                //stack_operatorの一番上の方が新しい演算よりも優先順位が高いまたは同じ場合
                //stack_numberから２つ数字を取り出し、stack_operatorから演算子を取り出す。
                //calculateで２つの数字を演算子で計算したものをstack_numberに加える。
                //新しい演算を加える。
                second_number=stack_number.top();
                stack_number.pop();
                first_number = stack_number.top();
                stack_number.pop();
                after_number = calculate(first_number,second_number,stack_operator.top());
                stack_operator.pop();
                stack_number.push(after_number);
                stack_operator.push(tokens[i].first);
            }else{
                //stack_operatorの一番上の方が新しい演算よりも優先順位が低い場合
                stack_operator.push(tokens[i].first);
            }
        }
    }
    while(!stack_operator.empty()){
        //残っている演算子がなくなるまで計算する。
        second_number=stack_number.top();
        stack_number.pop();
        first_number = stack_number.top();
        stack_number.pop();
        after_number=calculate(first_number,second_number,stack_operator.top());
        stack_number.push(after_number);
        stack_operator.pop();
    }
    
    double answer = stack_number.top();
    stack_number.pop();
    assert(stack_number.empty());
    assert(stack_operator.empty());//check
    return answer;

}
vector<double> expectedAnswer = {1,3,0.1,-0.099,-19,101,2,0.5,5,3.5,2.5,0.04,0.333333333,1,3.5,25,15,1.4};
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
void runTest(){
    printf("==== Test started! ====\n");
    int i=0;
    test("1",i++);
    test("1+2",i++);
    test("1.0+2.1-3",i++);
    test("0.001-0.1",i++);
    test("1-20",i++);
    test("111-10",i++);
    test("1*2",i++);
    test("1/2",i++);
    test("1*2+1*3",i++);
    test("1+2*5/4",i++);
    test("1/2*5",i++);
    test("0.5*0.08",i++);
    test("1/3",i++);
    test("1/3+1/3+1/3",i++);
    test("(1+2*3)/2",i++);
    test("((1+2)*(1+(4/2+1)))*2+1",i++);
    test("(15)",i++);
    test("(3.0+4*(2-1))/5",i++);
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
    cout<<answer<<endl;
  return 0;
}