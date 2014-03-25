#!/usr/bin/perl -w
use strict;use File::Copy;

my $forward = "/proc/sys/net/ipv6/conf/all/forwarding";


# forwarding activation
open (FILE, ">$forward") || die ("ERROR\n");
print FILE "1";
close(FILE);

print `more /proc/sys/net/ipv6/conf/all/forwarding `;

