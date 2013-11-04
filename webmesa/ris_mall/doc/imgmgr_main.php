<?php 
	require "global.inc";
    $header = "MESA RIS Mall"; 
    $title_no_link = "MESA RIS Mall:: Image Manager"; 
	$title = "<a href=\"index.php\">MESA RIS Mall</a>:: Image Manager";
    require "common_functions.inc";
    check_user_is_logged_in();
?>

<html>
<head>
  <title><?=$title_no_link?></title>
  <link href="include/general.css" type="text/css" rel="Stylesheet" rev="Stylesheet" media="all">
</head>
<body>

<?php 
    print_header($header);
?>

<form action="imgmgr.php" method="post">
    <input type="hidden" name="source" value="imgmgr">
    <table width="820" bgcolor="FFF8CE" border="0">
        <tr>
            <td bgcolor="#FFD557" colspan="4">
            <?php print_top_nav($title); ?>
            </td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td colspan="3">
			<br>
			Please select a task below and click the Submit button.
	    </td>
        </tr>
        <tr>
            <td colspan="4">&nbsp;</td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td width="8" align="center"><input type="radio" name="imgmgrSelect" value="imgmgr_1"></td>
            <td width="5">&nbsp;</td>
            <td align=left>Existing Patient, Visit, Order, and SPS: Send a scheduling Message.</td>
        </tr>
        <tr>
            <td colspan="4">&nbsp;</td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td width="8" align="center"><input type="radio" name="imgmgrSelect" value="imgmgr_2"></td>
            <td width="5">&nbsp;</td>
            <td align=left>New Patient: Create Patient, and Visit</td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td width="8" align="center"><input type="radio" name="imgmgrSelect" value="imgmgr_3"></td>
            <td width="5">&nbsp;</td>
            <td align=left>Existing Patient: Create Visit, Order.</td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td width="8" align="center"><input type="radio" name="imgmgrSelect" value="imgmgr_8"></td>
            <td width="5">&nbsp;</td>
            <td align=left>Existing Patient: Send ADT</td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td width="8" align="center"><input type="radio" name="imgmgrSelect" value="imgmgr_4"></td>
            <td width="5">&nbsp;</td>
            <td align=left>Existing Patient, Visit, and Order: Send ADT and ORM to DSS (MESA/Vendor).&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
        </tr>
        <tr>
            <td colspan="4">&nbsp;</td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td width="8" align="center"><input type="radio" name="imgmgrSelect" value="imgmgr_5"></td>
            <td width="5">&nbsp;</td>
            <td align=left>Existing Patient, Visit, and Order: Schedule MWL.</td>
        </tr>
        <tr>
            <td colspan="4">&nbsp;</td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td width="8" align="center"><input type="radio" name="imgmgrSelect" value="imgmgr_6"></td>
            <td width="5">&nbsp;</td>
            <td align=left>Send A08 (Rename Message)</td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td width="8" align="center"><input type="radio" name="imgmgrSelect" value="imgmgr_7"></td>
            <td width="5">&nbsp;</td>
            <td align=left>Send A40 (Merge Message)</td>
        </tr>
        <tr>
            <td colspan="4">&nbsp;</td>
        </tr>
        <tr>
            <td width="5">&nbsp;</td>
            <td width="8">&nbsp;</td>
            <td colspan="2"><input type="submit" name="cancel" value="Cancel">&nbsp;&nbsp;&nbsp;<input type="submit" name="imgmgrSelection" value="Submit"></td>
        </tr>
        <tr>
            <td colspan="4">&nbsp;</td>
        </tr>
    </table>
</form>

<? include "footer.inc" ?>

</body>
</html>
