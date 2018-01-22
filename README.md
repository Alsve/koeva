# koeva

### Preface
Heavily refactored code and documentation of koeva_old, better be separated for easy access. See https://www.github.com/Alsve/koeva_old for source. If you have a plan to make another bean classification device and wish to adopt koeva program, it is better getting this one, as the old one is not accompanied with any documentation and never will.
Refactoring is still in progress.

### Introduction
Koeva is a utility and/or template which is intended to run on Raspberry-Pi as a fully functional grade classification of Green Coffee Bean (GBC). This program make use of opencv as an assist functor to analize the green bean coffee. 

#### Part of Library
It comprise of several part of program. Those are following:

* **src/kvmain .c** : this is a main entry of koeva program
* **src/kvsession .h,.c** : session utility, currently made for Green Coffee Bean session.
* **include/kvmenu .h** : set of loop menu utility and creation of menu.
* **include/koeva-util-debug.h** : multipurpose set of logger macro

Each of the file is accompanied with detailed documentation between the source code.

### To-Do Lists
1. Refactoring of libkserial from koeva_old to linux kernel c programming standard style.
2. Adding full implementation of from koeva_old to kvmain.
3. Adding image processing algorithm and library to repository and add it to kvmain.
