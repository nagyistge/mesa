<?php
    require "global.inc";
    $source = $_GET["source"];
    $printSource = $actor{$source};
    $actorLink = $actor_link{$source};
    $successLink = $send_discharge_link{$source};
    $title = "<a href=\"index.php\">MESA RIS Mall</a>:: <a href=\"$actorLink\">$printSource</a>:: Send Discharge Message";
    $title_no_link = "MESA RIS Mall:: ". $printSource . " :: Send Discharge Message";
    $header = "MESA RIS Mall";

    $nativedb = "webadt";
    require "common_functions.inc";

	if (!empty($_POST["cancel"])) {
        header("Location:$actorLink");
    }

    check_user_is_logged_in();
	global $visitList;
?>

<html>
<head>
<title><?=$title_no_link?></title>
<link href="include/general.css" type="text/css" rel="Stylesheet" rev="Stylesheet" media="all">
</head>
<body>

<?php 
    print_header($header);

# process_form tells us later if this form is ready to be processed.
# first confirm all the info is here.
    $process_form = false;
	$visitList = getVisitList($_POST["viskey"], $nativedb);
    $submit = $_POST["submit"];
    if ($submit == "Send Message") {
        //if (empty($viskey)) {
	if (empty($_POST["viskey"])) {
            print_user_error("Please select visit."); 
        } else {
            $process_form = true;
        }
    }

    if ($submit == "Search Patients") {
        $lName = $_POST["last_name"];
        if (empty($lName)) {
            print_user_error("Please enter few letters from Patient's last name.");
        } else {
            $lName = strtoupper($lName);
            list ($rows, $visitList) = getVisitListByLname($lName, $_POST["viskey"], $nativedb);
            if ($rows == 0) {
                print_user_error($visitList);
                $visitList = getVisitList($_POST["viskey"], $nativedb);
            }
        }
    }
?>

<?php
if ($process_form) {
# Button has been pressed. First, create the hl7 message

    $viskey = $_POST["viskey"]; 
    $visit = get_patient_visit_by_viskey($viskey, $nativedb);
    if (rtrim($visit["patcla"]) == "O") {
        // Outpatients are not discharged.
		$string = "Outpatients cannot be discharged<br>";
		$link = "Please <a href=\"javascript:history.go(-1)\">go back</a> and choose different patient.";
        print_error_message($title, $string, $link);
        include "footer.inc";
        exit;
    }

    $message = "a03";

    $tmpdir = "$risMallRoot/tmp";
    $dest_id = rtrim($_POST["dest_id"]); 
    $template = "$mesaTarget/templates/hl7/$message.tpl";
    $outputFile = $tmpdir."/$message.hl7";
    $verbose = $_POST["verbose"];
    $vflag = $verbose ? "-v" : "";
    $noSend = !empty($_POST["noSend"]); 
    $short = $_POST["short"];

    #Append $short to $message with ~~ delimiter
    if ($short) {
      $message .= "~~".$short;
    }
#echo "<p>\$message: ". $message ."<p>";

    $bin = "$mesaTarget/bin/mesa_construct_hl7.pl";
    $cmd = "perl $bin $vflag -t $tmpdir -V $viskey " .
        "$message $template $nativedb $dest_id $outputFile";

    execute_piped_command($cmd, $verbose);

    send_hl7($nativedb, $dest_id, $outputFile, $noSend, $verbose, $title, $source, $successLink);
    logSendDischarge($viskey, $dest_id, $nativedb, !$noSend);
} else {
?>
<form action="<?php $PHP_SELF;?>" method="post">
    <table width="820" bgcolor="FFF8CE" border="0" cellspacing="2" cellpadding="2">
        <tr>
            <td bgcolor="#FFD557" colspan="3">
            <?php print_top_nav($title); ?>
            </td>
        </tr>
		<tr>
            <td colspan="3"><b><u>Search for Patients:</u></b></td>
        </tr>
        <tr>
            <td align="left">Patient's lastname begins with </td>
            <td width="5">&nbsp;</td>
            <td align=left><input type=text name="last_name" value="<?=$_POST["last_name"]?>">  </td>
        </tr>
        <tr>
            <td align="left">&nbsp;</td>
            <td width="5">&nbsp;</td>
            <td align=left><input type="submit" name="submit" value="Search Patients"></td>
        </tr>
        <tr>
            <td colspan="3">&nbsp;</td>
        </tr>
        <tr>
            <td align="left" valign="top">Please select visit record</td>
			<td width="5">&nbsp;</td>
            <td align=left>
			<?= $visitList ?>
			</td>
		</tr>
<!--
        <tr>
            <td align="left">Send short version of issuer</td>
            <td width="5">&nbsp;</td>
            <td align=left><input type="checkbox" name="short" value="short"
                <?=$_POST["short"] ? "checked" : "" ?>>
            </td>
        </tr>
-->
        <tr>
            <td align="left">Destination</td>
			<td width="5">&nbsp;</td>
            <td align=left><select name="dest_id">
                <? getDestinationList($_POST["dest_id"], $nativedb) ?>
                </select>
			</td>
		</tr>
        <tr>
            <td align="left">Debugging Output</td>
			<td width="5">&nbsp;</td>
            <td align=left><input type="checkbox" name="verbose" value="verbose"
                <?=$_POST["verbose"] ? "checked" : "" ?>>
			</td>
		</tr>
		<!--
        <tr>
            <td align="left">Do Not Send Message</td>
			<td width="5">&nbsp;</td>
            <td align=left><input type="checkbox" name="noSend" value="noSend">
			</td>
		</tr>
		-->
		<tr>
            <td colspan=3 align=center>
            <input type="submit" name="cancel" value="Cancel">&nbsp;&nbsp;&nbsp;<input type="reset" value="Reset">&nbsp;&nbsp;&nbsp;<input type=submit name="submit" value="Send Message">
            </td>
        </tr>
        <tr>
            <td colspan=3 align=center>
            &nbsp;
            </td>
        </tr>
    </table>
</form>
<?php } ?>
<? include "footer.inc" ?>
</body>
</html>