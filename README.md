# BSc Thesis: Design of a script programming language and implementation of an interpreter in C++  

# Project Overview:
The purpose of the thesis was the development of a custom script programming language and its interpreter in C++ from scratch. This project involved creating a lexical analyzer, syntax analyzer, and semantic analyzer to ensure accurate code processing.

# Key Features:
The language is very similar to JavaScript, so it boasts an array of features designed to accommodate diverse programming needs. These include:

**Data Types**: true, false, integer numbers, rational numbers, strings.
**Variables**: Storing variables and manipulate data.
```javascript
 var a;
 var num = 5;
 var _logical = true;
 var str = "This is a string.";
 var num2 = 10.543;
```
**Input/Output Commands**: Implementation of print and read commands for user interaction.
```javascript
print: "Hello, World!"; // Expect "Hello , World !"
printnl: 10; // Expect 10 and change new line
printnl: (4+10) * 10; // Expect 140
print: false; //Expect 0

read: x;
read: y,z;
```
**Operators**: Arithmetics, Logicals, Assignments ect.
```javascript
 var a = 10 + 5;
 a = 10 - 5;
 a = 10 * 5;
 a = 10 / 5;
 a = 10 % 3;

 a += 10;
 a -= 10;
 a *= 10;
 a /= 10;
 a %= 10;

 !a & !10
 -a & -5
 ++a & a++
 --a & a--

 a = a && 10;
 a = b || c;

 a = a == 0;
 a = b != 0;
 a = 10 <= 3;
 a = 10 >= 3;
 a = 10 > 3;
 a = 10 < 3;

 (a==10)?"True":"False"

```
**Control Flow**: For, While, Do-While, If, If-else, break, continue.

**Functions**: Functions, parameters, return.
```javascript
fun add(a,b){
  return a+b;
}
fun printer(a){
  print: a;
}

printer(add(10,5));

```
**Object-Oriented Concepts**: Classes and Inheritance.
```javascript
class Cat {
  init(voice, sleep, name) {
    this.voice = voice;
    this.sleep = sleep;
    this.name = name;
  }

  talk() {
    console.log(this.voice);
  }

  isSleeping() {
    console.log(this.sleep);
  }

  getName() {
    console.log(this.name);
  }
}

var cat = new Cat("meow", false, "Kitty");
cat.talk(); // Expect "meow"
cat.isSleeping(); // Expect false
cat.getName(); // Expect "Kitty"
```

```javascript
class classA {
  printA() { print: "A";}
}

class classB < classA {
  printB() { print: "B";}
}

var b = classB();
b.printB();
b.printA();
```
