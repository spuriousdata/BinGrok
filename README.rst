#######
BinGrok
#######

Hex editor created speficially with the goal of reverse-engineering
proprietary data-formats in mind. BinGrok allows for creation of 
c-struct-like 'views' of your data to better allow for visualization
of the parts that you have deciphered.

============
Installation
============

Installation requires Qt5 (Core, Widgets, and Gui)::

    qmake BinGrok.pro
    make && make install

=============
Struct Syntax
=============

There are four primary data types: ``int``, ``uint``, ``string``, and ``struct``. There is also ``array``, but it's just an alias for ``string``.

All types must be sized. You specify the size of an integer with parenthesis. Strings and arrays are sized with brackets. All sizes are in bytes. The size of a ``struct`` is determined dynamically by it's members. 

All types need to be followed by an identifier that will be used as it's name in the data visualizer.

Here is an example of a BinGrok struct defining how to visualize the header of a tar file::

    struct { 
      string[100] name; 
      int(8) mode; 
      int(8) uid;
      int(8) gid; 
      array[12] size; 
      array[12] mtime; 
      int(8) chksum; 
      int(1) typeflag; 
      string[100] linkname; 
      int(6) magic; 
      int(2) version; 
      string[32] uname; 
      string[32] gname; 
      int(8) devmajor; 
      int(8) devminor; 
      string[155] prefix; 
    } tar_header;
