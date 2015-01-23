#!/usr/bin/perl

use warnings;
use strict;
use feature 'say';

#my $LG=5000000000000
#my $PG=6000000000000

my $in;
my $out;

while (1){
	open $in, "<", "in.txt" or die $!;
	my $LG=<$in>;
	my $PG=<$in>;
	chomp $LG; chomp $PG;
	say "Wczytano zakresy: $LG - $PG";
	close $in;

	my $SG=$PG - $LG;
	$SG=int($SG / 2);

	my $DIR=int(rand(2));

	my $LGalt = $LG;
	my $PGalt = $PG;
	if ( $DIR == 0 ){
		say "W lewo!";
		$PG=$LG+$SG;
		$LGalt=$LG+$SG;
	} else {
		say "W prawo!";
		$LG=$LG+$SG;
		$PGalt=$LG+$SG;
	}

#nietestowane
	open $in, "+>", "in.txt" or die $!; 
	say "Zapisuje zakresy: $LG\n$PG";
	print $in "$LG\n$PG";
	close $in;

	say "Uruchamiam program";
	system('wine prime34.exe < in.txt > out.txt');

	open $out, "<", "out.txt" or die $!;
#wczytujemy plik
	my $read=<$out>;
	$read=<$out>;
	$read=<$out>;
	$read=<$out>;
	$read=<$out>;
	$read=<$out>;
	$read=<$out>;
	my $A3=<$out>;
	say "output A3: $A3";
	my $A4=<$out>;
	say "output A4: $A4";
	chomp $A3; chomp $A4;

#wyciagamy wyniki
	my ($A3wynik) = $A3 =~ /(\d+)/;
	my ($A4wynik) = $A4 =~ /(\d+)/;

	if ($A3wynik == $A4wynik) {
#nietestowane
		open $in, "+>", "in.txt" or die $!; 
		say "W zakresie nie ma szukanej liczby, sprawdzam: $LGalt - $PGalt";
		print $in "$LGalt\n$PGalt";
		close $in;
	} 
	if ($PG-$LG <= 1){
		say "\n";
		say "Podejrzany zakres: $LG - $PG";
		say "\n";
		last;
	}

	close $out;
}
