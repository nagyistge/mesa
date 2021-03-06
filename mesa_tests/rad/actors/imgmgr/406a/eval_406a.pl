#!/usr/local/bin/perl -w

use Env;
use lib "scripts";
require imgmgr;

$SIG{INT} = \&goodbye;

sub goodbye () {
    print "Exiting...\n";

    exit 1;
}

sub fail {
    report( "\nTest failed\n\n");
    report( "Exiting...\n");

    exit 1;
}

sub report {
    my $msg = shift(@_);
    print "$msg\n";
    print REPORT "$msg\n";
}

sub match_responses {
    my ($mesa_resp, $test_resp) = @_;

    @ra1 = split /\\/, $mesa_resp;
    @ra2 = split /\\/, $test_resp;
    $n1 = @ra1;
    $n2 = @ra2;
    if( $n1 ne $n2) {
        print "\nResponses differ in number of modalities\n";
        return 0;
    }
    foreach $s1 (@ra1) {
        $match = 0;
        foreach $s2 (@ra2) {
            if( $s1 eq $s2) {
                $match = 1;
                last;
            }
        }
        if( ! $match) {
            print "\nTest response @ra2 fails to include modality $s1 from MESA response\n";
            return 0;
        }
    }
    return 1;
}

sub compare_responses {
  my $rdir = shift(@_);

  my $rpt_path = "$rdir/result.txt";

  report("Evaluation of $rdir\n");

  opendir IM_DIR, "$rdir/imgmgr" or die "Unable to open directory: $rdir/imgmgr\n";
  @imfiles = readdir IM_DIR;
  close IM_DIR;

  opendir MS_DIR, "$rdir/mesa" or die "Unable to open directory: $rdir/mesa\n";
  @msfiles = readdir MS_DIR;
  close MS_DIR;

  foreach $msfile (@msfiles) {
      if( $msfile =~ /.dcm/) {
          $name = "Study Instance UID";
          $mspath = "$rdir/mesa/$msfile";
  	$studyUID = `$MESA_TARGET/bin/dcm_print_element 0020 000D $mspath`;
          chomp($studyUID);
          # print "studyUID: $studyUID\n";
          if( ! $studyUID) {
              $msg = "Response from MESA ImgMgr ($mspath) does not contain $name";
              report("$msg\n");
              fail();
          }
  
          foreach $imfile (@imfiles) {
              $imStudyUID = "";
              if( $imfile =~ /.dcm/) {
                  $impath = "$rdir/imgmgr/$imfile";
  	        $val = `$MESA_TARGET/bin/dcm_print_element 0020 000D $impath`;
                  chomp($val);
                  if( $val eq $studyUID) { 
                       $imStudyUID = $val;
                       last
                  }
              }
          }
          if( ! $imStudyUID) {
              $msg = "No response from Test ImgMgr contains $name $studyUID";
              report( "$msg");
              fail();
          }
  
          # we should have a response with a valid Study UID at this point.
          $msg = "\nResults for Study Instance UID: $studyUID";
          report( "$msg\n");
  
          $name = "Patient Name";
          $mesa_val = `$MESA_TARGET/bin/dcm_print_element 0010 0010 $mspath`;
          chomp($mesa_val);
          if( ! $mesa_val) {
              $msg = "Response from MESA ImgMgr ($mspath) does not contain $name attribute";
              report( "$msg\n");
              fail();
          }
  
          $test_val = `$MESA_TARGET/bin/dcm_print_element 0010 0010 $impath`;
          chomp($test_val);
          if( ! $test_val) {
              $msg = "Response from Test ImgMgr ($impath) does not contain $name attribute";
              report( "$msg\n");
              fail();
          }
  
          # we have a valid response, see if they match....
          if( ! match_responses( "$mesa_val", $test_val)) {
              $msg =  "Response from MESA ImgMgr ($mesa_val) fails to match\n";
              $msg .= "response from Test ImgMgr ($test_val).\n";
              report( "$msg\n");
              fail();
          }
          $msg =  "$name match\n";
          $msg .= "MESA IMGMGR: $mesa_val    TEST IMGMGR: $test_val\n";
          report("$msg\n");
      }
  }
  report("Test Passed.\n");
}

# begin main....

$outfile = "406a/grade_406a.txt";
open REPORT, ">$outfile" or die "Can't open file: $outfile\n";
my $mesaVersion = mesa::getMESAVersion();
print REPORT "CTX: $mesaVersion \n";


compare_responses("406a/results");
report( "Query for studies by patient name complete.\n");

report( "\nTest 406a passed.\n");


