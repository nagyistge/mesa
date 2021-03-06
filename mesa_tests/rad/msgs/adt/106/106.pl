#!/usr/local/bin/perl -w

use Env;

# Generate HL7 messages for Case 106

sub create_text_file {
  `perl $MESA_TARGET/bin/tpl_to_txt.pl $_[0] $_[1] $_[2]`;
}

sub create_hl7 {
  `$MESA_TARGET/bin/txt_to_hl7 -d ihe -b $MESA_TARGET/runtime < $_[0].txt > $_[0].hl7`;
}


create_text_file("green.var",       "../templates/a04.tpl", "106.tmp");
create_text_file("106.102.a04.var", "106.tmp",              "106.102.a04.txt");
create_hl7("106.102.a04");

