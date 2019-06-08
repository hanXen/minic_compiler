# Mini C Compiler

## Usage

### main.c
```bash
> make  
> ./mini_c [mini-c file(.mc)]  
```
### Ucodei.cpp
```bash
> g++ -g -o ucodei UcodeI.cpp  
> ./ucodei [ucode file(.uco)] [lst file(.lst)]  
```

## Supported Lexical Structure

1. Keywords  
: `break`, `case`, `char`, `const`, `continue`, `default`, `else`, `float`, `for`, `if`, `int`, `return`, `switch`, `void`, `while`

2. Literals
- `int` literal : e.g. 0 , 1 , ... 
- `char` literal : e.g. 'A', 'a', ...
- `float` literal(fixed-point real number) : e.g. 123.4 , 123.456 , ...

## Contents

#### 1. main.c
* minic Compiler의 main함수. code generate의 기능이 구현되어 있으며, 아래의 `Scanner.c`, `Parser.c`, `sdt.c`, `EmitCode.c`, `SymTab.c`를 include하고 있다.
* Code generate를 통해 ast 파일을 `ucode(.uco)`로 바꿔준다.
* Output: `ast file(.ast)`, `uco file(.uco)` 

#### 2. Scanner.c
* `Minic code(.mc)`를 input으로 하여 해당 문법에 맞는 token을 output으로 출력한다.

#### 3. Parser.c
* `Scanner.c` file에서의 output인 token을 읽어드려, ast 형태로 저장한다.

#### 4. sdt.c
* `Parser.c` file에서 저장된 ast를 출력한다.

#### 5. SymTab.c
* `Symbol Table`에 대한 정보와 그에 대한 함수를 갖고 있다. 

#### 6. EmitCode.c
* `Symbol Table`과 `main.c`에서 나온 정보를 이용하여, 'ucode'를 출력해준다. 

#### 7. Ucodei.cpp 
* `Ucode file(.uco)`을 읽어들여, 명령어에 따라 작업을 수행하고 결과 값을 출력한다.
* Output : `.lst` file (ucode와 결과 값, 실행 cycle 수를 출력) 
* 본 project에서는 g++을 이용하여 따로 컴파일 한다. (컴파일 방법은 위 참조)

#### 8. Makefile
* `main.c`파일을 컴파일 한다. 
* Output: `minic`

