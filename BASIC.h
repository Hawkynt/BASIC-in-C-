#pragma once

#include <iostream>
#include <windows.h>
#include <conio.h>

// macro hacks
#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)
#define UNIQUE(name) CONCAT(name, __LINE__)
#define EXPAND(x) x

// Assignment
// Type mapping
#define INTEGER int
#define LONG    long
#define SINGLE  float
#define DOUBLE  double
#define STRING  std::string
#define BYTE    unsigned char
#define BOOLEAN bool

// Variable declarations
#define AS ,
#define __DIM_IMPL(var , type) type var;
#define DIM(...) EXPAND(__DIM_IMPL(__VA_ARGS__))
#define __DIM_ARRAY_IMPL(var , size , type) type var[size];
#define DIM_ARRAY(...) EXPAND(__DIM_ARRAY_IMPL(__VA_ARGS__))

#define SET(x) x;
#define LET(x) auto x;
#define CONST(x) const constexpr auto x;

// Logic & conditionals
#define IF(cond) if(cond)
#define THEN {
#define ELSE } else {
#define ELSEIF(cond) } else if(cond) {
#define ENDIF }

#define SELECT(var) switch(var) {
#define CASE(val) case val: {
#define CASE_ELSE default: {
#define END_CASE } break;
#define END_SELECT }

#define AND &&
#define OR ||
#define NOT !

// Structured Paradigm
#define DO_WHILE(cond) while(cond) {
#define DO_UNTIL(cond) while(!(cond)) {

#define WHILE(cond) while(cond) {
#define WEND }

#define DO do {
#define LOOP_WHILE(cond) } while(cond);
#define LOOP_UNTIL(cond) } while(!(cond));
#define LOOP }
#define LOOP_FOREVER } while(true);

// For-loops
#define FOR(var, start, end) { auto UNIQUE(__end) = (end); for (auto var = start; var <= UNIQUE(__end); ++var) {
#define FOR_STEP(var, start, end, step) { auto UNIQUE(__end) = (end); auto UNIQUE(__step) = (step); for (auto var = start; UNIQUE(__step) > 0 ? var <= UNIQUE(__end) : var >= UNIQUE(__end); var += UNIQUE(__step)) {
#define NEXT } }

// Breaks and flow
#define BREAK break
#define CONTINUE continue
#define GOTO(label) goto label
#define LABEL(name) name:

// Modular Paradigm
#define __FUNCTION_IMPL(signature , rettype) rettype signature
#define FUNCTION(...) EXPAND(__FUNCTION_IMPL(__VA_ARGS__)) {
#define END_FUNCTION }

#define __SUB_IMPL(signature) void signature
#define SUB(...) EXPAND(__SUB_IMPL(__VA_ARGS__)) {
#define END_SUB }

#define RETURN(val) return val
#define END return 0;

// Core Functions
inline int __LEN_IMPL(const std::string & s) {
  return static_cast<int>(s.length());
}

inline int __LEN_IMPL(const char* s) {
  return static_cast<int>(std::strlen(s));
}

template<typename T>
int LEN(const T & s) {
  return __LEN_IMPL(s); // Calls correct overload based on type
}
#define CINT(x) static_cast<int>(x)
#define CLNG(x) static_cast<long>(x)
#define CSNG(x) static_cast<float>(x)
#define CDBL(x) static_cast<double>(x)
#define CSTR(x) std::to_string(x)

// I/O
#define SLEEP(ms) Sleep(ms);
#define RND() (static_cast<float>(rand()) / static_cast<float>(RAND_MAX))
#define TIMER() (static_cast<float>(std::clock()) / CLOCKS_PER_SEC)
#define RANDOMIZE_TIMER() srand(static_cast<unsigned int>(time(nullptr)));

template<typename... Args>
void __PRINT_IMPL(Args&&... args) {
  (std::cout << ... << args) << std::endl;
}

#define PRINT(...) do { __PRINT_IMPL(__VA_ARGS__); } while(0);

inline void __CLS_IMPL() {
  const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD count;
  if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

  const DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(hOut, ' ', cells, { 0, 0 }, &count);
  FillConsoleOutputAttribute(hOut, csbi.wAttributes, cells, { 0, 0 }, &count);
  SetConsoleCursorPosition(hOut, { 0, 0 });
}

#define CLS() __CLS_IMPL();

#define LOCATE(r,c) { COORD pos = { static_cast<SHORT>(c-1), static_cast<SHORT>(r-1) }; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); }
#define COLOR(fg,bg) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bg)<<4) | (fg));

#define INPUT(var) std::cin >> var;
#define INKEY() (_kbhit() ? _getch() : 0)

#define RUN(cmd) std::system(cmd);

