Description
============
Example of use of the primitive 'retrieve'.

The primitive 'retrieve' retrieves the content of a certain field (of a certain layer) in a packet.

It is a logical primitive, i.e. a primitive that can be used in conditional or unconditional attacks.


Sintax
======
retrieve(packetName, layer.field, variableName)

args:
 + packetName is the name of the target packet
 + layer.field specifies the layer in which search a certain field
 + variableName is the variable in which store the retrieved value


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
$ python ../../interpreter.py -i retrieve.adl -o retrieve.xml
```

or

``` sh
$ python ../../interpreter.py -i retrieve.as
```

or you can use the pre-built bash script

``` sh
$ ./retrieve.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>

