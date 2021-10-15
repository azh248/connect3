#include <iostream>

std::pair<std::pair<int, int>, std::pair<int, int>> dp[43046721][2]; // dp[i][j] = the next move in the form of {{index of row to place a piece, index of column to 
					 // place a piece}, {winner of game (0 for tie, 1 for program, 2 for user), number of moves until the game ends}}
					 // if the hashed board is i (when converted to base 3, the indices of the resulting string corresponding 
					 // to places in the board are as follows: 
					 // 0 1 2 3
					 // 4 5 6 7
					 // 8 9 10 11
					 // 12 13 14 15)
					 // these values can be either 0 (not filled), 1 (filled with user's color), or 2 (filled with program's color)
					 // and the player is j (0 for user, 1 for the program)

bool playerWon(std::string boardString) { // function to determine if a player won from a board
	int boardArr[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			boardArr[i][j] = boardString[4 * i + j] - '0';
		}
	}
	// check if either player has won

	// checking if either player won with 3 in a row horizontally
	for (int i = 0; i < 4; i++) {
		if (boardArr[i][0] != 0 && boardArr[i][0] == boardArr[i][1] && boardArr[i][1] == boardArr[i][2]) {
			return true; // the other player won because it was their move previously
		} else if (boardArr[i][1] != 0 && boardArr[i][1] == boardArr[i][2] && boardArr[i][2] == boardArr[i][3]) {
			return true; // the other player won
		}
	}

	// checking if either player won with 3 in a row vertically
	for (int j = 0; j < 4; j++) {
		if (boardArr[0][j] != 0 && boardArr[0][j] == boardArr[1][j] && boardArr[1][j] == boardArr[2][j]) {
			return true;
		} else if (boardArr[1][j] != 0 && boardArr[1][j] == boardArr[2][j] && boardArr[2][j] == boardArr[3][j]) {
			return true;
		}
	}
	// checking if either player won with 3 in a row diagonally

	// diagonal in the direction of (0, 0) to (3, 3)
	if (boardArr[0][0] != 0 && boardArr[0][0] == boardArr[1][1] && boardArr[1][1] == boardArr[2][2]) {
		return true;
	} else if (boardArr[1][1] != 0 && boardArr[1][1] == boardArr[2][2] && boardArr[2][2] == boardArr[3][3]) {
		return true;
	} else if (boardArr[1][0] != 0 && boardArr[1][0] == boardArr[2][1] && boardArr[2][1] == boardArr[3][2]) {
		return true;
	} else if (boardArr[0][1] != 0 && boardArr[0][1] == boardArr[1][2] && boardArr[1][2] == boardArr[2][3]) {
		return true;
	}

	// diagonal in the direction of (3, 0) to (0, 3)
	if (boardArr[3][0] != 0 && boardArr[3][0] == boardArr[2][1] && boardArr[2][1] == boardArr[1][2]) {
		return true;
	} else if (boardArr[2][1] != 0 && boardArr[2][1] == boardArr[1][2] && boardArr[1][2] == boardArr[0][3]) {
		return true;
	} else if (boardArr[2][0] != 0 && boardArr[2][0] == boardArr[1][1] && boardArr[1][1] == boardArr[0][2]) {
		return true;
	} else if (boardArr[3][1] != 0 && boardArr[3][1] == boardArr[2][2] && boardArr[2][2] == boardArr[1][3]) {
		return true;
	}

	// if none of the above conditions are satisfied no player has won yet
	return false;
}

bool takeMove(std::pair<std::pair<int, int>, std::pair<int, int>> move, std::pair<std::pair<int, int>, std::pair<int, int>> newMove, int player) {
	// conditions in which we should use this move

	// the best move is a loss and this move is a loss but this move takes longer to lose than the best move
	if (newMove.second.first == move.second.first && newMove.second.second > move.second.second && newMove.second.first == ((player + 1) % 2) + 1) {
		return true;
	}
	// both moves are a win but this move takes a shorter time to win
	if (newMove.second.first == move.second.first && newMove.second.second < move.second.second && newMove.second.first == player + 1) {
		return true;
	}
	// both moves are a tie but this move takes a longer time to tie
	if (newMove.second.first == move.second.first && newMove.second.second < move.second.second && newMove.second.first == 0) {
		return true;
	}
	// this move is a win and the best move isn't a win
	if (newMove.second.first == player + 1 && move.second.first != player + 1) {
		return true;
	}
	// there isn't a valid move yet, we have to go with this one
	if (move.second.first == -1) {
		return true;
	}
	// if the move is a tie and the best move previously was a loss
	if (newMove.second.first == 0 && move.second.first == ((player + 1) % 2) + 1) {
		return true;
	}
	return false;
}

std::pair<std::pair<int, int>, std::pair<int, int>> nextMove(std::string boardString, int player) { // returns optimal move given a board and the current player
	int board = stoi(boardString, nullptr, 3);
	if (dp[board][player].first.first != -1) { // checks if this move has already been calculated before
		return dp[board][player];
	}

	// checking if the board is filled and the game is a tie, as well as how many moves there's been in hte game so far

	int moves = 0;
	for (int i = 0; i < 16; i++) { // finding out how many moves have passed by seeing how many pieces have been placed
		if (boardString[i] != '0') {
			moves++;
		}
	}

	if (playerWon(boardString)) { // function to determine if either player won
		// if a player won, then we return the result
		dp[board][player] = {{-1, -1}, {((player + 1) % 2) + 1, moves}};
		return {{-1, -1}, {((player + 1) % 2) + 1, moves}};
	}

	if (moves == 16) { // board is completely filled and neither player has won; the game is a draw
		dp[board][player] = {{-1, -1}, {0, moves}};
		return {{-1, -1}, {0, moves}};
	}

	// neither player has won and the board isn't filled; now we just try all possible moves and use the best one
	std::pair<std::pair<int, int>, std::pair<int, int>> move = {{-1, -1}, {-1, -1}};

	for (int j = 0; j < 4; j++) { 	
		for (int i = 3; i >= 0; i--) {
			if (boardString[4 * i + j] == '0') { // the first place a piece can fall into column j

				// temporarily changing the empty space to the player's piece 
				boardString.replace(4 * i + j, 1, std::to_string(player + 1)); 
				std::pair<std::pair<int, int>, std::pair<int, int>> newMove = nextMove(boardString, (player + 1) % 2);

				if (takeMove(move, newMove, player)) {
					move = {{i, j}, newMove.second};
				}
				// changing the board back to its original state
				boardString.replace(4 * i + j, 1, "0");
				break; // can't place anywhere above this because it'll just fall dow to this square
			}
		}
	}	
	dp[board][player] = move;
	return move;
}	

void printBoard(std::string board) { 
	for (int i = 0; i < 16; i++) { // converts board to player-friendly format
		if (board[i] == '0') {
			board[i] = '-';
		} else if (board[i] == '2') {
			board[i] = 'o';
		} else {
			board[i] = 'x';
		}
	}
	// prints the board's contents
	std::cout << board[0] << " " << board[1] << " " << board[2] << " " << board[3] << "\n";
	std::cout << board[4] << " " << board[5] << " " << board[6] << " " << board[7] << "\n";
	std::cout << board[8] << " " << board[9] << " " << board[10] << " " << board[11] << "\n";
	std::cout << board[12] << " " << board[13] << " " << board[14] << " " << board[15] << "\n";
}

int main() {
	// output directions
	std::cout << "Welcome to Connect 3! Connect 3 is a game similar to the popular game Connect 4. It's a 2 player game\n";
	std::cout << "played on a 4x4 board in which players may place tokens of their color at any of the 4 columns (labeled 1 to 4), \n";
	std::cout << "in which the token will fall down to the lowest possible empty spot. The first player to achieve 3 tokens \n";
	std::cout << "of their color in a row will win. This program will play against the user––the user will go first, \n";
	std::cout << "and then the user and program will alternate turns with the program printing the board after each of its moves. \n";
	std::cout << "The board will be labeled where each space is either '-' representing an empty space, 'o' representing \n";
	std::cout << "a token of the program's color, or 'x' representing a token of the user's color. Good luck! \n";
	std::cout << "---------------------------------------------------------------------------------------------------------\n";

	// initialize dp array
	for (int i = 0; i < 43046721; i++) {
		dp[i][0] = {{-1, -1}, {-1, -1}};
		dp[i][1] = {{-1, -1}, {-1, -1}};
	}
	// play according to user's input, after each move check if the game is
	// over yet
	std::string board = "0000000000000000";
	for (int i = 0; i < 8; i++) { // 8 moves total for the program, user goes first
		printBoard(board);
		std::cout << "What's your move? (enter the column in which you want to place the piece) \n";
		int col;
		std::cin >> col;
		col--;
		while (col < 0 || col > 3) {
			std::cout << "Invalid move. \n" << "What's your move? (enter the column in which you want to place the piece) \n";
			std::cin >> col;
			col--;
		}
		// simulating the user's move
		for (int i = 3; i >= 0; i--) {
			if (board[4 * i + col] == '0') {
				board[4 * i + col] = '1';
				break;
			} else if (i == 0) {
				std::cout << "Invalid move. \n" << "What's your move? (enter the column in which you want to place the piece) \n";
				std::cin >> col;
				col--;
				i = 4;
			}
		}
		// user won
		if (playerWon(board)) {
			std::cout << "Congratulations! You won! \n" << "The board was: \n";
			printBoard(board);
			break;
		}
		// optimal move for the program
		std::pair<std::pair<int, int>, std::pair<int, int>> move = nextMove(board, 0);

		// do optimal move
		board[4 * move.first.first + move.first.second] = '2';
		// check if the program has won
		if (playerWon(board)) {
			std::cout << "The game is over. You lost. \n" << "The board was: \n";
			printBoard(board);
			break;
		} else if (i == 7) { // program and user tied
			std::cout << "The game was a tie. \n" << "The board was: \n";
			printBoard(board);
			break;
		}
	}
}
