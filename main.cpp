#include <iostream>
#include <fstream>
using namespace std;

bool **Map,**visited;
int **dist;
int m=0, n=0, B=0;
int R_row, R_col;

class Pair{
public:
    int x,y;
    Pair():x(-1),y(-1){};
    Pair(int a,int b):x(a),y(b){};
};

class Node{
public:
    Pair data;  //x: row [1]col
    Node *next;
    Node():next(0){
        data.x=-1; data.y=-1;
    };
    Node(int r, int c):next(0){
        data.x=r;  data.y=c;
    };
};

class Queue{
public:
    Node *head;
    Node *tail;

    Queue():head(0),tail(0){};
    void push(Pair p){
        int x=p.x;  int y=p.y;
        if(isEmpty()){
            head=new Node(x,y);
            tail=head;
        }
        else{
            Node *newNode=new Node(x,y);
            tail->next=newNode;
            tail=newNode;
        }
    }
    void push(int x, int y){
        if(isEmpty()){
            head=new Node(x,y);
            tail=head;
        }
        else{
            Node *newNode=new Node(x,y);
            tail->next=newNode;
            tail=newNode;
        }
    }
    void pop(){
        if(!isEmpty()){
            Node *deleteNode=head;
            head=head->next;
            if(head==NULL) tail=NULL;
            delete deleteNode;
            //deleteNode=NULL;
        }
    }
    bool isEmpty(){
        if(head==NULL && tail==NULL)
            return true;
        else
            return false;
    }
    Pair getHead(){
        return head->data;
    }
};

class Stack{
public:
    Node* top;
    Stack():top(0){};
    void push(int x, int y){
        if(isEmpty()){
            top=new Node(x,y);
        }
        else{
            Node* newNode= new Node(x,y);
            newNode->next=top;
            top=newNode;
        }
    }

    void pop(){
        Node* deleteNode=top;
        top=top->next;
        delete deleteNode;
        //deleteNode=NULL;
    }

    bool isEmpty(){
        if(top==NULL)   return true;
        else    return false;
    }

    Pair getTop(){
        return top->data;
    }

};

Stack calculateDist();
Stack findPath_go(Pair mid);
Queue findPath_back(Pair mid);

int main()
{
    int i, j;
    char x;
    int _m, _n, _B;

    ifstream inFile("floor.data",ios::in);
    if(!inFile){
        cout<<"cannot open floor.data"<<endl;
        return 1;
    }

    ///讀m n B
    inFile>>noskipws; //讀取空格
    while(1){
        inFile>>x;
        if(x==' ') break;
        _m=x-'0';
        m=m*10+_m;
    }

    while(1){
        inFile>>x;
        if(x==' ') break;
        _n=x-'0';
        n=n*10+_n;
    }

    while(1){
        inFile>>x;
        if(x=='\n'||x==' ') break;
        _B=x-'0';
        B=B*10+_B;
    }
    if(x==' '){
        do inFile>>x;
        while(x!='\n');
    }

    cout<<"m="<<m<<",n="<<n<<",B="<<B<<endl;

    Map=new bool* [m];
    for(i=0;i<m;i++)
        Map[i]=new bool[n];
    dist=new int* [m];
    for(i=0;i<m;i++)
        dist[i]=new int[n];
    visited=new bool* [m];
    for(i=0;i<m;i++)
        visited[i]=new bool[n];
    for(i=0;i<m;i++)
        for(j=0;j<n;j++){
            dist[i][j]=-1;
            visited[i][j]=0;
        }

    ///initialize final.path
    ofstream outFile("final.path",ios::out);
    if(!outFile){
        cout<<"cannot open final.path"<<endl;
        return 1;
    }
    outFile<<"                    "<<endl;

    ///construct Map[][]
    for(i=0;i<m;i++){
        for(j=0;j<=n;j++){
            inFile>>x;
            if((x=='\n'||x==' ')&& j!=n) inFile>>x; //濾掉不應出現的'\n' or ' '
            if(j!=n){           //濾掉'\n'
                if(x=='1')
                    Map[i][j]=1;
                else if(x=='0')
                    Map[i][j]=0;
                else if(x=='R'){
                    Map[i][j]=1;
                    R_row=i; R_col=j;
                }

                if(x=='1') visited[i][j]=true;
            }
            while(x==' '){      //濾掉後面出現的空格
                inFile>>x;
            }
        }
    }

    ///order: 找路的順序；裡面順便算dist[][]
    Stack order=calculateDist();
    if(2*dist[order.getTop().x][order.getTop().y]>B) printf("ERROR!!!\n");
    cout<<"at least "<<2*dist[order.getTop().x][order.getTop().y]<<endl;

    ///start to clean floor
    //Queue path;
    int total=0;

    Stack sub_path_go;
    Queue sub_path_back;

    while(!order.isEmpty()){
        Pair cur;
        if(!visited[order.getTop().x][order.getTop().y]){
            cur=order.getTop();
            sub_path_go=findPath_go(cur);
            while(!sub_path_go.isEmpty()){
                outFile<<sub_path_go.getTop().x<<" "<<sub_path_go.getTop().y<<endl;
                sub_path_go.pop();
                total++;
                //if(total==INT_MAX) cout<<"BIG"<<endl;
            }
            sub_path_back=findPath_back(cur);
            while(!sub_path_back.isEmpty()){
                //最後一個是R 但在中間的時候不用印出R (會重複 因為go一開始就有R 若不排除 會印出兩個R
                if(sub_path_back.getHead().x!=R_row||sub_path_back.getHead().y!=R_col){
                    outFile<<sub_path_back.getHead().x<<" "<<sub_path_back.getHead().y<<endl;
                    total++;
                    //if(total==INT_MAX) cout<<"BIG"<<endl;
                }
                sub_path_back.pop();
            }
            order.pop();
        }
        else
            order.pop();


    }

    cout<<"total "<<total<<endl;

    outFile.seekp(ios::beg);
    outFile<<total;
///*
    //check finish clean
    for(i=0;i<m;i++)
        for(j=0;j<n;j++){
            if(!visited[i][j]){
                cout<<"NOOOO"<<endl;
                break;
            }
        }
    cout<<"GREAT"<<endl;
//*/
    return 0;
}

Stack calculateDist(){
    Stack s;
    Queue que;
    que.push(R_row,R_col);
    dist[R_row][R_col]=0;
    s.push(R_row,R_col);
    while(!que.isEmpty()){
        Pair p=que.getHead();
        que.pop();
        if (p.y + 1 < n && Map[p.x][p.y + 1] == 0 && dist[p.x][p.y + 1] == -1) {
			dist[p.x][p.y + 1] = dist[p.x][p.y] + 1;
			que.push(p.x, p.y + 1);
			s.push(p.x, p.y + 1);
		}
		if (p.x + 1 < m && Map[p.x + 1][p.y] == 0 && dist[p.x + 1][p.y] == -1) {
			dist[p.x + 1][p.y] = dist[p.x][p.y] + 1;
			que.push(p.x + 1, p.y);
			s.push(p.x + 1, p.y);
		}
		if (p.y - 1 >= 0 && Map[p.x][p.y - 1] == 0 && dist[p.x][p.y - 1] == -1) {
			dist[p.x][p.y - 1] = dist[p.x][p.y] + 1;
			que.push(p.x, p.y - 1);
			s.push(p.x, p.y - 1);
		}
		if (p.x - 1 >= 0 && Map[p.x - 1][p.y] == 0 && dist[p.x - 1][p.y] == -1) {
			dist[p.x - 1][p.y] = dist[p.x][p.y] + 1;
			que.push(p.x - 1, p.y);
			s.push(p.x - 1, p.y);
		}
    }

    return s;
}

Stack findPath_go(Pair mid){
    //go (hence stack pop出來的是按照順序)
    Stack sub_path;
    Pair cur;
    cur=mid;
    visited[cur.x][cur.y]=true;
    sub_path.push(cur.x, cur.y);
    while(1){
        //cout<<cur.x<<" "<<cur.y<<endl;
        if(cur.x==R_row && cur.y==R_col) break;

        //go單純走一條就好，先不要判斷可不可以多走

        //先看沒有走過的
        if(cur.y+1<n && !visited[cur.x][cur.y+1] && dist[cur.x][cur.y+1]==dist[cur.x][cur.y]-1){
            visited[cur.x][cur.y+1]=true;
            sub_path.push(cur.x, cur.y+1);
            cur.y=cur.y+1;
            continue;
        }
        else if(cur.x+1<m && !visited[cur.x+1][cur.y] && dist[cur.x+1][cur.y]==dist[cur.x][cur.y]-1){
            visited[cur.x+1][cur.y]=true;
            sub_path.push(cur.x+1, cur.y);
            cur.x=cur.x+1;
            continue;
        }
        else if(cur.y-1>=0 && !visited[cur.x][cur.y-1] && dist[cur.x][cur.y-1]==dist[cur.x][cur.y]-1){
            visited[cur.x][cur.y-1]=true;
            sub_path.push(cur.x, cur.y-1);
            cur.y=cur.y-1;
            continue;
        }
        else if(cur.x-1>=0 && !visited[cur.x-1][cur.y] && dist[cur.x-1][cur.y]==dist[cur.x][cur.y]-1){
            visited[cur.x-1][cur.y]=true;
            sub_path.push(cur.x-1, cur.y);
            cur.x=cur.x-1;
            continue;
        }

        //再看已經清過的
        if(cur.y+1<n && dist[cur.x][cur.y+1]==dist[cur.x][cur.y]-1){
            sub_path.push(cur.x, cur.y+1);
            cur.y=cur.y+1;
            continue;
        }
        else if(cur.x+1<m && dist[cur.x+1][cur.y]==dist[cur.x][cur.y]-1){
            sub_path.push(cur.x+1, cur.y);
            cur.x=cur.x+1;
            continue;
        }
        else if(cur.y-1>=0 && dist[cur.x][cur.y-1]==dist[cur.x][cur.y]-1){
            sub_path.push(cur.x, cur.y-1);
            cur.y=cur.y-1;
            continue;
        }
        else if(cur.x-1>=0 && dist[cur.x-1][cur.y]==dist[cur.x][cur.y]-1){
            sub_path.push(cur.x-1, cur.y);
            cur.x=cur.x-1;
            continue;
        }
    }
    return sub_path;
}

Queue findPath_back(Pair mid){
    //go (hence stack pop出來的是按照順序)
    int will_use_step;
    Queue sub_path;
    Pair cur;
    will_use_step=2*dist[mid.x][mid.y]; //at least
    cur=mid;
    while(1){
        if(cur.x==R_row && cur.y==R_col) break;

        //先判斷可不可以多走
        if(will_use_step+2<=B){
            if(cur.y+1<n && !visited[cur.x][cur.y+1] && dist[cur.x][cur.y+1]>=dist[cur.x][cur.y]){
                will_use_step+=2;
                visited[cur.x][cur.y+1]=true;
                sub_path.push(cur.x, cur.y+1);
                cur.y=cur.y+1;
                continue;
            }
            else if(cur.x+1<m && !visited[cur.x+1][cur.y] && dist[cur.x+1][cur.y]>=dist[cur.x][cur.y]){
                will_use_step+=2;
                visited[cur.x+1][cur.y]=true;
                sub_path.push(cur.x+1, cur.y);
                cur.x=cur.x+1;
                continue;
            }
            else if(cur.y-1>=0 && !visited[cur.x][cur.y-1] && dist[cur.x][cur.y-1]>=dist[cur.x][cur.y]){
                will_use_step+=2;
                visited[cur.x][cur.y-1]=true;
                sub_path.push(cur.x, cur.y-1);
                cur.y=cur.y-1;
                continue;
            }
            else if(cur.x-1>=0 && !visited[cur.x-1][cur.y] && dist[cur.x-1][cur.y]>=dist[cur.x][cur.y]){
                will_use_step+=2;
                visited[cur.x-1][cur.y]=true;
                sub_path.push(cur.x-1, cur.y);
                cur.x=cur.x-1;
                continue;
            }
        }

        //if B不夠多去多走 or B夠多去走但附近dist較大的格子都已經被清過了
        //先看沒有走過的
        if(cur.y+1<n && !visited[cur.x][cur.y+1] && dist[cur.x][cur.y+1]==dist[cur.x][cur.y]-1){
            visited[cur.x][cur.y+1]=true;
            sub_path.push(cur.x, cur.y+1);
            cur.y=cur.y+1;
            continue;
        }
        else if(cur.x+1<m && !visited[cur.x+1][cur.y] && dist[cur.x+1][cur.y]==dist[cur.x][cur.y]-1){
            visited[cur.x+1][cur.y]=true;
            sub_path.push(cur.x+1, cur.y);
            cur.x=cur.x+1;
            continue;
        }
        else if(cur.y-1>=0 && !visited[cur.x][cur.y-1] && dist[cur.x][cur.y-1]==dist[cur.x][cur.y]-1){
            visited[cur.x][cur.y-1]=true;
            sub_path.push(cur.x, cur.y-1);
            cur.y=cur.y-1;
            continue;
        }
        else if(cur.x-1>=0 && !visited[cur.x-1][cur.y] && dist[cur.x-1][cur.y]==dist[cur.x][cur.y]-1){
            visited[cur.x-1][cur.y]=true;
            sub_path.push(cur.x-1, cur.y);
            cur.x=cur.x-1;
            continue;
        }

        //再看已經清過的
        if(cur.y+1<n && dist[cur.x][cur.y+1]==dist[cur.x][cur.y]-1){
            sub_path.push(cur.x, cur.y+1);
            cur.y=cur.y+1;
            continue;
        }
        else if(cur.x+1<m && dist[cur.x+1][cur.y]==dist[cur.x][cur.y]-1){
            sub_path.push(cur.x+1, cur.y);
            cur.x=cur.x+1;
            continue;
        }
        else if(cur.y-1>=0 && dist[cur.x][cur.y-1]==dist[cur.x][cur.y]-1){
            sub_path.push(cur.x, cur.y-1);
            cur.y=cur.y-1;
            continue;
        }
        else if(cur.x-1>=0 && dist[cur.x-1][cur.y]==dist[cur.x][cur.y]-1){
            sub_path.push(cur.x-1, cur.y);
            cur.x=cur.x-1;
            continue;
        }
    }
    return sub_path;
}
