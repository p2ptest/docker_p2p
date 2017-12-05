#!/usr/bin/perl
package MyDefine;
use strict;
use warnings;
require Exporter;

our @ISA = ("Exporter");
our @EXPORT = qw(ERR_OK ERR_FAIL RET_SUC RET_FAIL RET_ERROR KILOBITE MEGABYTE GIGABYTE TERABYTE);
our @EXPORT_OK = qw(PI E);
our %EXPORT_TAGS = (math => [qw(PI E)]);

use constant ERR_OK => 0;
use constant ERR_FAIL => -1;

use constant RET_SUC => 1;
use constant RET_FAIL => 0;
use constant RET_ERROR => -1;

use constant KILOBITE => 1024;
use constant MEGABYTE => 1048576;
use constant GIGABYTE => 1073741824;
use constant TERABYTE => 1099511627776;

use constant PI => 3.1415926535898;
use constant E =>  2.718281828459;

#============================================
##Ê¹ÓÃ¾ÙÀý
=cut
ÀýI£º
use MyDefine;
print ERR_OK."\n";
print KILOBITE."\n";
ÀýII£º
use MyDefine qw(PI)
print PI."\n";
ÀýIII£º
use MyDefine qw(:math);
print PI."\n";
print E."\n";
=cut
1;