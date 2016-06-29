Description
============
Example of physical attacks.

A physical attack is an attack composed by a single physical primitive  (i.e. destroy, disable, move, fakeread).
In this way, N different physical primitive correspond to N different physical attacks.


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
$ python ../../interpreter.py -i physical-attacks.adl -o physical-attacks.xml
```

or

``` sh
$ python ../../interpreter.py -i physical-attacks.adl
```

or you can use the pre-built bash script

``` sh
$ ./physical-attacks.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
