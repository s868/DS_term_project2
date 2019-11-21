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