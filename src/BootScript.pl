#!/usr/bin/perl -w
use strict;use File::Copy;
# Création du fichier 'fichier.txt'

my $fichier = "/etc/network/interfaces";
my $fichierOrig = "/etc/network/interfaces.orig";

copy($fichier,$fichierOrig);

open (FICHIER, ">>$fichier") || die ("ERROR\n");
# On écrit dans le fichier...
print FICHIER "\niface $ARGV[0] inet static";

close (FICHIER);
`NetworkManager status 2>&1`;
if(`echo $?`!=0){
 `service network-manager restart`;
}
 `iwconfig $ARGV[0] mode Ad-Hoc`;
print `echo Setting $ARGV[0] AD HOC`;

