#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

bool isAnagram(const string & s1, const string & s2) { 
  vector<int> chars(26, 0); 
  for (char c : s1) chars[c - 'a']++;
  for (char c : s2) chars[c - 'a']--;
  for (int i : chars) { 
    if (i) return false;
  }
  return true;
}

int main(int argc, char * argv[]) { 
  string filename = "words.txt"; 
  string target = argv[1];
  ifstream inputFile(filename);
  if (!inputFile.is_open()) {
    cerr << "Error: Unable to open the file." << endl;
    return 1;
  }
  string word;
  while (getline(inputFile, word)) {
    if (isAnagram(word, target)) { 
      cout << word << endl; 
      break;
    }
  }
  inputFile.close();
  return 0;
}