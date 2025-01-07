/*
Author: Shreyas Kumar Jaiswal
Roll  : 2301CS52
Assigned by: Prof. Jimson Mathew
*/

#include <bits/stdc++.h>
using namespace std;

vector<int> Memory(1 << 24);
vector<string> MachCode;
int A = 0, B = 0, PC = 0, SP = 0, linesOfCodeExecuted = 0;

void Operations(int type, int offset_val)
{
  if (type == 0) // ldc
  {
    B = A;
    A = offset_val;
  }
  else if (type == 1) // adc
  {
    A += offset_val;
  }
  else if (type == 2) // ldl
  {
    B = A;
    A = Memory[SP + offset_val];
  }
  else if (type == 3) // stl
  {
    Memory[SP + offset_val] = A;
    A = B;
  }
  else if (type == 4) // ldnl
  {
    A = Memory[A + offset_val];
  }
  else if (type == 5) // stnl
  {
    Memory[A + offset_val] = B;
  }
  else if (type == 6) // add
  {
    A += B;
  }
  else if (type == 7) // sub
  {
    A = B - A;
  }
  else if (type == 8) // shl
  {
    A = B << A;
  }
  else if (type == 9) // shr
  {
    A = B >> A;
  }
  else if (type == 10) // adj
  {
    SP = SP + offset_val;
  }
  else if (type == 11) // a2sp
  {
    SP = A;
    A = B;
  }
  else if (type == 12) // sp2a
  {
    B = A;
    A = SP;
  }
  else if (type == 13) // call
  {
    B = A;
    A = PC;
    PC += offset_val;
  }
  else if (type == 14) // return
  {
    PC = A;
    A = B;
  }
  else if (type == 15) // brz
  {
    if (A == 0)
      PC = PC + offset_val;
  }
  else if (type == 16) // brlz
  {
    if (A < 0)
      PC = PC + offset_val;
  }
  else if (type == 17) // br
  {
    PC = PC + offset_val;
  }
}

string DecToHex(unsigned int n)
{
  string temp = "";
  for (int i = 0; i < 8; i++)
  {
    int tem = n % 16;
    n /= 16;
    if (tem <= 9)
      temp += tem + '0';
    else
      temp += tem - 10 + 'a';
  }
  reverse(temp.begin(), temp.end());
  return temp;
}
int HexToDec(string s)
{
  int temp = 0;
  for (int i = 0; i < (int)s.length(); i++)
    if (s[i] >= 'a')
      temp = temp * 16 + (s[i] - 'a' + 10);
    else
      temp = temp * 16 + (s[i] - '0');
  return temp;
}

void Initially()
{
  cout << "To implement various operations, enter their code as shown: \n";
  cout << "  -exit   -> Exit the emulator.\n";
  cout << "  -before -> Memory trace in a file before execution\n";
  cout << "  -after  -> Memory trace in a file after execution\n";
  cout << "  -dump   -> Memory Dump\n";
  cout << "  -t      -> Run code one line at time\n";
  cout << "  -run    -> Run the end code\n";
  cout << "  -reg    -> Show registers and SP values\n";
  cout << "  -isa    -> Show instruction set used\n";
}

void readMachineCode(string file)
{
  ifstream binaryfile(file + ".o", ios::in | ios::binary);
  if (!binaryfile.is_open())
  {
    cout << "No such file present.\n";
    cout << "Program terminated.\n";
    exit(1);
  }
  unsigned int lines;
  int pos = 0;
  while (binaryfile.read((char *)&lines, sizeof(int)))
  {
    Memory[pos++] = lines;
    MachCode.push_back(DecToHex(lines));
  }
}

void displayMemory()
{
  cout << '\n';
  cout << "+----------+-----------+-----------+-----------+-----------+\n";
  cout << "|  Address | Value(+0) | Value(+4) | Value(+8) | Value(+c) |\n";
  cout << "+----------+-----------+-----------+-----------+-----------+\n";
  int sz = MachCode.size();
  for (int i = 0; i < sz;)
  {
    cout << ("| " + DecToHex(i) + " |  ");
    cout << ((i < sz) ? DecToHex(Memory[i++]) : "        ") << " |  ";
    cout << ((i < sz) ? DecToHex(Memory[i++]) : "        ") << " |  ";
    cout << ((i < sz) ? DecToHex(Memory[i++]) : "        ") << " |  ";
    cout << ((i < sz) ? DecToHex(Memory[i++]) : "        ") << " |\n";
  }
  cout << "+----------+-----------+-----------+-----------+-----------+\n\n";
}

bool runSingleComm()
{
  linesOfCodeExecuted++;
  if (PC >= (int)MachCode.size())
  {
    cout << "!! Segmentation fault. Please fix the code.\n";
    exit(1);
  }
  int operato = HexToDec(MachCode[PC].substr(6, 2));
  if (operato == 18)
  {
    cout << "HALT found. Program has finished running.\n";
    return false;
  }
  int value = HexToDec(MachCode[PC].substr(0, 6));
  if (value >= (1 << 23))
  {
    value -= (1 << 24);
  }
  Operations(operato, value);
  PC++;
  return true;
}

void runAll()
{
  while (true)
  {
    if (linesOfCodeExecuted > 10'000'000)
    {
      cout << "Infinite Loop detected in Program. Please fix.\n";
      cout << "Please fix the code.\n";
      exit(1);
    }
    if (!runSingleComm())
      break;
  }
}

void showRegisters()
{
  cout << '\n';
  cout << "+----+----------+----------+\n";
  cout << "|Reg.|  Decimal |   HEX    |\n";
  cout << "+----+----------+----------+\n";
  printf("|  A | %8d | ", A);
  cout << DecToHex(A) << " |\n";
  printf("|  B | %8d | ", B);
  cout << DecToHex(B) << " |\n";
  printf("| SP | %8d | ", SP);
  cout << DecToHex(SP) << " |\n";
  printf("| PC | %8d | ", PC);
  cout << DecToHex(PC) << " |\n";
  cout << "+----+----------+----------+\n\n";
}

void showInstructionSet()
{
  cout << '\n';
  cout << "+--------+----------+---------+\n";
  cout << "| OpCode | Mnemonic | Operand |\n";
  cout << "+--------+----------+---------+\n";
  cout << "|   0    |  ldc     |  value  |\n";
  cout << "|   1    |  adc     |  value  |\n";
  cout << "|   2    |  ldl     |  value  |\n";
  cout << "|   3    |  stl     |  value  |\n";
  cout << "|   4    |  ldnl    |  value  |\n";
  cout << "|   5    |  stnl    |  value  |\n";
  cout << "|   6    |  add     |         |\n";
  cout << "|   7    |  sub     |         |\n";
  cout << "|   8    |  shl     |         |\n";
  cout << "|   9    |  shr     |         |\n";
  cout << "|   10   |  adj     |  value  |\n";
  cout << "|   11   |  a2sp    |         |\n";
  cout << "|   12   |  sp2a    |         |\n";
  cout << "|   13   |  call    |  offset |\n";
  cout << "|   14   |  return  |         |\n";
  cout << "|   15   |  brz     |  offset |\n";
  cout << "|   16   |  brlz    |  offset |\n";
  cout << "|   17   |  br      |  offset |\n";
  cout << "|   18   |  HALT    |         |\n";
  cout << "+--------+----------+---------+\n\n";
}

void trace(string file, string typee)
{
  ofstream gentrace(file + '_' + typee + ".trace");
  if (typee == "after")
  {
    gentrace << '\n';
    gentrace << "+----+----------+----------+\n";
    gentrace << "|Reg.|  Decimal |   HEX    |\n";
    gentrace << "+----+----------+----------+\n";
    while (runSingleComm())
    {
      gentrace << "|  A | " << setw(8) << A << " | ";
      gentrace << DecToHex(A) << " |\n";
      gentrace << "|  B | " << setw(8) << B << " | ";
      gentrace << DecToHex(B) << " |\n";
      gentrace << "| SP | " << setw(8) << SP << " | ";
      gentrace << DecToHex(SP) << " |\n";
      gentrace << "| PC | " << setw(8) << PC << " | ";
      gentrace << DecToHex(PC) << " |\n";
      gentrace << "+----+----------+----------+\n";
    }
  }
  gentrace << '\n';
  gentrace << "+----------+-----------+-----------+-----------+-----------+\n";
  gentrace << "|  Address | Value(+0) | Value(+4) | Value(+8) | Value(+c) |\n";
  gentrace << "+----------+-----------+-----------+-----------+-----------+\n";
  int sz = MachCode.size();
  for (int i = 0; i < sz;)
  {
    gentrace << ("| " + DecToHex(i) + " |  ");
    gentrace << ((i < sz) ? DecToHex(Memory[i++]) : "        ") << " |  ";
    gentrace << ((i < sz) ? DecToHex(Memory[i++]) : "        ") << " |  ";
    gentrace << ((i < sz) ? DecToHex(Memory[i++]) : "        ") << " |  ";
    gentrace << ((i < sz) ? DecToHex(Memory[i++]) : "        ") << " |\n";
  }
  gentrace << "+----------+-----------+-----------+-----------+-----------+\n\n";
  gentrace.close();
  if (typee == "before") {
    cout << "Before trace generated in " + file + "_before.trace file.\n";
  }
  else {
    cout << "After trace generated in " + file + "_after.trace file.\n";
  }
}

bool EMU(string file)
{
  cout << "Enter command to execute: ";
  string comm;
  cin >> comm;
  if (comm == "0" || comm == "-exit" || comm == "exit") {
    return false;
  }
  else if (comm == "-before" || comm == "before") {
    trace(file, "before");
  }
  else if (comm == "-after" || comm == "after") {
    cout << "Ran all the commands to produce after trace.\n";
    trace(file, "after");
  }
  else if (comm == "-dump" || comm == "dump") {
    displayMemory();
  }
  else if (comm == "-t" || comm == "t") {
    runSingleComm();
    showRegisters();
    cout << "Single Line of Code executed.\n";
  }
  else if (comm == "-run" || comm == "run") {
    runAll();
    cout << "Total " << linesOfCodeExecuted << " lines of code were executed.\n";
  }
  else if (comm == "-reg" || comm == "reg") {
    showRegisters();
  }
  else if (comm == "-isa" || comm == "isa") {
    showInstructionSet();
  }
  else {
    cout << "Invalid Instruction. Try again.\n";
  }
  return true;
}

int main(int argc, char **argv)
{
  string s;
  if (argc == 2) {
    s = argv[1];
  }
  else {
    cout << "Enter machine code filename: ";
    cin >> s;
    // s = "mybubblesort.o";
  }
  s = s.substr(0, s.length() - 2);
  readMachineCode(s);
  Initially();
  while (EMU(s)){}
  return 0;
}