Description
============
Advanced examples of use of the packet filter ('filter' keyword).

It must be used (only) in conditional attacks.


Blocks
======
A packet filter is made by a set of blocks and a set of logical conditions.

The packet filter supports two types of 'blocks':
 + elementary blocks;
 + compound blocks.

The packet filter supports two types of 'logical conditions':
 + AND;
 + OR.
 
 
Elementary block
================
``` sh
"layerName.fieldName" comparisonOperator operandSet
```

For example
``` sh
"APP.sequenceNumber" == 1000
```

or
``` sh
"APP.sequenceNumber" == "1000|2000|3000"
```


Compound block
==============
``` sh
( "layerName.fieldName" arithmeticOperator operand ) comparisonOperator operandSet
```

For example
``` sh
( "APP.sequenceNumber" % 3 ) == 0
```


Sintax
======
``` sh
filter(block1 logicalOperator block2 logicalOperator ...)
```


Example 1
=========
``` sh
filter("APP.sourcePort" == "1000|2000")
```
It filters packets having the field "sourcePort" of the layer APP (5) equal to 1000 OR 2000.


Example 2
=========
``` sh
filter("APP.sourcePort" == 1000 or "APP.sourcePort" == 2000)
```
It filters packets having the field "sourcePort" of the layer APP (5) equal to 1000 OR 2000.
It is equivalent to the previous example.


Example 3
=========
``` sh
filter(("APP.sequenceNumber" % 3) == 0)
```
It filters packets having the field "sequenceNumber" as a perfect multiple of 3.


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
$ python ../../interpreter.py -i advanced-packet-filter-1.adl -o advanced-packet-filter-1.xml
```

or

``` sh
$ python ../../interpreter.py -i advanced-packet-filter-1.adl
```

or you can use the pre-built bash script

``` sh
$ ./advanced-packet-filter-1.sh
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>
