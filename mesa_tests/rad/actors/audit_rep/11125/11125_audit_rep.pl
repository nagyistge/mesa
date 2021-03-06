#!/usr/local/bin/perl -w

# Test script for test 11125: Audit Record Repository

use Env;
use File::Copy;
use lib "scripts";
require audit;

$SIG{INT} = \&goodbye;

sub goodbye {
  exit 0;
}

sub announce_end {
  print "This is the end of Audit Record Repository Test 11125. \n";
  print " Answer the questions for this test in the file audit_questions.txt \n";
  print " and email that file with your responses to the Project Manager. \n";
}


($syslogPortMESA,
 $syslogHostTest, $syslogPortTest) = audit::read_config_params();

print "Illegal MESA Syslog Port: $syslogPortMESA \n" if ($syslogPortMESA == 0);
print "Illegal Test Syslog Host: $syslogHostTest \n" if ($syslogHostTest eq "");
print "Illegal Test Syslog Port: $syslogPortTest \n" if ($syslogPortTest == 0);

# Two messages from ADT system
audit::create_send_audit($syslogHostTest, $syslogPortTest,
	"../../msgs/audit/11125/11125.002", "ATNA_PATIENT_RECORD");

audit::create_send_audit($syslogHostTest, $syslogPortTest,
	"../../msgs/audit/11125/11125.004", "ATNA_PATIENT_RECORD");

announce_end();

goodbye;

