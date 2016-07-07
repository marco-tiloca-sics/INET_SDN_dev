Description
============
Example of use of the primitive 'create'.

The primitive 'create' creates a packet. The new packet must be filled by using the action 'change'.

It is a logical primitive, i.e. a primitive that can be used in conditional or unconditional attacks.


Sintax
======
create(newPacketName, layer.type, value)

args:
 + newPacketName is the name of the new packet
 + layer.type identifies the layer and the protocol
 + value identifies the particular packet of the protocol


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
$ python ../../interpreter.py -i create-conditional.adl -o create-conditional.xml
```

or

``` sh
$ python ../../interpreter.py -i create-conditional.adl
```

or you can use the pre-built bash script

``` sh
$ ./create-conditional.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>

