#!/bin/csh
mkdir -p 555/IMAGES
dcm_modify_object -t -T ../tcia-rembrandt/000002 555/IMAGES/000002 << __EOF
0010 0010 IWRF^PIERRE
0010 0020 555
0010 0021 IHERED&1.3.6.1.4.1.21367.13.20.1000&ISO
0010 0030 19550505
0010 0040 M
0008 0050 ACC-555
0008 0030 120000
0020 0010 444
0020 000D 1.3.6.1.4.21367.999.10.1.1
0020 000E 1.3.6.1.4.21367.999.10.2.1
0008 0018 1.3.6.1.4.21367.999.10.3.3
__EOF