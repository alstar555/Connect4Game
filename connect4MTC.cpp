#include <iostream>
#include<string>
#include<vector>
#include<random>
#include <typeinfo> 
#include <unistd.h>
#include <bitset>
#include <bit>
#include <climits>
#include <stdint.h>

using namespace std;

#define DEBUG 0
#define M 64
#define connectN 4
#define sleeptime 0
#define WIDTH 9
#define HEIGHT 7


//reset game
void newGame(uint64_t &bitboard, uint64_t &playerBitboard, uint64_t &botBitboard, char &playerId, char &botId){
	bitboard = 0;
	playerBitboard = 0;
	botBitboard = 0;
	//X goes first and 0 goes second
	playerId=(rand()%1 == 0) ? 'X':'O'; 
	botId = (playerId=='X') ? 'O':'X';
	cout << "\t\t\tNEW GAME ... you: " << playerId << " bot: " << botId;
}

void printBitboard(uint64_t &playerBitboard, uint64_t &botBitboard, char playerId, char botId, int width, int height){
	cout << endl;
	bitset<M> p = playerBitboard;
	bitset<M> b = botBitboard;
	int index = height*width;
	cout << "player bitboard: " << p << endl;
	cout << "bot bitboard:    " << b << endl;
	cout << endl;
	for(int i=0; i!=width; i++){
		cout << i << " ";
	}
	cout <<  endl;
	for(int i = 0; i!= height;i++){
		for(int j = 0; j!= width;j++){
			if(p[index] == 1){
				cout << "\x1b[31m" << playerId<< "\x1b[0m ";
			}
			else if(b[index] == 1){
				cout << "\x1b[34m" << botId << "\x1b[0m ";
			}
			else{
				cout << ". ";
			}
			index--;
		}
		cout << endl;
	}
}


//add player's move to board vector
void updateBoard(vector<string>& board, int move, char id){
	move*=2;
	for(int i=0; i != board.size();i++){
		if(i==board.size()-1 || board[i+1][move] != '.' ){
			board[i][move] = id;
			break;
		}
	}
}


void updateBitboard(uint64_t &board, uint64_t &playerBoard, int move, int width, int height){
	uint64_t mask = 1 << width-move;
	uint64_t masked_board =1;
	for(int i=0;i!=width*height;i++){
		masked_board = board & mask;
		if(masked_board==0){
			break;
		}
		mask = mask << width;
	}
	board ^= mask;
	playerBoard ^=mask;
	bitset<M> b = board;
}

//count total 1s in bit
size_t popcount(size_t n) {
    std::bitset<sizeof(size_t) * CHAR_BIT> b(n);
    return b.count();
}


//check if there is 4 in row on board 0=no winner, 1=winner;
int checkWinner(uint64_t board, int move, int width, int height, char id){
	uint64_t mask = 0; 
	uint64_t mask2 = 0;
	uint64_t mask3 = 0;
	bitset<M> m;
	uint64_t masked_board;
    
	//create mask for 4 in row 0b1111
	for(int i=0; i!=connectN;i++){
		mask <<= 1;
		mask ^= 1;
	}
	//create mask width of board 0b1111111
	for(int i=0; i!=width+1;i++){
		mask2 <<= 1;
		mask2 ^= 1;
	}
	//4 in row horizontal
	for(int i=0; i!=(height*width);i++){
		masked_board = board & mask & mask2;
		mask <<= 1;
		if(i!= 0 && i%(width) ==0){
			mask2 <<= width;
		}
		//check 4 in row after masking
		if(popcount(masked_board) == connectN){
			printBitboard(masked_board, masked_board, id, id, width, height); 
			return 1;
		}
	}

	//4 in row vertically
	mask = 1; //...00000001
	mask2 = 1;
	mask3; //111111111111111
	//create mask with height of 4 
	for(int i=0; i!=width*connectN;i++){
		mask3 <<= 1;
		mask3 ^= 1;
	}
	masked_board = 0;
	int index = 0;
	for(int i=0; i!=(height-connectN+1);i++){
		for(int j=0; j!=width;j++){  
			for(int i=0; i!=height;i++){
				masked_board += board & mask &mask3;
#if DEBUG 
				m = board;
				cout << "board:        " << m << endl;
				m = mask;int boardWidth = 9;
	// int boardHeight = 7;
				cout << "mask:         " << m << endl;
				m = mask3;
				cout << "mask3:        " << m << endl;
				m = masked_board;
				m = masked_board;
				cout << "masked_board: " << m << endl;
				cout << endl;
#endif	
				mask <<= width;
				//check 4 in row after masking
				if(popcount(masked_board) == connectN){
					printBitboard(masked_board, masked_board, id, id, width, height); 
					return 1;
				}
			}
			mask2<<=1;
			mask = mask2;
			masked_board = 0;
		}
		mask2 = 1;
		mask = mask2;
		mask3 <<=width;
	}

	return 0;
}



//MCT class
class MCT{
	public:

	private:

};





int main(){
	//defining board
	int boardWidth;
	int boardHeight;
	cout << "board width: ";
	cin >> boardWidth;
	cout << "board height: ";
	cin >> boardHeight;
	if(boardWidth<4 || boardWidth>12){
		cout << "invalid board width" << endl;
		boardWidth = WIDTH;
	}
	if(boardHeight<4 || boardHeight>8){
		cout << "invalid board heigth" << endl;
		boardHeight = HEIGHT;
	}

	uint64_t bitboard;
	uint64_t playerBitboard;
	uint64_t botBitboard;
	int playerMove;
	int botMove;
	int playerScore=0;
	int botScore =0;
	char playerId; 
	char botId;


	newGame(bitboard, playerBitboard, botBitboard, playerId, botId);
	//game loop
	while(1){
		//printBoard(board);
		printBitboard(playerBitboard, botBitboard, playerId, botId, boardWidth, boardHeight);
		cout << "enter index: ";
		cin >> playerMove;
		//decide bot's move
		botMove = rand()%7;

		//PLAYER goes first bot second
		if(playerId == 'X'){
			updateBitboard(bitboard, playerBitboard, playerMove, boardWidth, boardHeight);
			sleep(sleeptime);
			updateBitboard(bitboard, botBitboard, botMove, boardWidth, boardHeight);
			printBitboard(playerBitboard, botBitboard, playerId, botId, boardWidth, boardHeight);
		}
		else{ //BOT goes first, player second
			updateBitboard(bitboard, botBitboard, botMove, boardWidth, boardHeight);
			updateBitboard(bitboard, playerBitboard, playerMove, boardWidth, boardHeight);
			printBitboard(playerBitboard, botBitboard, playerId, botId, boardWidth, boardHeight);
			sleep(sleeptime);
		}
		
		//check for connect 4
		if(checkWinner(playerBitboard, playerMove, boardWidth, boardHeight, playerId)){
			cout << "\t\t\t\tYOU WIN!" << endl;
			playerScore ++;
			cout << "\t\t\tscore:   you: " << playerScore << "  bot: " << botScore << endl;
			newGame(bitboard, playerBitboard, botBitboard, playerId, botId);
		}
		else if (checkWinner(botBitboard, botMove, boardWidth, boardHeight, botId)){
			cout << "\t\t\t\tYOU LOOSE :(" << endl;
			botScore ++;
			cout << "\t\t\tscore:   you: " << playerScore << "  bot: " << botScore << endl;
			newGame(bitboard, playerBitboard, botBitboard, playerId, botId);
		}
	}
}