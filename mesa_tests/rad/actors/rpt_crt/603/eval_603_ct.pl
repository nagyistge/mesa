#!/usr/local/bin/perl -w

use Env;

use lib "scripts";
require rpt_crt;

sub goodbye() {
  exit 1;
}

# Search for all SR objects that match on accession number

sub find_candidate_files {
  my $tag = shift(@_);
  my $val = shift(@_);
  print LOG "Report Creator 603\n";
  print LOG " Tag = $tag, val = $val \n";

 @srFiles = rpt_crt::find_matching_composite_objs ($verbose, $tag, $val);

 $s = scalar(@srFiles);
 print LOG "Matching SR files: $s\n";
 if ($s == 0) {
  print "No files found in MESA Report Manager that match $tag $val \n";
  print LOG "No files found in MESA Report Manager that match $tag $val \n";
 }

 print LOG "\n";
}

sub evaluate_candidate_files {
  $rtnValueEval = 1;
  $rtnValueCompare = 1;

  foreach $f (@srFiles) {
    print LOG "\nEvaluating $f\n";
    my $x = "$MESA_TARGET/bin/mesa_sr_eval ";
    $x .= " -v " if $verbose;
    $x .= " -p ../../msgs/sr/603/sr_603ct.dcm -t 2000:DCMR $f";

    print LOG "$x \n";
    print LOG `$x`;
    if ($? == 0) {
      $rtnValueEval = 0;
    } else {
      print LOG "SR object $f does not pass SR evaluation.\n";
    }

    $x = "$MESA_TARGET/bin/compare_dcm -m 603/sr_603_ct.ini";
    $x .= " -o " if $verbose;
    $x .= " ../../msgs/sr/603/sr_603ct.dcm $f";
    print LOG "$x \n";
    print LOG `$x`;
    if ($? == 0) {
      $rtnValueCompare = 0;
    } else {
      print LOG "SR object $f does not pass SR compare test.\n";
    }

    print LOG "\n";
  }
  $diff += $rtnValueEval + $rtnValueCompare;
}


$verbose = grep /^-v/, @ARGV;

open LOG, ">603/grade_603_ct.txt" or die "?!";
$diff = 0;

find_candidate_files("0008 0050", "2001B20");
evaluate_candidate_files;

print LOG "\nTotal Differences: $diff \n";
print "\nTotal Differences: $diff \n";

print "Logs stored in 603/grade_603_ct.txt \n";

exit $diff;
