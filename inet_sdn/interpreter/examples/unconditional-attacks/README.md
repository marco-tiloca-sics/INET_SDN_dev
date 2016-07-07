Description
============
This example contains two different unconditional attacks.

An unconditional attack is defined by the following statement:
``` sh
from *time* every *period* do {
    # set of logical primitives
} 
```
i.e. an unconditional attack is a loop statement that contains a set of logical primitives (clone, create, drop, put, change, retrieve, send).

The primitives contained inside the loop are periodically executed. The period of the loop is defined by *period*. 
The occurrence time of the attack is defined by *time*.


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
$ python ../../interpreter.py -i unconditional-attacks.adl -o unconditional-attacks.xml
```

or

``` sh
$ python ../../interpreter.py -i unconditional-attacks.adl
```

or you can use the pre-built bash script

``` sh
$ ./unconditional-attacks.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
