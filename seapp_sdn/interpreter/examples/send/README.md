Description
============
Example of use of the primitive 'send'.

The primitive 'send' sends a new packet (created or cloned) to the following layer in the communication stack.

It is a logical primitive, i.e. a primitive that can be used in conditional or unconditional attacks.


Sintax
======
send(packetName, delay)

args:
 + packetName is the name of the target packet
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
$ python ../../interpreter.py -i send.adl -o send.xml
```

or

``` sh
$ python ../../interpreter.py -i send.adl
```

or you can use the pre-built bash script

``` sh
$ ./send.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
