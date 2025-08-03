# 🧮 BASIC.h

[![License](https://img.shields.io/badge/License-LGPL_3.0-blue)](https://licenses.nuget.org/LGPL-3.0-or-later)
![Language](https://img.shields.io/github/languages/top/Hawkynt/BASIC-in-C-?color=purple)

**“The header your C++ professor warned you about.”**

---

## 🎯 What is this?

`BASIC.h` is a C++ header that lets you write code like it’s 1985 — **but it compiles today**.

It gives you a **PowerBASIC-style façade over modern C++**, with macros like:

```vb
FUNCTION(main() AS INTEGER)
  DIM(x AS INTEGER)
  SET(x, 42)
  IF(x > 0) THEN
    PRINT("We're back in business!")
  ENDIF
  END
END_FUNCTION
```

Yes. That compiles.
No. I’m not sorry.

---

## 🧪 Why?

Because BASIC was:

* Readable 🧑‍🏫
* Fast to write 💨
* Dangerous 🔥
* Full of gotos and global state 😈
* Didn't have those ugly brackets and semicolons 🗑
* Was my first programming language 🎉

And so is this.

Except this time, it's **C++** wearing BASIC's skin.

---

## 🔨 What’s Inside?

### 🧱 Structured Syntax

| BASIC            | C++ with BASIC.h                |
| ---------------- | ------------------------------- |
| `DIM x AS INT`   | `DIM(x AS INTEGER)`             |
| `SET x = 1`      | `SET(x, 1)`                     |
| `IF x > 0 THEN`  | `IF(x > 0) THEN`                |
| `ELSE`           | `ELSE`                          |
| `ENDIF`          | `ENDIF`                         |
| `FOR i = 1 TO 5` | `FOR(i, 1, 5)`                  |
| `NEXT`           | `NEXT`                          |
| `DO LOOP`        | `DO ... LOOP` or `LOOP_FOREVER` |
| `FUNCTION`       | `FUNCTION(name(...) AS TYPE)`   |
| `RETURN`         | `RETURN(value)`                 |
| `END`            | `END` (yes, no semicolon)       |

### 🖥️ Console I/O

| Command        | Behavior                                 |
| -------------- | ---------------------------------------- |
| `PRINT(...)`   | Sends to `std::cout << ...`              |
| `INPUT(x)`     | Reads into `x` from `cin`                |
| `CLS()`        | Clears screen (WinAPI, no system())      |
| `LOCATE(y,x)`  | Moves cursor to line/column              |
| `COLOR(fg,bg)` | Sets color via `SetConsoleTextAttribute` |

### 🧠 Types

```cpp
INTEGER → int  
LONG    → long  
SINGLE  → float  
DOUBLE  → double  
STRING  → std::string  
BYTE    → unsigned char  
BOOLEAN → bool
```

Because `auto` is too fancy for 80s kids.

---

## 🧪 Math & Conversion

```cpp
CINT(x)   → (int)(x)  
CSNG(x)   → (float)(x)  
CDBL(x)   → (double)(x)  
CSTR(x)   → std::to_string(x)  
LEN(x)    → length of string (supports std::string & char*)  
RND()     → float between 0.0 and 1.0  
RANDOMIZE_TIMER() → seed with time  
TIMER()   → current time in seconds
```

---

## ☢️ What’s the Catch?

* 🧨 You’re committing crimes against both C++ and BASIC simultaneously
* 🧠 Nested `FOR` loops? We made that work with line-number macros
* 🕳️ You will enter a state of macro-induced madness
* 🧼 Your linter will cry
* 👁️‍🗨️ Your teammates will **never trust you again**

---

## ⚙️ How It Works (Madness Inside™)

This entire monstrosity operates on a volatile cocktail of macro abuse, template trickery, and type-correct chaos. We simulate structured BASIC syntax using C++ macros that expand into scoped blocks, control flow, and even variable declarations — complete with type inference and BASIC-style AS TYPE syntax. We use UNIQUE identifiers based on _ _ LINE _ _ to prevent loop collisions (so you can nest a FOR inside a FOR_STEP inside a FUNCTION and somehow sleep at night). Type conversions like CINT, CSTR, and LEN are powered by a symphony of static_casts and overload-resolved templates that pretend C++ is duck-typed — until the compiler reminds you that it’s not. Even the CLS() function rewrites the console buffer byte-by-byte via Windows API, just so we don’t call system("cls") like a peasant. It's a macro metaverse. A DSL fever dream. A header file so cursed and beautiful it probably qualifies as modern art.

---

## 💡 Why You Might Actually Use It

* Teaching programming in a friendly syntax (but secretly C++)
* When there's just no BASIC interpreter around your toolbox but a GCC at hand
* Porting old BASIC algorithms to C++ with minimal effort
* Writing **console games** like Snake, Nibbles, etc.
* Because you can. And shouldn't. But still can.

---

## 🐍 Example: Snake in BASIC.h

Just look inside this repository's **ConsoleSnake** solution to see how weird this can be.

Yes, I made this compile.
Yes, it moves a snake across the console.
No, it’s not okay. 
Yes, it’s beautiful.

---

## 🧪 Final Word

**BASIC.h** is a shrine to weirdness, a playground for nostalgia, and a tool for creating retro magic inside modern compilers.

You're not just writing C++.

You're writing BASIC.
In C++.
And getting away with it.

😉
