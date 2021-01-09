Compile adr2 (Digirule 2U Cross-Assembler)
-------------------------------------------

You can compile the Digirule 2U Cross-Assembler using the terminal under Mac OS or Linux, or using the command prompt under Windows.

Note: You will need to make sure that git and gcc are installed on your system before running the following commands.

For Linux, use the following:
```
git clone https://github.com/bradsprojects/Digirule2U
cd Digirule2U/Software\ Tools/adr2\ \(Digirule\ 2U\ Assembler\)/Source
gcc -Wall -Os -s *.c -o adr2
```

For Mac OS, use the following:
```
git clone https://github.com/bradsprojects/Digirule2U
cd Digirule2U/Software\ Tools/adr2\ \(Digirule\ 2U\ Assembler\)/Source
gcc -Wall -Os *.c -o adr2
```

For Windows, use following:
```
git clone https://github.com/bradsprojects/Digirule2U
cd Digirule2U\Software Tools\adr2 (Digirule 2U Assembler)\Source
gcc -Wall -Os -s *.c -o adr2.exe
```
