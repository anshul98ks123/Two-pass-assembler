#include <bits/stdc++.h>

#define f first
#define s second
using namespace std;

int StoI(string s) {
  stringstream convert(s);
  int code;
  convert >> code;
  return code;
}

string decToBin(int n) {
  string Bin;

  while (n) {
    Bin += (n & 1 ? "1" : "0");
    n /= 2;
  }
  reverse(Bin.begin(), Bin.end());

  return Bin;
}

string regCode(char a) {
  switch (a) {
    case 'A':
      return "001";
    case 'B':
      return "010";
    case 'C':
      return "011";
    case 'D':
      return "100";
    case 'H':
      return "101";
    case 'L':
      return "110";
  }
  return "XXX";
}

string padStr(string a) { return *(new string(32 - a.size(), '0')) + a; }

int main() {
  ifstream instrFile("input/codes.txt"), inputFile("input/assemblycode.txt");
  ofstream outputFile("output/output.txt"),
      symbolFile("output/symbol_table.txt");

  // storing codes info in a hashmap
  string a, b, c;
  int start = 200;
  unordered_map<string, pair<string, string>> codes;

  while (!instrFile.eof()) {
    instrFile >> a >> b >> c;
    codes[a] = {b, c};
  }

  // FIRST SCAN
  unordered_map<string, int> symbols;
  while (getline(inputFile, a)) {
    if (a[a.size() - 1] == ':') {
      symbols[a.substr(0, a.size() - 1)] = start;
      start--;
    }
    start++;
  }

  // SAVE SYMBOLS IN A FILE
  for (auto it : symbols) {
    symbolFile << it.f << " " << it.s << "\n";
  }

  // REPOSITION TO START OF INPUT FILE
  inputFile.clear();
  inputFile.seekg(0, ios::beg);

  // SECOND SCAN
  string opcode, r1, r2, type;
  while (!inputFile.eof()) {
    r1 = r2 = "";
    inputFile >> opcode;
    outputFile << opcode << "\n";

    if (!codes.count(opcode)) continue;

    type = codes[opcode].s;

    if (type == "rri" || type == "ri") {
      inputFile >> r1;
      r2 = decToBin(StoI(r1.substr(2)));
      r1 = regCode(r1[0]);
      opcode = codes[opcode].f + r1 + r2;

      outputFile << padStr(opcode) << "\n" << padStr(r2) << "\n";

    } else if (type == "rrr" || type == "rr") {
      inputFile >> r1;
      r2 = regCode(r1[2]);
      r1 = regCode(r1[0]);
      opcode = codes[opcode].f + r1 + r2;

      outputFile << padStr(opcode) << "\n";

    } else if (type == "a") {
      inputFile >> r1;
      r1 = decToBin(symbols[r1]);
      opcode = codes[opcode].f;

      outputFile << padStr(opcode) << "\n" << padStr(r1) << "\n";
    }
  }

  return 0;
}