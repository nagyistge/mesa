<?php 
    $title = "Order Placer: Create an Order";
    $nativedb = "webop";
    require "common_functions.inc";
    check_user_is_logged_in();
?>

<html>
<head>
<title><?=$title?></title>
</head>
<body>

<?php 
    print_header($title);
    link_to_main();

# process_form tells us later if this form is ready to be processed.
# first confirm all the info is here.
    $process_form = false;
    $submit = $_POST["submit"];
    if ($submit == "Create") {
        if (empty($viskey)) {
            print_user_error("Please select visit."); 
        } else {
            $process_form = true;
        }
    }

// Constants -- needed for DB as well as for form
    $quatim = "1^once^^^^S";
    $entby = "^ROSEWOOD^RANDOLPH";
    $entorg = "922229-10^IHE-RAD^IHE-CODE-231";
    $ordcalphonum = "314-555-1212";

    if ($process_form) {
        $verbose = $_POST["verbose"];

        $viskey = rtrim($_POST["viskey"]);
        $patient_visit = get_patient_visit_by_viskey($viskey, $nativedb);

        $cmd = "$mesaTarget/bin/mesa_identifier webop pon 2>&1";
        $plaordnum = execute_piped_command($cmd, $verbose, true);
        $plaordnum = rtrim($plaordnum);

        $filordnum = "";
        $uniserid = rtrim($_POST["uniserid"]);
        $visnum = rtrim($patient_visit["visnum"]);

        // orduid is generated by db upon insert
        $ordcon = "NW";
        $plagronum = "";
        $ordsta = "";
        $messta = "REQUESTED";
        $par = "";
        $dattimtra = rtrim($_POST["dattimtra"]);
        $ordpro = rtrim($_POST["ordpro"]);
        $refdoc = $patient_visit["refdoc"];
        $ordeffdattim = $dattimtra;
        $spesou = rtrim($_POST["spesou"]);
        $traarrres = "";
        $reaforstu = rtrim($_POST["reaforstu"]);
        $obsval = "";
        $dancod = rtrim($_POST["dancod"]);
        $relcliinf = rtrim($_POST["relcliinf"]);

        $query = "INSERT INTO ordr " .
            "(plaordnum, filordnum, uniserid, visnum, ordcon, plagronum, " . 
            "ordsta, messta, quatim, par, dattimtra, entby, ordpro, refdoc, " .
            "ordeffdattim, entorg, spesou, ordcalphonum, traarrres, " .
            "reaforstu, obsval, dancod, relcliinf) VALUES " .    
            "('$plaordnum', '$filordnum', '$uniserid', '$visnum', '$ordcon', " .
            "'$plagronum', '$ordsta', '$messta', '$quatim', '$par', " .
            "'$dattimtra', '$entby', '$ordpro', '$refdoc', '$ordeffdattim', " .
            "'$entorg', '$spesou', '$ordcalphonum', '$traarrres', " .
            "'$reaforstu', '$obsval', '$dancod', '$relcliinf')";


        if ($_POST["verbose"]) {
            echo "<pre>SQL Query:\n";
            echo "$query\n</pre>";
        }

        $db_connection = connectDB($nativedb);
        pg_exec($db_connection, $query) or 
            die("Error in query: $query" . pg_errormessage($db_connection));
        $orduid = getCurrentOrderUID($db_connection);

        print_success_message("Successfully Created Order Record");

        logCreateOrder($orduid, $nativedb, true);
        pg_close($db_connection);
    }

?>

<form action="<?php $PHP_SELF;?>" method="post">
    <table>
        <tr>
            <th align=right>Please select visit record:
            <td align=left><select name="viskey" size=8>
                <? getVisitList($_POST["viskey"], $nativedb) ?>
                </select>
        <tr>
            <th align=right>Placer Order Number:
            <td align=left><b>Automatically Assigned</b>
        <tr>
            <th align=right>Filler Order Number:
            <td align=left><b>&quot &quot</b>
        <tr>
            <th align=right>Universal Service ID:
            <td align=left><select name="uniserid"> 
                <?php  
                    $fn = "../config/uniserid.txt";
                    configFileOptions($fn, $_POST["uniserid"]);
                ?>
            </select>  
        <tr>
            <th align=right>Visit Number:
            <td align=left><b>From Selected Visit</b>
        <tr>
            <th align=right>Order UID:
            <td align=left><b>Automatically Assigned</b>
        <tr>
            <th align=right>Order Control:
            <td align=left><b>NW</b>
        <tr>
            <th align=right>Placer Group Number:
            <td align=left><b>&quot &quot</b>
        <tr>
            <th align=right>Order Status:
            <td align=left><b>&quot &quot</b>
        <tr>
            <th align=right>MESA Status:
            <td align=left><b>REQUESTED</b>
        <tr>
            <th align=right>Quantity/Timing
            <td align=left><b><?=$quatim?></b>
        <tr>
            <th align=right>Parent:
            <td align=left><b>&quot &quot</b>
        <tr>
            <th align=right>Date/Time of Transaction:
            <td align=left><b><?php echo 
                    $dattimtra = `perl ../bin/getMesaInfo.pl datetime`?></b>
            <!-- Post values of dattimtra secretly -->
            <input type="hidden" name="dattimtra" value="<?=$dattimtra?>">
        <tr>
            <th align=right>Entered By:
            <td align=left><b><?=$entby?></b>
        <tr>
            <th align=right>Ordering Provider:
            <td align=left><select name="ordpro"> 
                <?php  
                    $fn = "../config/ordpro.txt";
                    configFileOptions($fn, $_POST["ordpro"]);
                ?>
            </select>  
        <tr>
            <th align=right>Referring Doctor:
            <td align=left><b>From Selected Visit</b>
        <tr>
            <th align=right>Order Effective Date/Time
            <!-- same as Date/Time of Transacton -->
            <td align=left><b><?=$dattimtra ?></b>
        <tr>
            <th align=right>Entering Organization:
            <td align=left><b><?=$entorg?></b>
        <tr>
            <th align=right>Specimen Source:
            <td align=left><select name="spesou">
                <?php  
                    $fn = "../config/spesou.txt";
                    configFileOptions($fn, $_POST["spesou"]);
                ?>
                </select>
        <tr>
            <th align=right>Order Callback Phone Number
            <td align=left><b><?=$ordcalphonum?></b>
        <tr>
            <th align=right>Transport Arrangement Responsibility:
            <td align=left><b>&quot &quot</b>
        <tr>
            <th align=right>Reason for Study
            <td align=left><input type=text name="reaforstu" 
                value="<?=$_POST["reaforstu"]?>">
        <tr>
            <th align=right>Observation Value
            <td align=left><b>&quot &quot</b>
        <tr>
            <th align=right>Danger Code
            <td align=left><select name="dancod">
                <?php  
                    $fn = "../config/dancod.txt";
                    configFileOptions($fn, $_POST["dancod"]);
                ?>
                </select>
        <tr>
            <th align=right>Relevant Clinical Info
            <td align=left><input type=text name="relcliinf" 
                value="<?=$_POST["relcliinf"]?>">
        <tr>
            <th align=right>Debugging Output:
            <td align=left><input type="checkbox" name="verbose" value="verbose"
                <?=$_POST["verbose"] ? "checked" : "" ?>>
        <tr>
            <td colspan=2 align=center>
            <input type=submit name="submit" value="Create">
    </table>
</form>

<? include "footer.inc" ?>

</body>
</html>