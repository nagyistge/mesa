#!/usr/local/bin/perl -w

use Env;
use lib "../../..//common/scripts";
require mesa_common;

sub x_11194_1 {
  print LOG "\nCTX: PMI 11194.1\n";
  print LOG "CTX: Evaluation using ATNA Log Message-Actor Patient Care Protocol Schematron\n";

  my ($logLevel, $file) = @_;
  print LOG "CTX: File $file\n";

#  my $x = "java -cp $MESA_TARGET/lib/xalan.jar org.apache.xalan.xslt.Process -IN 11194/11194_schematron.xml -XSL schematron/schematron-basic.xsl -OUT 11194/11194.xsl -PARAM diagnose yes";
  my $x = "java -cp $MESA_TARGET/lib/xalan.jar org.apache.xalan.xslt.Process -IN 11194/11194_schematron.xml -XSL schematron/schematron-basic.xsl -OUT 11194/11194.xsl -PARAM diagnose yes";
  print LOG "CTX: $x\n" if ($logLevel >= 3);
  print LOG `$x`;

  $x = "java -cp $MESA_TARGET/lib/xalan.jar org.apache.xalan.xslt.Process -IN $file -XSL 11194/11194.xsl -OUT 11194/schematron_11194.log";
  print LOG "CTX: $x\n" if ($logLevel >= 3);
  print LOG `$x`;

  return 1 if $?;
  return 0;
}

sub appendResult {
  open RESULT, "11194/schematron_11194.log" or die "Could not open output file 11194/schematron_11194.log";
  print LOG "\n\nResult from schematron validation:\n";
  while (<RESULT>) {
	print LOG "$_";
  } 
  print LOG "\n";
}
# Main starts here

 die "You need to set JAVA_HOME properly to run this test" if !$JAVA_HOME;
 die "You need to set JAVA_HOME properly to run this test" if ! -e $JAVA_HOME;

 die "Usage: perl 11194/eval_11194.pl <log level> " if (scalar(@ARGV) < 1);
 $logLevel = $ARGV[0];

 open LOG, ">11194/grade_11194.txt" or die "Could not open output file 11194/grade_11194.txt";
 my $mesaVersion = mesa_get::getMESAVersion();
 my ($x, $date, $timeToMinute, $timeToSec) = mesa_get::getDateTime(0);
 print LOG "CTX: $mesaVersion \n";
 print LOG "CTX: current date/time $date $timeToMinute\n";
 print LOG "CTX: Log level $logLevel\n";


 my $fileName = "$MESA_TARGET/logs/syslog/last_log.xml";
 
 my $diff = 0;
 $diff += x_11194_1($logLevel, $fileName);

 appendResult();

 #print LOG "\nTotal errors: $diff\n";
 #print LOG "This test does not tell you the XML errors; you need to find those yourself\n" if ($diff > 0);
 #print LOG "Check the terminal emulator to see if there are hints, or use a commercial XML tool\n" if ($diff > 0);
 #print "\n Total errors: $diff\n";
 print "This test does not tell you the XML errors; you need to find those yourself\n";
 print "Check the terminal emulator to see if there are hints, or use a commercial XML tool\n";
 print "Logs stored in 11194/grade_11194.txt\n";
 exit 0;

