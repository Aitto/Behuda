/*
Author @Aitto_sang
CSE@Buet
*/
#include<stdio.h>
#include<iostream>
//#include<ctime>
//#include<cmath>
//#//include<cstdlib>
#include<algorithm>
//#include<map>
//#include<stack>
//#include<cstring>
//#include<bits/stdc++.h>

#define lli long long int
#define pf printf
#define sf scanf
#define pi 3.141592653589793116
#define inf 1e9
#define linf 1e18
#define NULL_VALUE -999999
#define maxNEG -inf
#define debug cout<<"Hello here!"<<endl;
#define readFile freopen("in.txt","r",stdin);
#define syncronize ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
#define red 1
#define black 2
#define doubleBlack 3
#define nullNode -176539

using namespace std;

struct nodes{
    nodes* parent;
    nodes* left;
    nodes* right;
    int color;
    int data;
};

class Tree{
    int length;
    nodes *head;

    int height(nodes *node){
        if(node==0) return 0;
        int l=height(node->left);
        int r=height(node->right);
        return l>r?l+1:r+1;
    }

    void inorder(nodes* root){
        if(root==0) return;
        inorder(root->left);
        cout<<root->data;
        if(root->color==black) cout<< "B\n";
        else cout<< "R\n";
        inorder(root->right);
    }

    void print(nodes *node,int height){
        if(node==0) return;
        print(node->left,height+1);

        for(int i=0;i<height;i++) cout<< "  ";
        cout<<node->data;
        if(node->color==black) cout<< "B\n";
        else cout<<"R\n";

        print(node->right,height +1);
    }

    nodes *getNodeofADta(nodes* root,int x){
        if(root==0 or root->data==x) return root;
        if(x < root->data) return getNodeofADta(root->left,x);
        else return getNodeofADta(root->right,x);
    }

    void rightRotate(nodes *root,bool switchColor){
        nodes *parent=root->parent;
        root->parent=parent->parent;
        if(parent->parent!=0){
            if(parent->parent->left==parent)
                parent->parent->left=root;
            else parent->parent->right=root;
        }else head=root;

        nodes *right=root->right;
        root->right=parent;
        parent->parent=root;
        parent->left=right;
        if(right!=0) right->parent=parent;

        if(switchColor){
            if(root->color==red) root->color=black;
            else root->color=red;
            if(parent->color==black) parent->color=red;
            else parent->color=black;
        }
    }

    //Return sibling of a node
    nodes* getSibling(nodes *root){
        if(root->parent==0) return root;
        return root->parent->left==root?root->parent->right:root->parent->left;
    }

    void leftRotate(nodes *root,bool switchColor){
        nodes *parent=root->parent;
        root->parent=parent->parent;
        if(parent->parent!=0){
            if(parent->parent->left==parent)
                parent->parent->left=root;
            else parent->parent->right=root;
        }else head=root;

        nodes *left=root->left;
        root->left=parent;
        parent->parent=root;
        parent->right=left;
        if(left!=0) left->parent=parent;
        if(switchColor){
            if(root->color==red) root->color=black;
            else root->color=red;
            if(parent->color==black) parent->color=red;
            else parent->color=black;
        }
    }

    void LL(nodes *root){
        rightRotate(root->parent,true);
    }

    void RL(nodes *root){
        leftRotate(root,false);
        rightRotate(root,true);
    }

    void RR(nodes *root){
        leftRotate(root->parent,true);
    }

    void LR(nodes* root){
        rightRotate(root,false);
        leftRotate(root,true);
    }

    void balanceHeight(nodes *root){
        if(root->parent==0) return;

        nodes* parent=root->parent;
        if(root->color==red and parent->color==red){
            nodes* sibling=getSibling(parent);
            if(sibling==0 or sibling->color==black){
                if(parent->left==root){
                    if(parent->parent->left==parent) LL(root);
                    else LR(root);
                }else{
                    if(parent->parent->left==parent) RL(root);
                    else RR(root);
                }
            }else{
                if(head==parent->parent){
                    parent->color=black;
                    sibling->color=black;
                }else{
                    parent->color=black;
                    sibling->color=black;
                    parent->parent->color=red;
                    balanceHeight(parent->parent);
                }
            }
        }
    }

    int BH(nodes *node){
        if(node==0) return 0;
        //As black height of a red black tree is same in all paths, we traverse through left path only
        int l=BH(node->left);

        return node->color==black?l+1:l;
    }

    nodes *getInorderSuccessorofANode(nodes *root){
        if(root==0 or root->left==0) return root;
        return getInorderSuccessorofANode(root->left);
    }

    nodes* getSuccessor(nodes *root){
        return getInorderSuccessorofANode(root->right);
    }

    int getCase(nodes *root){
        if(root==head) return 1;
        nodes *parent=root->parent;
        nodes *sibling=getSibling(root);
        if(parent->color==black){
            //Parent is black
            if(sibling==0) return 3;
            //sibling is red
            if(sibling->color==red) return 2;
            else{
                //Sibling black
                //Both children of black sibling are black
                if((sibling->left==0 or sibling->left->color==black ) and (sibling->right==0 or sibling->right->color==black)) return 3;
                //left child of sibling is red
                if((sibling->right==0 or sibling->right->color==black) and sibling->left->color==red ) return 5;
                //right child of sibling is red
                if(sibling->right!=0) if(sibling->right->color==red) return 6;
            }
        }else{
            //Parent is red
            if(sibling==0) return 4;
            //Both children of black sibling are black
            if((sibling->left==0 or sibling->left->color==black) and (sibling->right==0 or sibling->right->color==black) ) return 4;
            if(sibling->right!=0){
                if(sibling->right->color==red) return 6;
            }
        }
        return -1;
    }

    void deleteDoubleBlack(nodes *root){
        cout<< "go go"<<endl;
        int caseNumber=getCase(root);
        if(caseNumber==-1){
            cout<< "Can't delete\n";
            return;
        }

        switch(caseNumber){
        case 1:
            {
                cout<< "Case 1"<<endl;
                if(root->data==nullNode){
                    head=0;
                    free(root);
                }
                else{
                    root->color=black;
                }
                return;
            }
        case 2:
            {
                cout<< "Case 2"<<endl;
                nodes *sibling=getSibling(root);
                leftRotate(sibling,true);
                deleteDoubleBlack(root);
                break;
            }
        case 3:
            {
                cout<< "Case 3"<<endl;
                nodes *sibling=getSibling(root);
                if(sibling!=0) sibling->color=red;
                root->parent->color=doubleBlack;
                if(root->data==nullNode){
                    root->parent->left==root? root->parent->left=0:root->parent->right=0;
                    deleteDoubleBlack(root->parent);
                    free(root);
                }else root->color=black;
                break;
            }
        case 4:
            {
                cout<< "Case 4"<<endl;
                nodes *parent=root->parent;
                nodes *sibling=getSibling(root);
                parent->color=black;
                if(sibling!=0) sibling->color=red;
                if(root->data==nullNode){
                    parent->left==root?parent->left=0:parent->right=0;
                    free(root);
                }else root->color=black;
                return;
            }
        case 5:
            {
                cout<< "Case 5"<<endl;
                nodes *parent=root->parent;
                nodes *sibling=getSibling(root);
                rightRotate(sibling->left,true);
                deleteDoubleBlack(root);
                break;
            }
        case 6:
            {
                cout<< "Case 6"<<endl;
                nodes *sibling=getSibling(root);
                nodes *parent=root->parent;
                leftRotate(sibling,true);
                sibling->right->color=black;
                if(root->data==nullNode){
                    parent->left==root?parent->left=0:parent->right=0;
                    free(root);
                }else root->color=black;
                return;
            }
        }
    }

public:
    Tree(){
        length=0;
        head=0;
    }

    void insert(int x){
        if(head==0){
            nodes* newnode=new nodes;
            newnode->parent=0;
            newnode->left=0;
            newnode->right=0;
            newnode->color=black;
            newnode->data=x;
            head=newnode;

            length++;
        }else{
            nodes* newnode=new nodes;
            newnode->data=x;
            newnode->left=0;
            newnode->right=0;
            newnode->color=red;

            nodes* temp=head;

            while(true){
                if(temp->data>x){
                    if(temp->left==0){
                        newnode->parent=temp;
                        temp->left=newnode;
                        //cout<<"Inserted! now balancing 1"<<endl;
                        balanceHeight(newnode);
                        return;
                    }
                    temp=temp->left;
                }else{
                    if(temp->right==0){
                        newnode->parent=temp;
                        temp->right=newnode;
                        //cout<<"Inserted! Now balancing 2"<<endl;
                        balanceHeight(newnode);
                        return;
                    }
                    temp=temp->right;
                }
            }
        }
    }

    void deleteNode(int x){
        debug
        nodes* delNode=getNode(x);

        state:

        if(delNode==0) {
            cout<< "terminating"<<endl;
            return;
        }
        if(delNode->left!=0 and delNode->right!=0){
             nodes* temp=getSuccessor(delNode);
             delNode->data=temp->data;
             //temp->data=x;
             cout<< "Got two children"<<endl;
             delNode=temp;
             goto state;
             //deleteNode(temp->data);
        }else if(delNode->color==red){
            nodes *child;
            //Checking if any child exist
            if(delNode->left==0) child=delNode->right;
            else child=delNode->left;
            //Fixing parent child relation
            nodes *parent=delNode->parent;
            if(child!=0)child->parent=parent;
            if(parent!=0) parent->left==delNode?parent->left=child:parent->right=child;
            else head=child; //if parent is zero means thats the root node;
            free(delNode);
            cout<< "Got red node"<<endl;
        }
        else{
            if(delNode->left!=0){
                if(delNode->left->color==red){
                    nodes *child=delNode->left;
                    nodes *parent=delNode->parent;

                    child->color=delNode->color;
                    child->parent=parent;
                    if(parent!=0) parent->left==delNode?parent->left=child:parent->right=child;
                    else head=child;
                    free(delNode);
                    cout<< "Got left red child"<<endl;
                    return;
                }
            }
            if(delNode->right!=0){
                if(delNode->right->color==red){
                    nodes *child=delNode->right;
                    nodes *parent=delNode->parent;

                    child->color=delNode->color;
                    child->parent=parent;
                    if(parent!=0) parent->left==delNode?parent->left=child:parent->right=child;
                    else head=child;
                    free(delNode);
                    cout<< "Got right red child"<<endl;
                    return;
                }
            }
            delNode->color=doubleBlack;
            delNode->data=nullNode;
            deleteDoubleBlack(delNode);
        }
    }

    int blackHeight(){
        return BH(head);
    }

    void inorderPrint(){
        cout<< "Printing tree in-order:\n";
        inorder(head);
    }

    void printTree(){
        cout<< "Printing the tree:\n";
        print(head,0);
    }

    int totalNodes(){
        return length;
    }

    nodes* getNode(int x){
        return getNodeofADta(head,x);
    }
};

int main(){

    Tree tree;

    int n,x;

    while(1){
        cout<< "1.insert 2.delete 3.search 4.Print Tree 5.Print Inorder data 6.exit"<<endl;
        cin>>n;
        switch(n){
        case 1:
            {
                cin>>x;
                tree.insert(x);
                tree.printTree();
                break;
            }
        case 2:
            {
                cin>>x;
                tree.deleteNode(x);
                tree.printTree();
                break;
            }
        case 3:
            {
                cin>>x;
                nodes *temp=tree.getNode(x);
                if(temp==0) cout<<"Data not found"<<endl;
                else cout<<"Data found"<<endl;
                break;
            }
        case 4:
            {
                tree.printTree();
                break;
            }
        case 5:
            {
                tree.inorderPrint();
                break;
            }
        case 6:
            {
                exit(1);
            }
        }
    }

    return 0;
}
/**
8
10 20 -10 15 17 40 50 60

1 10 1 20 1 -10 1 15 1 17 1 40 1 50 1 60
*/
