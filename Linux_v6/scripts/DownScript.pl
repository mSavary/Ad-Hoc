#!/usr/bin/perl -w
use strict;use warnings;use File::Copy;
# Création du fichier 'fichier.txt'
my $fichier = "/etc/network/interfaces";
my $fichierOrig = "/etc/network/interfaces.orig";
`rm $fichier`;
rename ($fichierOrig,$fichier);


my $forward = "/proc/sys/net/ipv6/conf/all/forwarding";


# forwarding activation
open (FILE, ">$forward") || die ("ERROR\n");
print FILE "0";
close(FILE);

my $interface = `iwconfig 2>&1 | grep -e 'IEEE 802.11' | cut -f1 -d ' '`;
chomp($interface);

`NetworkManager status 2>&1`;
if(`echo $?`!=0){
 `service network-manager restart`;
}
print `echo Setting $interface DEFAULT`;
