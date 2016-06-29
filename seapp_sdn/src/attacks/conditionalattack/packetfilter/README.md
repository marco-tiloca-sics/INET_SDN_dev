Description
============
This folder contains the classes that model the packet-filter and the 
packet-filter elements (aka blocks).


Class hierarchy
===============
PacketFilter

FilterBlock
 + ElementartBlock
 + ComposedBlock


Packet filter format
====================
A packet filter can be composed by two types of blocks:
 + elementary;
 + composed.

An elementary block is:
``` sh
[ : operand : comparisonOperator : operand : ]
```

A composed block is:
``` sh
[ : ( : operand : ArithmeticOperator : operand : ) : comparisonOperator : operand : ]
```


Authors
=======
+ Francesco Racciatti  	<racciatti.francesco@gmail.com>


Maintainer
==========
+ Francesco Racciatti	<racciatti.francesco@gmail.com>
