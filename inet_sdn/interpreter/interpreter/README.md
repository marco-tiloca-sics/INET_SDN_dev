Description
============
This tool is an interpreter for SEA++. The user can write attacks by using the Attack Specification Language (aka ADL).
The interpreter parses the ADL file (containing the attacks) and produces the Attack Configuration
File (aka ACF), i.e. an XML representation of the ADL file that is understandable by SEA++.


Source tree
===========
interpreter
 - interpreter/
   - attacks/
   - engine/
   - grammar/
   - lexer/
   - primitives/

Content of sub-dirs: 
 + attacks contains the classes that model attacks;
 + engine contains the parsing engine of the interpreter and other useful scripts;
 + grammar contains the grammar rules of the interpreter;
 + lexer contains the lexer of the interpreter;
 + primitives contains the classes which model primitives provided by the ADL.


Requirements
============
argparse library.

It is also necessary to have PLY (Python Lex-Yacc) created by David Beazley. You can download it from 
his personal [page] (http://www.dabeaz.com/ply/) or from his [Github page](https://github.com/dabeaz/ply).


Usage
=====
``` sh
$ python path/interpreter.py -i inputfile -o outputfile
```
-i inputfile is mandatory

-o outputfile is optional

The commands above produces the XML Attack Configuration File (aka ACF) for SEA++, according to the ADL description in the input file.
Only the input file is mandatory. If the user does not specify the output filename, the interpreter will use the default filename 'file_input.xml'.


Authors
=======
+ Alessandro Pischedda	<alessandro.pischedda@gmail.com>
+ Marco Tiloca			<marco.tiloca84@gmail.com>
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
