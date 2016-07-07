Description
============
Example of use of the primitive 'clone'.

The primitive 'clone' produces a perfect copy of a given packet.

It is a logical primitive, i.e. a primitive that can be used in conditional or unconditional attacks.


Sintax
======
clone(srcPacketName, dstPacketName)

args:
 + srcPacketName is the name of the source packet
 + dstPacketName is the name of the new packet


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
$ python ../../interpreter.py -i clone.adl -o clone.xml
```

or

``` sh
$ python ../../interpreter.py -i clone.adl
```

or you can use the pre-built bash script

``` sh
$ ./clone.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
