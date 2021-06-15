#include <iostream>
#include <vector>

using namespace std;

#define M 64

class Node{
public:
	Node(uint64_t b){
		board = b;
		childrenVec.push_back(NULL);
		root = NULL;
		wins = 0;
		total = 0;
		move = 0;
	}
	uint64_t board; //current state of the board
	int wins;   //how many times this action brought a win
	int total;  //total times the board was at this state
	int move;    //action that player made 
	Node* root;
	vector<Node*> childrenVec;  //some possible moves from current board state
};


class MCTS{
	public:
		MCTS():root(NULL), size(0){}

		//int get_move(){return move;}
		void insert(uint64_t b);
		void print_tree(){print_tree(root, 0);};

	private:
		Node* root;
		Node* current;
		int size;
		
		void print_tree(Node* p, int i);


};


void MCTS::insert(uint64_t b){
	Node* t = new Node(b);
	if(size==0)root=t;
	else{
		current->childrenVec[0] = t;
	}
	//update
	current = t;
	size++;

}


void MCTS::print_tree(Node* p, int i){
	if(p==NULL || p->childrenVec.size() < i){
		return;
	}else{ 
		bitset<M> m = p->board;
		print_tree(p->childrenVec[i], i);
		cout << m << endl;
		i++;
	}

}