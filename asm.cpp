/*
Author: Shreyas Kumar Jaiswal
Roll  : 2301CS52
Assigned by: Prof. Jimson Mathew
*/

#include <bits/stdc++.h>
using namespace std;

vector<pair<int, string>> allLines;
vector<pair<string, string>> operAndData;
vector<int> typeofInstruction;
// -1 : Labels   0 : 0 operand  1 : value as operand  2 : label as operand
vector<pair<int, string>> errors;
map<string, string> OpCode;
map<string, int> allLabels;
multimap<string, int> toLabelfound;
bool HALTpresent = false;
set<string> zeroOperandCommand;
map<string, int> SETvalues;
map<string, int> datavalues;
vector<pair<string, string>> machineCode;
string filename = "";

void initCommands()
{
  //  No OPCODE for data and SET
  OpCode["data"] = "";   // value as operand
  OpCode["ldc"] = "00";  // value as operand
  OpCode["adc"] = "01";  // value as operand
  OpCode["ldl"] = "02";  // label as operand
  OpCode["stl"] = "03";  // label as operand
  OpCode["ldnl"] = "04"; // label as operand
  OpCode["stnl"] = "05"; // label as operand
  OpCode["add"] = "06";
  OpCode["sub"] = "07";
  OpCode["shl"] = "08";
  OpCode["shr"] = "09";
  OpCode["adj"] = "0a"; // value as operand
  OpCode["a2sp"] = "0b";
  OpCode["sp2a"] = "0c";
  OpCode["call"] = "0d"; // label as operand
  OpCode["return"] = "0e";
  OpCode["brz"] = "0f";  // label as operand
  OpCode["brlz"] = "10"; // label as operand
  OpCode["br"] = "11";   // label as operand
  OpCode["HALT"] = "12";
  OpCode["SET"] = ""; // value as operand
  zeroOperandCommand.insert("add");
  zeroOperandCommand.insert("sub");
  zeroOperandCommand.insert("shl");
  zeroOperandCommand.insert("shr");
  zeroOperandCommand.insert("a2sp");
  zeroOperandCommand.insert("sp2a");
  zeroOperandCommand.insert("return");
  zeroOperandCommand.insert("HALT");
}

char digittohex(int n)
{
  if (n <= 9)
    return n + '0';
  else
    return n - 10 + 'a';
}

string DecToHex(long long n, int len)
{
  string temp = "";
  if (n < 0)
  {
    n += (1ll << (4 * len));
  }
  for (int i = 0; i < len; i++)
  {
    temp += digittohex(n % 16);
    n /= 16;
  }
  reverse(temp.begin(), temp.end());
  return temp;
}

void errorSort(vector<pair<int, string>> &df)
{
  int n = df.size();
  for (int i = 0; i < n - 1; i++)
  {
    for (int j = 0; j < n - 1 - i; j++)
    {
      if (df[j].first > df[j + 1].first)
      {
        swap(df[j], df[j + 1]);
      }
    }
  }
}

unsigned int convertMachineCodeToInt(string s)
{
  unsigned int temp = 0;
  for (int i = 0; i < 8; i++)
  {
    if (s[i] >= 'a')
      temp = temp * 16 + (s[i] - 'a' + 10);
    else
      temp = temp * 16 + (s[i] - '0');
  }
  return temp;
}

int HexToDec(string s)
{
  int temp = 0;
  for (int i = 2; i < (int)s.length(); i++)
    if ((s[i] | ' ') >= 'a')
      temp = temp * 16 + ((s[i] | ' ') - 'a' + 10);
    else
      temp = temp * 16 + (s[i] - '0');
  if (temp >= (1 << 23))
    temp -= (1 << 24);
  return temp;
}

int OctalToDec(string s)
{
  int temp = 0;
  for (int i = 2; i < (int)s.length(); i++)
    temp = temp * 8 + (s[i] - '0');
  if (temp >= (1 << 23))
    temp -= (1 << 24);
  return temp;
}

bool isDec(string s)
{
  if (*s.begin() == '-' || *s.begin() == '+')
    s.erase(s.begin());
  for (auto &x : s)
    if (x < '0' || x > '9')
      return false;
  return true;
}
bool isHEX(string s)
{
  if ((int)s.length() < 3)
    return false;
  if (s[0] != '0' || (s[1] != 'x' && s[1] != 'X'))
    return false;
  for (int i = 2; i < (int)s.length(); i++)
    if (!((s[i] >= '0' && s[i] <= '9') || ((s[i] | ' ') >= 'a' && (s[i] | ' ') <= 'f')))
      return false;
  return true;
}

bool isOctal(string s)
{
  if ((int)s.length() < 3)
    return false;
  if (s[0] != '0' || (s[1] != 'o' && s[1] != 'O'))
    return false;
  for (int i = 2; i < (int)s.length(); i++)
    if ((s[i] < '0' || s[i] > '7'))
      return false;
  return true;
}

bool isValidLabel(string s)
{
  if (!((toupper(s[0]) >= 'A' && toupper(s[0]) <= 'Z') || s[0] == '_'))
    return false;
  for (int i = 1; i < (int)s.length(); i++)
    if (!((toupper(s[i]) >= 'A' && toupper(s[i]) <= 'Z') || (s[i] == '_') || (s[i] >= '0' && s[i] <= '9')))
      return false;
  return true;
}

string filter(string s, int line)
{
  for (int _ = 0; _ < 2; _++)
  {
    while (s.back() == ' ' || s.back() == '\t' || s.back() == '\n')
      s.pop_back();
    reverse(s.begin(), s.end());
  }
  string temp = "";
  for (int i = 0; i < (int)s.size(); i++)
  {
    if (s[i] == ';' || s[i] == '\n')
    {
      while (temp.back() == ' ')
        temp.pop_back();
      break;
    }
    if (s[i] == ':')
    {
      temp += ':';
      temp += ' ';
      while (s[i + 1] == ' ')
        i++; // clear all consecutive spaces
      continue;
    }
    if (s[i] == ' ')
    {
      temp += ' ';
      while (s[i + 1] == ' ')
        i++; // clear all consecutive spaces
      continue;
    }
    if (s[i] == '\t')
    {
      continue;
    }
    temp += s[i];
  }
  while (temp.back() == ' ')
    temp.pop_back();
  int cntspaces = 0;
  for (auto ch : temp)
  {
    if (ch == ' ')
      cntspaces++;
  }
  if (temp.empty())
  {
  }
  else if (cntspaces == 0)
  {
  }
  else if (cntspaces > 2)
  {
    errors.push_back({line + 1, "Invalid Syntax."});
  }
  return temp;
}

int countSpaces(string s)
{
  int cnt = 0;
  for (auto &x : s)
  {
    if (x == ' ')
      cnt++;
  }
  return cnt;
}

void processLabelsandAssignPC()
{
  int pc = 0;
  for (int i = 0; i < (int)allLines.size(); i++)
  {
    bool SETpresent = false; // we are replacing all labels which are SET eith their values.
    string s = allLines[i].second;
    if (s == "")
      continue;
    int numSpaces = countSpaces(s);
    if (numSpaces == 0)
    {
      if (s.back() == ':')
      {
        allLines[i].first = pc;
        s.pop_back();
        if (!isValidLabel(s))
        {
          errors.push_back({i + 1, "Invalid Label Name: \"" + s + "\"."});
        }
        else if (allLabels.count(s))
        {
          errors.push_back({i + 1, "Multiple Declaration of the same label \"" + s + "\"."});
        }
        else
        {
          allLabels[s] = pc;
          typeofInstruction[i] = -1;
        }
      }
      else
      {
        allLines[i].first = pc++;
        if (s == "HALT")
          HALTpresent = true;
        if (OpCode.find(s) != OpCode.end())
        {
          if (zeroOperandCommand.find(s) == zeroOperandCommand.end())
          {
            // cout << "here " << s << ' ' << numSpaces << '\n';
            errors.push_back({i + 1, "Missing Operands"});
          }
          else
          {
            typeofInstruction[i] = 0;
          }
        }
        else
        {
          errors.push_back({i + 1, "Invalid Mnemonic."});
        }
        operAndData[i] = {s, ""};
      }
    }
    else if (numSpaces == 1)
    {
      allLines[i].first = pc++;
      for (int j = 0; j < (int)s.length(); j++)
      {
        if (s[j] == ' ')
        {
          string temp = s.substr(j + 1, s.length() - j - 1);
          string oper = s.substr(0, j);
          if (zeroOperandCommand.find(oper) != zeroOperandCommand.end())
          {
            errors.push_back({i + 1, "No operand should be there."});
            break;
          }
          else if (OpCode.find(oper) == OpCode.end())
          {
            errors.push_back({i + 1, "Invalid Operation."});
            break;
          }
          else if (isDec(temp) || isHEX(temp) || isOctal(temp))
          {
            typeofInstruction[i] = 1;
          }
          else
          {
            toLabelfound.insert({temp, i + 1});
            typeofInstruction[i] = 2;
          }
          operAndData[i] = {oper, temp};
          break;
        }
        else if (s[j] == ':')
        {
          string labelfound = s.substr(0, j);
          if (!isValidLabel(labelfound))
          {
            errors.push_back({i + 1, "Invalid Label Name: \"" + labelfound + "\"."});
          }
          else if (allLabels.count(labelfound))
          {
            errors.push_back({i + 1, "Multiple Declaration of the same label \"" + labelfound + "\"."});
          }
          else
          {
            allLabels[labelfound] = pc;
            string operand = s.substr(j + 2, s.length() - j - 2);
            if (zeroOperandCommand.find(operand) == zeroOperandCommand.end())
            {
              errors.push_back({i + 1, "Operands missing."});
            }
            else
            {
              if (operand == "HALT")
              {
                HALTpresent = 1;
              }
              operAndData[i] = {operand, ""};
              typeofInstruction[i] = 0;
            }
          }
          break;
        }
      }
    }
    else if (numSpaces == 2)
    {
      int semicolon = -1;
      for (int j = 0; j < (int)s.length(); j++)
      {
        if (s[j] == ':')
        {
          semicolon = j;
          break;
        }
      }
      if (semicolon == -1)
      {
        errors.push_back({i + 1, "Multiple Operands present."});
        // cout << s << '\n';
        continue;
      }
      string labelfound = s.substr(0, semicolon);
      if (!isValidLabel(labelfound))
      {
        errors.push_back({i + 1, "Invalid Label Name: \"" + labelfound + "\"."});
      }
      else if (allLabels.count(labelfound))
      {
        errors.push_back({i + 1, "Multiple Declaration of the same label \"" + labelfound + "\"."});
      }
      else
      {
        string temp1 = s.substr(semicolon + 2, 4);
        string temp2 = s.substr(semicolon + 2, 3);
        string data1 = s.substr(semicolon + 6);
        string data2 = s.substr(semicolon + 5);
        if (temp1 != "data" && temp2 != "SET")
        {
          allLabels[labelfound] = pc;
        }
        else if (temp1 == "data")
        {
          datavalues[labelfound] = pc;
        }
        else if (temp2 == "SET")
        {
          SETpresent = true;
          if (isHEX(data1))
          {
            SETvalues[labelfound] = HexToDec(data1);
          }
          else if (isOctal(data1))
          {
            SETvalues[labelfound] = OctalToDec(data1);
          }
          else if (isDec(data1))
          {
            SETvalues[labelfound] = stoi(data1);
          }
        }
      }
      string righthalf = s.substr(semicolon + 2, s.length() - semicolon - 2);

      for (int j = 0; j < (int)righthalf.length(); j++)
      {
        if (righthalf[j] == ' ')
        {
          string oper = righthalf.substr(0, j);
          string temp = righthalf.substr(j + 1, righthalf.length() - j - 1);
          if (zeroOperandCommand.find(oper) != zeroOperandCommand.end())
          {
            errors.push_back({i + 1, "Extra Operands present."});
            break;
          }
          if (OpCode.find(oper) == OpCode.end())
          {
            errors.push_back({i + 1, "Invalid Operation."});
            break;
          }
          if (isDec(temp) || isHEX(temp) || isOctal(temp))
          {
            typeofInstruction[i] = 1;
          }
          else
          {
            toLabelfound.insert({temp, i + 1});
            typeofInstruction[i] = 2;
          }
          operAndData[i] = {oper, temp};
          break;
        }
      }
      allLines[i].first = pc++;
    }
    if (SETpresent)
    {
      pc--;
      allLines.erase(allLines.begin() + i);
      i--;
    }
  }
  for (auto &x : allLabels)
  {
    toLabelfound.erase(x.first);
  }
  for (auto &x : datavalues)
  {
    toLabelfound.erase(x.first);
  }
  for (auto &x : SETvalues)
  {
    toLabelfound.erase(x.first);
  }
  for (auto &x : toLabelfound)
  {
    errors.push_back({x.second, "No such label: \"" + x.first + "\""});
  }
}

void checkRoundOne(string filename)
{
  ifstream f(filename + ".asm");
  if (!f.is_open())
  {
    cout << "No such file found.\n";
    exit(1);
  }
  string s;
  int n = 0;
  while (getline(f, s))
  {
    string temp = filter(s, n++);
    if (temp == "")
      continue;
    allLines.push_back({0, temp});
  }
  typeofInstruction.assign(allLines.size(), -1);
  operAndData.assign(allLines.size(), {"", ""});
  initCommands();
  processLabelsandAssignPC();
}

bool errorsWritten()
{
  ofstream errorfile(filename + ".log");
  errorfile << "Log code generated in: " + filename + ".log\n";
  cout << "Log code generated in: " + filename + ".log\n";
  if (!HALTpresent)
  {
    errorfile << "Warning! HALT is missing in the code.\n";
    cout << "Warning! HALT is missing in the code.\n";
  }
  if (errors.size())
  {
    errorSort(errors);
    for (auto &x : errors)
    {
      errorfile << "Error at line " << x.first << " -- \"" << allLines[x.first - 1].second << "\" -- " << x.second << '\n';
      cout << "Error at line " << x.first << " -- \"" << allLines[x.first - 1].second << "\" -- " << x.second << '\n';
    }
    errorfile.close();
    return true;
  }
  errorfile << "Machine code generated in: " + filename + ".o\nListing code generated in: " + filename + ".l\n";
  cout << "Machine code generated in: " + filename + ".o\nListing code generated in: " + filename + ".l\n";
  errorfile.close();
  return false;
}

void WritingRound()
{
  for (int i = 0; i < (int)allLines.size(); i++)
  {
    machineCode.push_back({DecToHex(allLines[i].first, 8), ""});
    if (typeofInstruction[i] == -1)
    {
      machineCode.back().second = "        ";
    }
    else if (typeofInstruction[i] == 0)
    {
      machineCode.back().second = "000000" + OpCode[operAndData[i].first];
    }
    else if (typeofInstruction[i] == 1)
    {
      string data = operAndData[i].second;
      if (isDec(data))
      {
        if (operAndData[i].first == "data")
        {
          machineCode.back().second = DecToHex(stoi(data), 8);
        }
        else
        {
          machineCode.back().second = DecToHex(stoi(data), 6) + OpCode[operAndData[i].first];
        }
      }
      else if (isHEX(data))
      {
        if (operAndData[i].first == "data")
        {
          machineCode.back().second = DecToHex(HexToDec(data), 8);
        }
        else
        {
          machineCode.back().second = DecToHex(HexToDec(data), 6) + OpCode[operAndData[i].first];
        }
      }
      else if (isOctal(data))
      {
        machineCode.back().second = DecToHex(OctalToDec(data), 6) + OpCode[operAndData[i].first];
      }
    }
    else if (typeofInstruction[i] == 2)
    {
      string label = operAndData[i].second;
      if (datavalues.find(label) != datavalues.end())
      {
        machineCode.back().second = DecToHex(datavalues[label], 6);
      }
      else if (SETvalues.find(label) != SETvalues.end())
      {
        machineCode.back().second = DecToHex(SETvalues[label], 6);
      }
      else if (operAndData[i].first == "ldc" || operAndData[i].first == "adc")
      {
        machineCode.back().second = DecToHex(allLabels[label], 6);
      }
      else
      {
        machineCode.back().second = DecToHex(allLabels[label] - allLines[i].first - 1, 6);
      }
      machineCode.back().second = machineCode.back().second + OpCode[operAndData[i].first];
    }
  }
  ofstream code(filename + ".l");
  for (int i = 0; i < (int)machineCode.size(); i++)
  {
    code << machineCode[i].first << ' ' << machineCode[i].second << ' ';
    code << allLines[i].second << '\n';
  }
  code.close();
  ofstream outMachineCode;
  outMachineCode.open((filename + ".o"), ios::binary | ios::out);
  for (auto to : machineCode)
  {
    if (to.second.empty() or to.second == "        ")
      continue;
    unsigned int x = convertMachineCodeToInt(to.second);
    outMachineCode.write((char *)&x, sizeof(int));
  }
  outMachineCode.close();
}

int main(int argc, char **argv)
{
  string s;
  if (argc >= 2)
  {
    s = argv[1];
  }
  else
  {
    cout << "Enter file name to assemble: ";
    cin >> s;
  }
  filename = s.substr(0, (int)s.length() - 4);
  checkRoundOne(filename);
  if (!errorsWritten())
  {
    WritingRound();
  }
}