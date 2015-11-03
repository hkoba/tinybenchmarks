#!/usr/bin/env perl
use strict;
use warnings;
use File::Basename;

use fields qw/user_sec sys_sec benchmark_id N theme/;
sub LTSV () {__PACKAGE__}

{
  local $/ = "";
  my $all;
  while (<>) {
    chomp;
    my $filename = rootname(basename($ARGV));
    my LTSV $sum = +{};
    my LTSV $sqr = +{};
    my $nlines = 0;
    foreach my $line (split "\n") {
      my LTSV $ltsv = +{map {s/^([^:]+)://; ($1, $_)} split "\t", $line};
      $sum->{theme} //= $ltsv->{theme};
      $sum->{N} //= $ltsv->{N};
      $sum->{benchmark_id} //= $ltsv->{benchmark_id};
      $sum->{user_sec} += $ltsv->{user_sec};
      $sum->{sys_sec}  += $ltsv->{sys_sec};
      $sqr->{user_sec} += $ltsv->{user_sec} * $ltsv->{user_sec};
      $sqr->{sys_sec}  += $ltsv->{sys_sec} * $ltsv->{sys_sec};
    } continue {
      $nlines++;
    }

    next unless $nlines and $sum->{theme};

    $all->{"$filename $sum->{theme}\t($sum->{benchmark_id})"} = [$nlines, $sum, $sqr];

  }

  foreach my $name (sort keys %$all) {
    (my $n, my LTSV $sum, my LTSV $sqr) = @{$all->{$name}};
    print "theme\t$name\n";
    print tsv(user =>
	      trunc(my @U = avg_n_var($n, $sum->{user_sec}, $sqr->{user_sec})));
    print tsv(sys =>
	      trunc(my @S = avg_n_var($n, $sum->{sys_sec}, $sqr->{sys_sec})));
    print tsv("OPs/sec" => commas(int($sum->{N} / ($U[0] + $S[0]))));
    print "\n";
  }
}

sub rootname {
  my ($fn) = @_;
  $fn =~ s/\.[^\.]+$//;
  $fn;
}

sub tsv {
  join("\t", @_)."\n";
}

sub avg_n_var {
  my ($n, $sum, $sqr) = @_;
  my $avg = $sum/$n;
  ($avg, sqrt($sqr/$n - $avg*$avg));
}

sub commas {
  my ($n) = @_;
  1 while $n =~ s/^([-+]?\d+)(\d{3})/$1,$2/;
  $n;
}

sub trunc {
  map {
    sprintf "%.4f", $_;
  } @_;
}
