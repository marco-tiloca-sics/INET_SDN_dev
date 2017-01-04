Description
============
Example of use of the primitive 'destroy'.

The primitive 'destroy' destroies a node.

It is a physical primitive.


Sintax
======
destroy(nodeId, occurrenceTime)

args:
 + nodeId is the Id of the node to destroy
 + occurenceTime is the start time of the action


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
$ python ../../interpreter.py -i destroy-one.adl -o destroy-one.xml
```

or

``` sh
$ python ../../interpreter.py -i destroy-one.adl
```

or you can use the pre-built bash script

``` sh
$ ./destroy-one.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>

