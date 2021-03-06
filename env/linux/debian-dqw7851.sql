# First section contains MESA specific variables

#
# MESA software location
#

setenv MESA_TARGET /opt/mesa
setenv MESA_STORAGE $MESA_TARGET/storage
setenv MESA_OS LINUX

#
#
setenv MESA_ROOT `pwd`
setenv XERCESCROOT $MESA_ROOT/external/xerces-c-src1_7_0

setenv MESA_INCLUDE $MESA_ROOT/include
setenv MESA_LIB $MESA_ROOT/lib
setenv MESA_MAKE $MESA_ROOT/env/linux/make.debian-gcc4.psql
setenv MESA_BIN $MESA_TARGET/bin
setenv PSQL_INCLUDE /usr/include/postgresql
setenv PSQL_LIB /usr/lib/postgresql/8.3/lib

setenv CTN_LIB $MESA_ROOT/../ctn/lib/debian.psql
setenv INSTALL install

# Now, the variables for building CTN software
setenv DICOM_ROOT `pwd`/../ctn
setenv DICOM_BIN $MESA_BIN
setenv DICOM_LIB $MESA_ROOT/../ctn/lib/debian.psql
setenv DICOM_LIBSRC $MESA_ROOT/../ctn/libsrc
setenv DICOM_INCLUDE $DICOM_ROOT/include
setenv DICOM_MAKE $MESA_ROOT/env/linux/ctnmake.debian-gcc4.psql
setenv CLASSPATH .:$DICOM_ROOT/javactn:$MESA_ROOT/javamesa:$MESA_ROOT/webmesa/mesa-iti:$MESA_ROOT/javaexternal/xerces-1_2_3/xerces.jar:$TOMCAT_HOME/common/lib/servlet-api.jar
setenv JAR_DIRECTORY $MESA_TARGET/lib
setenv JDK_ROOT $JAVA_HOME
setenv INSTALLUID "install -m 4775"
setenv INSTALLINCLUDE "install -m 666"

setenv LEX flex
setenv RANLIB ranlib
