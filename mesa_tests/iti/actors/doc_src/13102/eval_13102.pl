#!/usr/local/bin/perl -w

use Env;
use lib "../common/scripts";
require mesa;

sub promptForFile()
{
  print "Please enter full path to document \n" .
        " --> ";
  $response = <STDIN>;
  chomp $response;
  exit(1) if ($response eq "");
  return $response;
}

sub x_13102_1 {
  print LOG "CTX: Digital Signature Content Evaluation 13102.1\n";
  print LOG "CTX: Evaluation using DSG Schematron\n";

  my ($logLevel, $file) = @_;

  my $x = "java org.apache.xalan.xslt.Process -IN 13102/dsg_schematron.xml -XSL 13102/schematron-basic.xsl -OUT 13102/13102.xsl -PARAM diagnose yes";
  print LOG "CTX: $x\n" if ($logLevel >= 3);
  print LOG `$x`;

  $x = "java org.apache.xalan.xslt.Process -IN $file -XSL 13102/13102.xsl -OUT 13102/schematron_13102.log";
  print LOG "CTX: $x\n" if ($logLevel >= 3);
  print LOG `$x`;

  return 1 if $?;
  return 0;
}

sub appendResult{
  open RESULT, "13102/schematron_13102.log" or die "Could not open output file 13102/schematron_13102.log";
  print LOG "\n\nResult from schematron validation:\n";
  while (<RESULT>) {
	print LOG "$_";
  } 
  print LOG "\n";
}
# Main starts here

 open LOG, ">13102/grade_13102.txt" or die "Could not open output file 13102/grade_13102.txt";
 $mesaVersion = mesa::getMESAVersion();
 print LOG "CTX: $mesaVersion \n";

 die "Usage: perl 13102/eval_13102.pl <log level> FILE" if (scalar(@ARGV) < 1);
 my $fileName;
 if (scalar(@ARGV) > 1) {
  $fileName = $ARGV[1];
 } else {
  $fileName = promptForFile();
 }
 $logLevel = $ARGV[0];
 
 my $diff = 0;
 $diff += x_13102_1($logLevel, $fileName);

 appendResult();

 #print LOG "\nTotal errors: $diff\n";
 #print LOG "This test does not tell you the XML errors; you need to find those yourself\n" if ($diff > 0);
 #print LOG "Check the terminal emulator to see if there are hints, or use a commercial XML tool\n" if ($diff > 0);
 #print "\n Total errors: $diff\n";
 print "This test does not tell you the XML errors; you need to find those yourself\n";
 print "Check the terminal emulator to see if there are hints, or use a commercial XML tool\n";
 print "Logs stored in 13102/grade_13102.txt\n";
 exit 0;

