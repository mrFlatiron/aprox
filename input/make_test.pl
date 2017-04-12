#!/usr/bin/perl
use strict;
use warnings;
use utf8;

sub func
{
  my $x = shift;
  return sin ($x);
}

sub deriv
{
  my $x = shift;
  return cos ($x);
}

my $count = (abs (int $ARGV[0]) or 15);

my $a = ($ARGV[1] or -5);
my $b = ($ARGV[2] or 10);

my $out = ($ARGV[1] or "t1");

my $hx = ($b - $a) / ($count - 1);
open (my $fh, ">", $out) or die $!;
print $fh $count . "\n";
for (0..($count - 1))
{
  printf $fh "%.6lf ", ($a + $hx * $_);
}
print $fh "\n";
for (0..($count -1))
{
  print $fh func ($a + $hx * $_) . " ";
}
print $fh "\n";
for (0..($count - 1))
{
  print $fh deriv ($a + $hx * $_) . " ";
}
