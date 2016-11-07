#INET_SDN_dev

This is an extension of attack simulator SEA++ which can be found: https://github.com/seapp/seapp_stable

The project is under development and constitutes an extension of attack simulator SEA++ in Software Defined Networks (SDN)

Installation Steps:
-------------------
Pre-requirements:
- Python v.2.7.6 or greater (the current implementation has been tested under v. 2.7.12)
- gcc  v.4.7 or greater as the implementation is based on C++11 (the current implementationsion has been tested under v.5.4.0)
	
In order to build the project the steps are the following:
1. get & install libxml library:
    * sudo apt-get install libxml++2.6-dev libxml++2.6-doc
2. get & install OMNeT ++ 4.6
3. get & build INET_SDN
	* download the source code, extract the files and build:
	* make makefiles
	* make

Link libraries- Check Makefile:
-------------------------------
Check the makefile to confirm that it fits your architecture.
	
After downloading INET_SDN, in the initial makefile, all the links to the libraries are under /usr/lib/x86_64-linux-gnu/* except the link to 'libxml' where it is under the path -I/usr/lib/libxml++-2.6/include. The current version has been tested under Ubuntu 16.04 LTS.

For Ubuntu 14.04 all the links to the libraries are under /usr/lib/x86_64-linux-gnu/* .
For Ubuntu 12.04 (36-bit architecture) all the links to the libraries are under -l/usr/lib/i386-linux-gnu/*.


