#
# lgpl-stamp.pl -- add lgpl file to source code
#
#     Replaces "//LGPL-HEADER//" markers with a standard lgpl header.
#


# CONVERT ALL TABS INTO EQUIVALENT NUMBER OF SPACES
sub TabsToSpaces($) {
    my $str = shift;
    1 while $str =~ s/\t+/' ' x (length($&)*8 - length($`)%8)/e;
    return $str;
}

my $infilename = $ARGV[0];

print STDERR "--- Working on $infilename\n";
unless(open(IN, "<$infilename")) {
    print STDERR "$0: ERROR: $infilename: $!\n";
    exit(1);
}
while(<IN>) {
    if ( m%//INSERT-LGPL-HEADER-HERE//% ) {
        print <<"EOF";
//////////////////////
// $infilename
//////////////////////
//
// OpDesk (Version $ENV{VERSION})
// This file is part of the OpDesk node graph FLTK widget.
// Copyright (C) 2009,2011 by Greg Ercolano.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING.txt" which should have been included with this file.
// If this file is missing or damaged, see the FLTK library license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems to:
//
//     erco (at) seriss.com
//
EOF
    } else {
        print TabsToSpaces($_);
    }
}
close(IN);
exit(0);
