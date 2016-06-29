Description
============
This example contains two different conditional attacks.

A conditional attack is defined by the following statement:
``` sh
from *time* nodes in *list* do {
    filter(*filter content*)
    # set of logical primitives
} 
```
i.e. a conditional attack is a statement that contains a *packet-filter* and a 
set of logical primitives (clone, create, drop, put, change, retrieve, send).

The packet-filter filters all the packet passing throught the nodes contained in the list *list* from time *time*.
The logical primitives are executed only if an intercepted packet matches with the packet-filter.


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
$ python ../../interpreter.py -i conditional-attacks.adl -o conditional-attacks.xml
```

or

``` sh
$ python ../../interpreter.py -i conditional-attacks.adl
```

or you can use the pre-built bash script

``` sh
$ ./conditional-attacks.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>

