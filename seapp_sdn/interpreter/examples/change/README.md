Description
============
Example of use of the primitive 'change'.

The primitive 'change' changes the content of a certain field (of a certain layer) in a packet.

It is a logical primitive, i.e. a primitive that can be used in conditional or unconditional attacks.


Sintax
======
change(packetName, layer.field, value)

args:
 + packetName is the name of the target packet
 + layer.field specifies the layer in which search a certain field
 + value is the value to overwrite in the field


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
$ python ../../interpreter.py -i change.adl -o change.xml
```

or

``` sh
$ python ../../interpreter.py -i change.adl
```

or you can use the pre-built bash script

``` sh
$ ./change.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
