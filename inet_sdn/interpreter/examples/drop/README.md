Description
============
Example of use of the primitive 'drop'.

The primitive 'drop' drops a packet.

It is a logical primitive, i.e. a primitive that can be used in conditional or unconditional attacks.


Sintax
======
drop(packetName, threshold)

args:
 + packetName is the name of the packet to drop
 + threshold defines the probability that the drop action occurs


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
$ python ../../interpreter.py -i drop-original.adl -o drop-original.xml
```

or

``` sh
$ python ../../interpreter.py -i drop-original.adl
```

or you can use the pre-built bash script

``` sh
$ ./drop-one.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
