#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

void tokenize(const string& cmd, vector<string>& tokens) {
	stringstream ss{ cmd };
	string token{ "" };
	while (ss >> token)
		tokens.push_back(token);
}

bool execute_command(vector<string>& tokens) {

	static int count = 0;

	if (tokens[0] == "quit") {
		return false;
	}
	else if (tokens[0] == "isready") {
		cout << "readyok" << endl;
	}
	else if (tokens[0] == "ucinewgame") {
	}
	else if (tokens[0] == "stop") {
	}
	else if (tokens[0] == "ponderhint") {
	}
	else if (tokens[0] == "position") {
	}
	else if (tokens[0] == "go") {
		++count;
		if (count % 2 == 1)
			cout << "bestmove b1a3" << endl;
		else
			cout << "bestmove a3b1" << endl;
	}
	else if (tokens[0] == "register") {
	}
	else if (tokens[0] == "setoption") {
	}
	else if (tokens[0] == "debug") {
	}
	return true;
}

int main(int argc, char *argv[])
{
	cout << "Sultan 1.0, by Murat Yirci" << endl;

	// 1) wait until uci command is received
	string cmd{ "" };
	vector<string> tokens;
	do {
		getline(cin, cmd, '\n');	// note that getline is a blocking function
		tokenize(cmd, tokens);
	} while (none_of(tokens.begin(), tokens.end(), [](string& s) {return s == "uci";}));

	// 2) identify the engine
	cout << "id name Sultan 1.0" << endl;
	cout << "id author Murat Yirci" << std::endl;

	// 3) send the options of the engine
	cout << "option name Hash type spin default 16 min 1 max 1024" << endl;
	cout << "option name Ponder type check default false" << endl;
	cout << "option name MultiPV type spin default 1 min 1 max 20" << endl;
	cout << "option name Clear Hash type button" << endl;

	// 4) send uciok
	cout << "uciok" << endl;

	// 5) wait for the next commands
	do {
		getline(cin, cmd, '\n');
		tokens.clear();
		tokenize(cmd, tokens);
	} while(execute_command(tokens));

    cout << "Sultan 1.0 is exiting." << endl;
    return 0;
}
