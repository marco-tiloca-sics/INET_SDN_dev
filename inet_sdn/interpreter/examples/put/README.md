Description
============
Example of use of the primitive 'put'.

The primitive 'put' puts a packet in the transmission or reception buffer of a set of nodes.

It is a logical primitive, i.e. a primitive that can be used in conditional or unconditional attacks.


Sintax
======
put(packetName, dstNodeList, direction, updateStats, delay)

args:
 + packetName is the name of the target packet
 + dstNodeList is the list of nodes in which put the target packet
 + direction is the direction of the target packet
 + updateStats decides if update or not the stats
 + delay is the forwarding delay


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
$ python ../../interpreter.py -i put-conditional.adl -o put-conditional.xml
```

or

``` sh
$ python ../../interpreter.py -i put-conditional.adl
```

or you can use the pre-built bash script

``` sh
$ ./put-conditional.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
