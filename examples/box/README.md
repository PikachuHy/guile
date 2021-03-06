# box

## Overview

This directory includes an example program for extending Guile with a new (and even useful) data type.


## Build Instructions

To build the example, simply type

```shell
make box
```



in this directory.

The resulting `box` program is a Guile interpreter which has one additional data type called `box`.


## The Box Data Type

A box is simply an object for storing one other object in.  It can be used for passing parameters by reference, for example.  You simply store an object into a box, pass it to another procedure which can store a new object into it and thus return a value via the box.


### Usage

Box objects are created with `make-box`, set with `box-set!` and examined with `box-ref`.  See the following example session for usage details:


### Example Session
```shell script
guile> (define b (make-box))
guile> b
#<box #f>
guile> (box-set! b '(list of values))
guile> b
#<box (list of values)>
guile> (box-ref b)
(list of values)
guile> (quit)
```