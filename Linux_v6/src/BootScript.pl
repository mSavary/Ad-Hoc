#!/usr/bin/perl -w
use strict;use File::Copy;
# Création du fichier 'fichier.txt'

my $fichier = "/etc/network/interfaces";
my $fichierOrig = "/etc/network/interfaces.orig";

copy($fichier,$fichierOrig);

open (FICHIER, ">>$fichier") || die ("ERROR\n");
# setting interface !
print FICHIER "auto $ARGV[0]\niface $ARGV[0] inet6 static
	address $ARGV[1]
	netmask 64
	wireless-essid Whisky 
        wireless-channel 5
        wireless-mode Ad-Hoc 
        wireless-power on";
     

close (FICHIER);
`NetworkManager status 2>&1`;
if(`echo $?`!=0){
 `service network-manager restart`;
}
`ifconfig $ARGV[0] up`;
`shutdown -r now`;
print `echo Setting $ARGV[0] AD HOC`;

