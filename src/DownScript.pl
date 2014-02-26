#!/usr/bin/perl -w
use strict;use warnings;use File::Copy;
# Création du fichier 'fichier.txt'
my $fichier = "/etc/network/interfaces";
my $fichierOrig = "/etc/network/interfaces.orig";
`rm $fichier`;
rename ($fichierOrig,$fichier);
`NetworkManager status 2>&1`;
if(`echo $?`!=0){
 `service network-manager restart`;
}
print `echo Setting $ARGV[0] DEFAULT`;
