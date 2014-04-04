#!/usr/bin/perl -w
use strict;use File::Copy;
# Création du fichier 'fichier.txt'


my $interface = `iwconfig 2>&1 | grep -e 'IEEE 802.11' | cut -f1 -d ' '`;
chomp($interface);

my $commande = "ifconfig " . $interface . " | grep inet6 | awk '\$0 ~ /inet6:/ { print \$3 }'";

my $ip;
$ip = qx {$commande};

my $ipAdHoc = substr($ip, 4, length($ip)-8);

$ipAdHoc = "2014".$ipAdHoc;

my $fichier = "/etc/network/interfaces";
my $fichierOrig = "/etc/network/interfaces.orig";

copy($fichier,$fichierOrig);

open (FICHIER, ">>$fichier") || die ("ERROR\n");
# setting interface !
print FICHIER "auto $interface\niface $interface inet6 static
	address $ipAdHoc
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
qx{ifconfig $interface up};
`shutdown -r now`;
print `echo Setting $interface AD HOC`;

