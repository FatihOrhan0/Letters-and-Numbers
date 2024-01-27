#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

bool feasible(const string word, vector<int> letters) {
    for (char c : word) {
      if (!isalpha(c)) break;
      letters[c - 'a']--;
      if (letters[c - 'a'] < 0) return false;
    }
    return true;
}

vector<string> words(const vector<int>& letters, const vector<string>& vocab) {
    vector<string> res;
    for (const auto& word : vocab) {
        if (feasible(word, letters)) {
            res.push_back(word);
            if (res.size() > 5) break;
        }
    }
    return res;
}

string toLowercase(const string& word) {
    string result = word;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

int main(int argc, char* argv[]) {
    vector<int> letters(26, 0);

    string filename = "words.txt"; 

    for (int i = 1; i < argc; i++) {
      letters[argv[i][0] - 'a']++;
    }

    vector<string> vocab;
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
      cerr << "Error: Unable to open the file." << endl;
      return 1;
    }
    string word;
    while (getline(inputFile, word)) {
      vocab.push_back(toLowercase(word));
    }
    inputFile.close();
    vector<string> res = words(letters, vocab);
    for (const auto& word : res) {
      cout << word << '\n';
    }
    return 0;
}
