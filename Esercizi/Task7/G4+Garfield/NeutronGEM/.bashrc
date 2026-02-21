#+##########################################################################
# File:            .profile                                       
# Description:     general purpose .profile file          
#                  This file gives examples of how to modify the behaviour of
#                  Bourne-shells.
#                  It sources a file on the central file server which is 
#                  starting the HEP scripts if they are not installed locally.
# Authors:         Arnaud Taddei
# Site Contact:    Setup.Support@cern.ch
# Version: 	   3.0.1
# Last Change:	   Mon Nov 20 15:04:50 MET 199
#-############################################################################
if [ -r /afs/cern.ch/sw/lcg/contrib/gcc/4.6.3/x86_64-slc6-gcc46-opt/setup.csh ]; then
   . /afs/cern.ch/sw/lcg/contrib/gcc/4.6.3/x86_64-slc6-gcc46-opt/setup.sh
fi
if [ -r /afs/cern.ch/sw/lcg/external/geant4/9.6.p02/x86_64-slc6-gcc46-opt/bin/geant4.csh ]; then
   . cd/bin/geant4.sh
fi
if [ -r /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.09/x86_64-slc6-gcc46-opt/root/bin/thisroot.csh ]; then
	. /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.09/x86_64-slc6-gcc46-opt/root/bin/thisroot.sh
fi
export ROOT_DIR=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.09/src/root/cmake/scripts
cd ~/programming





#======================================================================#
# The following is a set of examples.
#
# If you want to try them uncomment the line(s) you selected.
#======================================================================#
#
# If you want to have ENVIRONMENT specific settings:
#===================================================
# Template for environment dependent login
# case ${ENVIRONMENT} in
#        INTERACTIVE) # Interactive non-login behaviour of the shell
#                     # (No output)
#                     ;;
#        LOGIN)       # STANDARD LOGIN AND DMLOGIN :
#                     # echo "Environment $ENVIRONMENT"
#                     #
#                     # Warning quota limits:
#                     #======================
#                     # /usr/local/lib/hepix/tools/quota-warn
#                     #
#                     # News:
#                     #======
#                     ;;
#        X11)         # X11 LOGIN (No output):
#                     ;;
#        BATCH)       # NQS,... (No output):
#                     ;;
# esac
#
#
# How can you change your prompt:
#================================
# Definition		 	# Result	# Comment
#-------------------------------------------------------------------
# PS1="[$HOST] "  		# [plus1]	#
# PS1='R; ';		        # R;		#
#
#
#
# How can you change the path:
#=============================
# Basic rule:
#------------
# To include the access to new programs after the defaults:
# PATH=$PATH:new_directory; export PATH
#
# To include the access to new programs before the defaults:
# PATH=new_directory:$PATH; export PATH
#
# Examples of new_directory could be:
# new_directory			# Comment
#-----------------------------------------------------------------------
# /afs/cern.ch/user/f/fred/bin 	# To be able to execute Fred's programs
# /usr/local/bin/gnu		# To access GNU programs
#
# So the next lines are provided in order to help you
# removing the '.' in the path (if any)! (refer to CN/DCI/170 from the UCO)
#-----------------------------------------------
# PATH=`echo $PATH | sed 's/ \.//'`; export PATH
# PATH=`echo $PATH | sed 's/\. //'`; export PATH
#
#
# How can you set environment variables:
#======================================
# Action				# Comment
#--------------------------------------------------------------
# EDITOR=nedit; export EDITOR		# if you want nedit as default
# PRINTER=513-lps; export PRINTER  	# print on 513-lps as default
# LPDEST=513-ps; export LPDEST          # another way to set a printer
# LPDEST="$PRINTER"; export LPDEST      # if PRINTER is defined, use it!
# PRINT_CMD="xprint"; export PRINT_CMD  # The printing command mechanism
#                                       # used at CERN
# PAGER=less; export PAGER		# use less to read man pages
# FMHOME=/afs/cern.ch/@sys/usr/fm4; export FMHOME # to use Framemaker
#
[ -z "$PRINTER" ] && {
    PRINTER=864-2a17-hp
    [ -x /mcr/print/guess_printer ] && { PRINTER=`/mcr/print/guess_printer`; }
}
export PRINTER
LPDEST=$PRINTER; export LPDEST
#
#
# ORACLE
# ------
# If you want ORACLE uncomment the next lines:
#=============================================
#if [ -r /usr/local/lib/hepix/oracle_login.sh ]; then
# . /usr/local/lib/hepix/oracle_login.sh
#fi


#EOF
d=/acc/src/dsc
export CDPATH=.:$d/co:$d/mcr:$d/ctf:$d/lei:$d/ln3:$d/ln4:$d/lin:$d/cps:$d/psb:$d/sps:$d/lhc:$d/tst:$d/ade:$d/co/em
unset d
export LESS="-esSX" 
. /afs/cern.ch/project/parc/env/parc_apps.sh
