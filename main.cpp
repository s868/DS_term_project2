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