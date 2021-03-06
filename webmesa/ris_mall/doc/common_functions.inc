<?php
include "header.inc";
include "user.inc";
include "log.inc";

session_start();

$dbuser = "postgres";

# Setting environment variables...
# We expect the following to be set as apache server environment vars:
#   MESA_TARGET
#   RIS_MALL_DOC_ROOT
# Typically these are set with the SetEnv command in the apache config
# files (httpd.conf, .htaccess, apache.conf)
# If MESA_TARGET is not set it is an error.
# If RIS_MALL_DOC_ROOT is not set, we will parse the SCRIPT_FILENAME
# server variable to try to ascertain the document root (assuming that
# this script is in the doc subdir of the root dir)
#
# We will then set the MESA_TARGET and MESA_BIN environment variables,
# as various scripts we might call depend on them.
# The env var PGUSER will also be set.
# Finally, we will set internal variables ($mesaTarget, $risMallRoot)
# which will be referenced internally.
# this is necessary when we set the MESA_TARGET env in apache.config

# The location of the mesa install directory
$mesaTarget = $_SERVER["MESA_TARGET"];
if (!$mesaTarget) 
    die("MESA_TARGET not set in apache.conf");
elseif (!is_dir($mesaTarget))
    die("MESA_TARGET = $mesaTarget does not exist");

# The location of the base ris_mall document dir
$risMallRoot = $_SERVER["RIS_MALL_DOC_ROOT"];
if (($risMallRoot) && (!is_dir($risMallRoot)))
    die("RIS_MALL_DOC_ROOT = $risMallRoot does not exist");
if (!$risMallRoot) {
    // Assume calling script is in doc dir or a subdir of it!  
    // This imposes a restriction on our directory structure, but 
    // I'm assuming that everything is installed it as it should.
    $dir = $_SERVER["SCRIPT_FILENAME"];
    do {
        $topname = basename($dir);
        $dir = dirname($dir);
        if ($dir == "/") break;
    } while ($topname != "doc");
    if ($dir == "/") {
        trigger_error("Cannot determine Ris Mall document root.  " .
                "Setting it to /.<br>", E_USER_WARNING); 
        // If doc not found, keep it at /.  This will likely work for getting
        // a temp directory (/tmp exists on most unix systems) and will
        // probably yield permission errors for other uses.
    }
    $risMallRoot = $dir;
}

putenv("MESA_TARGET=$mesaTarget");
putenv("MESA_BIN=$mesaTarget/bin");
putenv("PGUSER=".$dbuser);

# Creates options for a select control by reading from a text file.
# Each line in a file becomes an option.  Comments (#...) are ignored.
# the optional parameter value is the current selection, which will be
# selected.
# It is expected that this is called after the <select> tag.
# values of "." are mapped to empty entries.
function configFileOptions($filename, $value="") {

    $fh = fopen($filename, 'r') or 
        die("</select><br>Can't open $filename: $php_errormsg");
	system (`/bin/sort $filename`);
    while (!feof($fh)) {
        $l = rtrim(fgets($fh, 1024));  # strip off trailing whitespace
        $l = preg_replace('/#.+$/', '', $l); # strip off comments
        if ("" == $l) continue;  # ignore blank lines 
        if ("." == $l)      # lines "." are mapped to blank entries
            $l = "";
        $tag = $l==$value ? "<option selected>" : "<option>";
        echo "$tag $l";
    }
    fclose($fh) or die($php_errormsg);
}

function connectDB($db, $host="localhost", $user="", $pass="") {
    global $dbuser;
    if ($user=="") $user = $dbuser;
    if ($pass=="")
        $connect_string = "host=$host user=$user dbname=$db";
    else
        $connect_string = "host=$host user=$user password=$pass dbname=$db";
    
    $db_connection = pg_connect($connect_string);
    if (!$db_connection) {
        die ("Cannot connect to db='$db' <br>(connect string: $connect_string)");
    }
    return $db_connection;
}

function getPatientList($selectedKey, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT patient_key, patid, issuer, nam, datbir from patient " .
        "ORDER BY nam";
    $result = pg_exec($db_connection, $query) or 
         die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows > 0) {
		$patientList = "<select name=\"patient_key\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$patkey = rtrim($row['patient_key']);
        	$patid = rtrim($row['patid']);
        	$issuer = rtrim($row['issuer']);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);
        	$datbir = rtrim($row['datbir']);
        	$sel = $selectedKey==$patkey ? "selected" : "";
        	$patientList .= "<option value=$patkey $sel>$nam, $datbir, $patid, $issuer\n";
		}
		$patientList .= "</select>\n";
    }

    // close database connection
    pg_close($db_connection);
	return $patientList;
}

function getPatientListByLname($lName, $selectedKey, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT patient_key, patid, issuer, nam, datbir from patient " .
        "WHERE nam ilike '$lName%' ORDER BY nam";
    $result = pg_exec($db_connection, $query) or 
         die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows > 0) {
		$patientList = "<select name=\"patient_key\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$patkey = rtrim($row['patient_key']);
        	$patid = rtrim($row['patid']);
        	$issuer = rtrim($row['issuer']);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);

        	$datbir = rtrim($row['datbir']);
        	$sel = $selectedKey==$patkey ? "selected" : "";
        	$patientList .= "<option value=$patkey $sel>$nam, $datbir, $patid, $issuer\n";
		}
		$patientList .= "</select>\n";
    } else {
        $patientList = "Could not find any patient with the string <b>$lName</b>.";
    }

    // close database connection
    pg_close($db_connection);
	return array ($rows, $patientList);
}

function getPatientListByLname2($lName, $selectedKey, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT patient_key, patid, issuer, nam, datbir from patient " .
        "WHERE nam ilike '$lName%' ORDER BY nam";
    $result = pg_exec($db_connection, $query) or 
         die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows > 0) {
		$patientList = "<select name=\"$selectedKey\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$patkey = rtrim($row['patient_key']);
        	$patid = rtrim($row['patid']);
        	$issuer = rtrim($row['issuer']);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);
        	$datbir = rtrim($row['datbir']);
        	$sel = $selectedKey==$patkey ? "selected" : "";
        	$patientList .= "<option value=$patkey $sel>$nam, $datbir, $patid, $issuer\n";
		}
		$patientList .= "</select>\n";
    } else {
        $patientList = "Could not find any patient with the string <b>$lName</b>.";
    }

    // close database connection
    pg_close($db_connection);
	return array ($rows, $patientList);
}

function getPatientList2($selectedKey, $db, $setName) {
    $db_connection = connectDB($db);
    $query = "SELECT patient_key, patid, issuer, nam, datbir from patient " .
        "ORDER BY nam";
    $result = pg_exec($db_connection, $query) or 
         die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows > 0) {
		$patientList = "<select name=\"$setName\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$patkey = rtrim($row['patient_key']);
        	$patid = rtrim($row['patid']);
        	$issuer = rtrim($row['issuer']);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);
        	$datbir = rtrim($row['datbir']);
        	$sel = $selectedKey==$patkey ? "selected" : "";
        	$patientList .= "<option value=$patkey $sel>$nam, $datbir, $patid, $issuer\n";
		}
		$patientList .= "</select>\n";
    }

    // close database connection
    pg_close($db_connection);
	return $patientList;
}


function getVisitList($selectedVisitKey, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT viskey, nam, sex, datbir, visnum, patcla, admdat " .
        "FROM patient_visit ORDER BY nam"; 
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows > 0) {
		$visitList = "<select name=\"viskey\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$viskey = rtrim($row['viskey']);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);
        	$sex = rtrim($row['sex']);
        	$datbir = rtrim($row['datbir']);
        	$visnum = rtrim($row['visnum']);
        	$patcla = rtrim($row['patcla']);
        	$admdat = rtrim($row['admdat']);
        	$sel = $selectedVisitKey == $viskey ? "selected" : "";
        	$visitList .= "<option value=$viskey $sel>$nam, $sex, $datbir, $visnum, " .
            	"$patcla, $admdat\n";
    	}
		$visitList .= "</select>\n";
	}

    // close database connection
    pg_close($db_connection);
	return $visitList;
}

function getVisitListByLname($lName, $selectedVisitKey, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT viskey, nam, sex, datbir, visnum, patcla, admdat " .
        "FROM patient_visit WHERE nam ilike '$lName%' ORDER BY nam"; 
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows > 0) {
		$visitList = "<select name=\"viskey\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$viskey = rtrim($row['viskey']);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);
        	$sex = rtrim($row['sex']);
        	$datbir = rtrim($row['datbir']);
        	$visnum = rtrim($row['visnum']);
        	$patcla = rtrim($row['patcla']);
        	$admdat = rtrim($row['admdat']);
        	$sel = $selectedVisitKey == $viskey ? "selected" : "";
			$visitList .= "<option value=$viskey $sel>$nam, $sex, $datbir, $visnum, $patcla, $admdat\n";
    	}
		$visitList .= "</select>\n";	
	} else {
		$visitList = "Could not find any patient with the string <b>$lName</b>.";
	}

    // close database connection
    pg_close($db_connection);
	return array ($rows, $visitList);
}

function getOrderList($selectedOrduid, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT nam, datbir, admdat, uniserid, orduid " .
        "FROM patient_visit_order ORDER BY nam"; 
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows > 0) {
		$orderList = "<select name=\"orduid\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);
        	$datbir = rtrim($row['datbir']);
        	$admdat = rtrim($row['admdat']);
        	$uniserid = rtrim($row['uniserid']);
        	$orduid = rtrim($row['orduid']);
        	$sel = $selectedOrduid == $orduid ? "selected" : "";
        	$orderList .= "<option value=$orduid $sel>$nam, $datbir, $admdat, $uniserid (order id=$orduid)\n";
		}
		$orderList .= "</select>\n";
    }

    // close database connection
    pg_close($db_connection);
	return $orderList;
}

function getOrderListByLname($lName, $selectedOrduid, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT nam, datbir, admdat, uniserid, orduid " .
        "FROM patient_visit_order WHERE nam ilike '$lName%' ORDER BY nam"; 
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows > 0) {
		$orderList = "<select name=\"orduid\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);
        	$datbir = rtrim($row['datbir']);
        	$admdat = rtrim($row['admdat']);
        	$uniserid = rtrim($row['uniserid']);
        	$orduid = rtrim($row['orduid']);
        	$sel = $selectedOrduid == $orduid ? "selected" : "";
        	$orderList .= "<option value=$orduid $sel>$nam, $datbir, $admdat, $uniserid (order id=$orduid)\n";
    	}
		$orderList .= "</select>\n";	
	} else {
		$orderList = "Could not find any patient with the string <b>$lName</b>.";
	}

    // close database connection
    pg_close($db_connection);
	return array($rows, $orderList);
}

function getMWLList($selectedMWLKey, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT mwl_key, mwl.nam, mwl.datbir, admdat, uniserid " . 
        "FROM mwl JOIN patient_visit_order USING (orduid) ORDER BY nam";
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows >0) {
		$mwlList = "<select name=\"mwl_key\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$mwl_key = rtrim($row['mwl_key']);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);
        	$datbir = rtrim($row['datbir']);
        	$admdat = rtrim($row['admdat']);
        	$uniserid = rtrim($row['uniserid']);
        	$sel = $selectedMWLKey == $mwl_key ? "selected" : "";
        	$mwlList .=  "<option value=$mwl_key $sel>$nam, $datbir, $admdat, $uniserid\n";
    	}
		$mwlList .= "</select>\n";
	}

    // close database connection
    pg_close($db_connection);
	return ($mwlList);
}

function getMWLListByLname($lName, $selectedMWLKey, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT mwl_key, mwl.nam, mwl.datbir, admdat, uniserid " . 
        "FROM mwl JOIN patient_visit_order USING (orduid) WHERE mwl.nam ilike '$lName%' ORDER BY nam";
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

	if ($rows >0) {
		$mwlList = "<select name=\"mwl_key\" size=8>\n";
    	for ($i=0; $i<$rows; $i++) {
        	$row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        	$mwl_key = rtrim($row['mwl_key']);
        	$nam = rtrim($row['nam']);
			$nam = fromTo($nam);
        	$datbir = rtrim($row['datbir']);
        	$admdat = rtrim($row['admdat']);
        	$uniserid = rtrim($row['uniserid']);
        	$sel = $selectedMWLKey == $mwl_key ? "selected" : "";
        	$mwlList .=  "<option value=$mwl_key $sel>$nam, $datbir, $admdat, $uniserid\n";
    	}
		$mwlList .= "</select>\n";
	} else {
        $mwlList = "Could not find any patient with the string <b>$lName</b>.";
    }

    // close database connection
    pg_close($db_connection);
	return array ($rows, $mwlList);
}


function getDestinationList($selectedDest, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT dest_id, hostname, com_nam, actor_type, port, rec_app " .
        "FROM hl7_destination ORDER BY rec_app";
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

    for ($i=0; $i<$rows; $i++) {
        $row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        $dest_id = rtrim($row['dest_id']);
        $hostname = rtrim($row['hostname']);
        $com_nam = rtrim($row['com_nam']);
        $rec_app = rtrim($row['rec_app']);
        $actor_type = rtrim($row['actor_type']);
        $port = rtrim($row['port']);
        if ($selectedDest == "") {
            if (preg_match("/^MESA_RIS_MALL/", $rec_app)) $sel = "selected";
            else $sel = "";
        } else 
            $sel = $selectedDest == $dest_id ? "selected" : "";
        echo "<option value=$dest_id $sel>$rec_app ($com_nam $actor_type" .
            " $hostname:$port)\n";
    }

    // close database connection
    pg_close($db_connection);
}

function getDestinationListMod($selectedDest, $db) {
    $db_connection = connectDB($db);
    $query = "SELECT dest_id, hostname, com_nam, actor_type, port, rec_app " .
        "FROM hl7_destination where actor_type = 'IM'";
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    $rows = pg_numrows($result);

    for ($i=0; $i<$rows; $i++) {
        $row = pg_fetch_array($result, $i, PGSQL_ASSOC);
        $dest_id = rtrim($row['dest_id']);
        $hostname = rtrim($row['hostname']);
        $com_nam = rtrim($row['com_nam']);
        $rec_app = rtrim($row['rec_app']);
        $actor_type = rtrim($row['actor_type']);
        $port = rtrim($row['port']);
        if ($selectedDest == "") {
            if (preg_match("/^MESA_RIS_MALL/", $rec_app)) $sel = "selected";
            else $sel = "";
        } else 
            $sel = $selectedDest == $dest_id ? "selected" : "";
        echo "<option value=$dest_id $sel>$rec_app ($com_nam $actor_type" .
            " $hostname:$port)\n";
    }

    // close database connection
    pg_close($db_connection);
}

// A general purpose DB call which returns first row matching query, and dies
// if no rows match.  Good for selecting by ID, where expect only one returned row.
// Returns the associative array with column names as keys.
function get_query_row($query, $db) {
    $db_connection = connectDB($db);
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" . 
            pg_errormessage($db_connection));

    if (pg_numrows($result) == 0) 
        die("No results for query (db='$db'):\n$query");

    $row = pg_fetch_array($result, 0, PGSQL_ASSOC);
    pg_close($db_connection);
    return $row;
}

// Returns the associative array containing results of patient_visit table
// for given viskey.  First row is returned, in the weird case there'd be more than
// one result.  Dies on error.
function get_patient_visit_by_viskey($viskey, $db) {
    $query = "SELECT * FROM patient_visit WHERE viskey = '$viskey'"; 
    return get_query_row($query, $db);
}

// Returns the associative array containing results of hl7_destination table
// for given dest_id.  First row is returned, in the weird case there'd be 
// more than one result.  Dies on error.
function get_destination_by_id($dest_id, $db) {
    $query = "SELECT * FROM hl7_destination WHERE dest_id = '$dest_id'";
    return get_query_row($query, $db);
}

// Returns the associative array containing results of patient table
// for given patient_key.  First row is returned, in the weird case 
// there'd be more than one result.  Dies on error.
function get_patient_by_key($patient_key, $db) {
    $query = "SELECT * FROM patient WHERE patient_key = '$patient_key'";
    return get_query_row($query, $db);
}

// Returns the associative array containing results of patient_visit_order view 
// for given order uid.  First row is returned, in the weird case 
// there'd be more than one result.  Dies on error.
function get_patient_visit_order_by_key($orduid, $db) {
    $query = "SELECT * FROM patient_visit_order WHERE orduid = '$orduid'";
    return get_query_row($query, $db);
}

// returns true if there exists an entry in given db for this user_name 
// and md5(password), and false if there does not.
function confirm_user_name_password($db, $user_name, $password, $require_pw) {
    $found = false;
    $db_connection = connectDB($db);

    if ($require_pw) {
        $query="SELECT * FROM \"user_table\" WHERE user_name='$user_name' " .
            "AND password='". md5($password) ."'";
    } else {
        $query="SELECT * FROM \"user_table\" WHERE user_name='$user_name'";
    }
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    if (pg_numrows($result) > 0) 
        $found = true;

    // close database connection
    pg_close($db_connection);
    return $found;
}

function execute_piped_command($cmd, $verbose, $quiet=false) {
    if ($verbose) $quiet = false;
    if ($verbose) echo "<pre>Executing:\n$cmd</pre>";

    // Now execute the command using popen, piping stderr to stdout.  Print all
    // stdout; however, without the verbose flag our command is quiet except on
    // error.  The return value of the command is captured upon pclose().
    // Returns the last non-empty line output.
    // if $quiet = true, does not output except on errors.
    $ph = popen("$cmd 2>&1", "r") 
        or die("Error executing command:\n$cmd\n\t$php_errormsg");
    if (!$quiet) echo "<pre>";
    while (!feof($ph)) {
        $line = fgets($ph, 1024);
        if (!$quiet) echo $line;
        if (rtrim($line) != "")
            $retline = rtrim($line);
    }
    if (!$quiet) echo "</pre>";
    $retval = pclose($ph);
    if ($retval) {
        print_user_error("Encountered error executing<br><code>$cmd</code>\n");
        die();
    }
    return $retline;
}

function send_hl7($db, $dest_id, $outputFile, $noSend, $verbose, $title, $source, $successLink) {
	require "global.inc";
    global $mesaTarget;
    // Construct send_hl7 command
    $dest = get_destination_by_id($dest_id, $db);
    $hostname = rtrim($dest["hostname"]);
    $port = rtrim($dest["port"]);
	$printSource = $actor{$source};
    $actorLink = $actor_link{$source};

    $bin = "$mesaTarget/bin/send_hl7";
    $cmd = "$bin $hostname $port $outputFile";
	//$link = "Click <a href=\"$actorLink?source=$source\">here</a> to go back to $printSource test control page.";
   	print_success_message($title, "Message sent successfully", $successLink);
    if ($verbose) echo "<pre>\nSending command:\n$cmd\n</pre>";

    if ($noSend) {  // do not actually send
        print_user_error("<em>Message not sent</em>");
    } else {
        // and execute the command.
        $ph = popen("$cmd 2>&1", "r") 
            or die("Error executing command:\n$cmd\n\t$php_errormsg");
        echo "<pre>";
        while (!feof($ph)) {
            echo fgets($ph, 1024);
        }
        echo "</pre>";
        $retval = pclose($ph);
        if ($retval) 
            die("Encountered error executing<br><code>$cmd</code>\n");

        // now delete the temporary hl7 file
        #unlink($outputFile) or die("Can't delete $outputFile: $php_errormsg\n");

    }
}

function send_hl7_ihe_iti($db, $dest_id, $outputFile, $noSend, $verbose, $title, $source, $successLink) {
	require "global.inc";
   	global $mesaTarget;
   	// Construct send_hl7 command
   	$dest = get_destination_by_id($dest_id, $db);
   	$hostname = rtrim($dest["hostname"]);
   	$port = rtrim($dest["port"]);
    $printSource = $actor{$source};
   	$actorLink = $actor_link{$source};
    $parse_flag = "-d ihe-iti";
                                                                                       
   	$bin = "$mesaTarget/bin/send_hl7";
   	$cmd = "$bin $parse_flag $hostname $port $outputFile";
     //$link = "Click <a href=\"$actorLink?source=$source\">here</a> to go back to $printSource test control page.";
    print_success_message($title, "Message sent successfully", $successLink);
   if ($verbose) echo "<pre>\nSending command:\n$cmd\n</pre>";
   if ($noSend) {  // do not actually send
       print_user_error("<em>Message not sent</em>");
   } else {
       // and execute the command.
       $ph = popen("$cmd 2>&1", "r")
           or die("Error executing command:\n$cmd\n\t$php_errormsg");
       echo "<pre>";
	   while (!feof($ph)) {
           echo fgets($ph, 1024);
       }
       echo "</pre>";
       $retval = pclose($ph);
       if ($retval)
           die("Encountered error executing<br><code>$cmd</code>\n");
       // now delete the temporary hl7 file
       #unlink($outputFile) or die("Can't delete $outputFile: $php_errormsg\n");
   }
}

function getCurrentValue($db_connection, $sequence_name) {
    $query = "SELECT currval('$sequence_name')";
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));

    if (pg_numrows($result) == 0) 
        die("No results for query (db='$db'):\n$query");

    $row = pg_fetch_array($result, 0, PGSQL_ASSOC);
    return $row['currval'];
}

# Returns the log ID of the most recently added log entry.  Must be called with 
# an open db connection
function getCurrentLogID($db_connection) {
    return getCurrentValue($db_connection, 'logentry_log_id_seq');
}

# Returns the patient key of the most recently added patient entry.  
# Must be called with an open db connection.
function getCurrentPatientKey($db_connection) {
    return getCurrentValue($db_connection, 'patient_patient_key_seq');
}

# Returns the visit key of the most recently added visit entry.  Must be 
# called with an open db connection.
function getCurrentVisitKey($db_connection) {
    return getCurrentValue($db_connection, 'visit_viskey_seq');
}

# Returns the order uid of the most recently added order entry.  Must be 
# called with an open db connection.
function getCurrentOrderUID($db_connection) {
    return getCurrentValue($db_connection, 'ordr_orduid_seq');
}

# Returns the mwl uid of the most recently added order entry.  Creates its own
# db connection, and we query the db directly for the latest sequence.  This makes
# it possible, if there are a number of simultaneous DB accesses, that we won't get
# the UID we're looking for, but since we don't have a connection, this is the 
# best we can do.
function getRecentMWLUID($db_name) {
    $db_connection = connectDB($db_name);
    $query = "SELECT last_value FROM mwl_mwl_key_seq";
    $result = pg_exec($db_connection, $query) or 
            die("Error in query (db='$db'): $query" .
            pg_errormessage($db_connection));
    pg_close($db_connection);

    if (pg_numrows($result) == 0) 
        die("No results for query (db='$db'):\n$query");

    $row = pg_fetch_array($result, 0, PGSQL_ASSOC);
    return $row['last_value'];
}

# Print a user error in a font which stands out.
function print_user_error($string) {
    //echo "<h4><FONT color=\"red\">$string</FONT></h4>";
	print <<<HTML
<table width="820" bgcolor="#FFFFFF" border="0" cellspacing="2" cellpadding="2">
    <tr>
        <td colspan="3" align=left>&nbsp;</td>
    </tr>
    <tr>
        <td colspan="3" align=left>
        <img src="images/redballsm.gif" border=0">&nbsp;<font class="error">$string</font>
        </td>
    </tr>
    <tr>
        <td colspan="3" align=left>&nbsp;</td>
    </tr>
</table>
HTML;
}

function print_success_message($title, $string, $link) {
    //echo "<h3><FONT color=\"green\">$string</FONT></h3>";
	        print <<<HTML
<table width="820" bgcolor="FFF8CE" border="0" cellspacing="0" cellpadding="0">
    <tr>
        <td align="left" bgcolor="FFD557"><b>$title</b></td>
		<td align="right" bgcolor="FFD557"><b><a href="logout.php">Logout</a></b></td>
    </tr>
    <tr>
        <td colspan="2" align=left>&nbsp;</td>
    </tr>
    <tr>
        <td colspan="2" align=left>
                <h3><font color="green">$string</font></h3>
                $link
        </td>
    </tr>
    <tr>
        <td colspan="2" align=left>&nbsp;</td>
    </tr>
</table>
HTML;
}

function print_error_message($title, $string, $link) {
    //echo "<h3><FONT color=\"green\">$string</FONT></h3>";
	        print <<<HTML
<table width="820" bgcolor="FFF8CE" border="0" cellspacing="0" cellpadding="0">
    <tr>
        <td bgcolor="FFD557" align="left"><b>$title</b></td>
		<td bgcolor="FFD557" align="right"><b><a href="logout.php">Logout</a></b></td>
    </tr>
    <tr>
        <td colspan="2" align=left>&nbsp;</td>
    </tr>
    <tr>
        <td colspan="2" align=left>
                <h3><font color="green">$string</font></h3>
                $link
        </td>
    </tr>
    <tr>
        <td colspan="3" align=left>&nbsp;</td>
    </tr>
</table>
HTML;
}

function print_top_nav($title) {
	print <<<HTML
<table width="820" border="0">
                <tr>
                    <td align="left"><b>$title</b></td>
                    <td align="right"><b><a href="logout.php">Logout</a></b></td>
                </tr>
            </table>
HTML;
}

function fromTo($nam) {
	if (preg_match('/~/', $nam)) {
		exec("perl /opt/mesa/webmesa/ris_mall/bin/eucJP_iso2022JP.pl $nam", $result, $ret );
        #print "result: $result[0]\n";
        #print "ret: $ret\n";
        $nam = $result[0];
	} else {;}
	return $nam;
}
?>
