Description
============
Example of use of the primitive 'move'.

The primitive 'move' moves a node in the simulation field.

It is a physical primitive.


Sintax
======
move(nodeId, occurrenceTime, x, y, z)

args:
 + nodeId is the Id of the node to move
 + occurenceTime is the start time of the action
 + x is the 'x' coordinate of the simulation field
 + y is the 'y' coordinate of the simulation field
 + z is the 'z' coordinate of the simulation field


Usage of the interpreter
========================
``` sh
$ python path/interpreter.py -i inputfile -o outputfile
```

or you can use the bash script for convenience:

``` sh
$ ./bash_script.sh
```

Example
=======
``` sh
$ python ../../interpreter.py -i move-one.adl -o move-one.xml
```

or

``` sh
$ python ../../interpreter.py -i move-one.adl
```

or you can use the pre-built bash script

``` sh
$ ./move-one.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
