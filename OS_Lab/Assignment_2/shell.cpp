#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <wait.h>

using namespace std;

vector<string> split(const string &str, char delimiter)
{
  vector<string> tokens;
  string token;
  istringstream tokenStream(str);
  while (getline(tokenStream, token, delimiter))
  {
    tokens.push_back(token);
  }
  return tokens;
}

int main()
{
  while (true)
  {
    cout << "$ ";
    string line;
    getline(cin, line);
    vector<string> tokens = split(line, ' ');
    if (tokens.empty())
    {
      continue;
    }
    char *command = &tokens[0][0];
    char *argv[tokens.size() + 1];
    for (int i = 0; i < tokens.size(); i++)
    {
      argv[i] = &tokens[i][0];
    }
    argv[tokens.size()] = NULL;
    pid_t pid = fork();
    if (pid == 0)
    {
      execvp(command, argv);
      cout << "Error: command not found" << endl;
      return 1;
    }
    else
    {
      wait(NULL);
    }
  }
  return 0;
}