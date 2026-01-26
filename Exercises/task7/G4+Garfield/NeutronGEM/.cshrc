#+###########################################################################
# File:            .cshrc            
# Description:     general purpose .cshrc file           
#                  This file gives examples of how to modify the behaviour of
#                  C-shells (csh and tcsh).
#                  It sources a file on the central file server which is 
#                  starting the HEP scripts if they are not installed locally 
#                  and it compensates csh deficiencies.
# Authors:         Arnaud Taddei
# Site Contact:    Setup.Support@cern.ch
# Version: 	   2.5.1
# Last Change:	   Mon Feb 27 09:42:49 MET 19
#-############################################################################

if ( -r /afs/cern.ch/sw/lcg/contrib/gcc/4.6.3/x86_64-slc6-gcc46-opt/setup.csh ) then
   source /afs/cern.ch/sw/lcg/contrib/gcc/4.6.3/x86_64-slc6-gcc46-opt/setup.csh
endif
if ( -r /afs/cern.ch/sw/lcg/external/geant4/9.6.p02/x86_64-slc6-gcc46-opt/bin/geant4.csh ) then
   source /afs/cern.ch/sw/lcg/external/geant4/9.6.p02/x86_64-slc6-gcc46-opt/bin/geant4.csh /afs/cern.ch/sw/lcg/external/geant4/9.6.p02/x86_64-slc6-gcc46-opt/bin
endif
if ( -r /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.09/x86_64-slc6-gcc46-opt/root/bin/thisroot.csh ) then
	source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.09/x86_64-slc6-gcc46-opt/root/bin/thisroot.csh /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.09/x86_64-slc6-gcc46-opt/root
endif

cd ~/programming
#setenv PATH ${PATH}:${G4WORKDIR}/bin/Linux-g++
#setenv PATH ${PATH}:${G4WORKDIR}/bin/run
setenv OS Linux



#=====================================================================
# The following is a set of examples.
# See CN/DCI/172 from the UCO for more information on customising your shell.
# If you want to try them uncomment the line(s) you selected.
#======================================================================#
#
# If you want to have ENVIRONMENT specific settings:
#===================================================
# Template for environment dependent login
# switch ( $ENVIRONMENT )
#        case INTERACTIVE: # Interactive non-login behaviour of the shell 
#                          # (No output)
#		           breaksw
#        case LOGIN:       # STANDARD LOGIN AND DMLOGIN :
#                          #echo "Environment $ENVIRONMENT"
#                          breaksw
#        case X11:         # X11 LOGIN (No output):
#                          breaksw
#        case BATCH:       # NQS,... (No output):
#                          breaksw
# endsw
#
# How can you change your prompt:
#================================
# Definition		 	# Result	# Comment
#-------------------------------------------------------------------
# set prompt="$HOST "	 	#  		# 
# set prompt='\! > '	 	# 3 >		# 3 means the third 
#			 	# 		# you executed
# set prompt="[$HOST] > "	# [plus1] > 	#
# set prompt='R; '		# R;		# for VM hackers!
#
#
#
# How can you set aliases:
#=========================
# Definition				  # Comment
#---------------------------------------------------------------------
# alias H 'history -r | fgrep "\!*"'	  # just type H latex to get the 
#					  # last latex command
# alias h history			  # just type h for the history
# alias m more				  # just type m for more
# alias findfile "find . -name \!^ -print" # To find a file in a hierarchy
#
#
# How can you change the path:
#=============================
# Basic rule:
#------------
# To include the access to new programs after the defaults:
# set path=( $path new_directory ) 
#
# To include the access to new programs before the defaults:
# set path=( new_directory $path )
#set path=(~/bin $path)
#set path=($path /cern/mad/new/bin)
#
# Examples of new_directory could be:
# new_directory			# Comment
#-----------------------------------------------------------------------
# /afs/cern.ch/user/f/fred/bin 	# To be able to execute Fred's programs
# /usr/local/bin/gnu		# To access GNU programs
#
# The next lines are provided in order to help you
# removing the '.' in the path (if any)! (refer to CN/DCI/170 from the UCO)
#-----------------------------------------------
# set path = ( `echo $path | sed 's/ \.//'`)
# set path = ( `echo $path | sed 's/\. //'`)
#
#
#
# How can you set environment variables:
#=======================================
# Action			# Comment
#--------------------------------------------------------------
setenv EDITOR  /afs/cern.ch/group/pz/bin/e
setenv PRINTER 864-2a17-hp
setenv LPDEST  $PRINTER
#
#
# Changing the shell behaviour:
#==============================
# Normally, redirecting command output to a file, for example
#
#      ls > file.list
#
# will OVERWRITE any existing file.list. If you want to avoid overwriting
# existing files in this way then set the noclobber option:
#
# set noclobber
#
# If you have this option set you can override the extra protection by
# adding a '!' - e.g.,
#
#      ls >! file.list
#
#---------------------------------------------------
# By default you need to type "logout" to logout from a standard CERN Unix
# system. If you prefer to logout with a simple "CTRL-D" you should
#
unset ignoreeof
unset noclobber
#
# BEWARE: If you use the "menu" shell facilities it can be easy to logout
# by mistake if you do this as "CTRL-D" is also used for menu completion
#
#-------------------------------------------------------
#
# The number of commands remembered is usually 40. To change this
# to 100 (that's just an example) uncomment the next line and 
# change the number to suit yourself.
#

set histsize=100
#
#
# ORACLE
# ------
# If you want ORACLE uncomment the next lines:
#=============================================
#if ( $?ORACLE_CERN == 0 ) then
#  if ( -r /usr/local/lib/hepix/oracle_env.csh ) then
#    source /usr/local/lib/hepix/oracle_env.csh
#    # access PS database
#    setenv TWO_TASK         dbpsco
#    setenv ORACLE_SID       dbco
#  endif
#endif
unsetenv ENV

set d=/acc/src/dsc
set cdpath=($d/co $d/mcr $d/ctf $d/lei $d/ln3 $d/lin $d/cps $d/psb $d/sps $d/lhc $d/tst $d/ade $d/co/em )
unset d

setenv NODAL_HISTORY    100
if ($OS == "Linux") then

# for debugging Alloc/free  1=print 2=abort task
#setenv MALLOC_CHECK_ 2
alias ll ls -l

else
# Use only GNU make on all platforms..
alias make gmake
endif
#
# keep man pages still on the terminal when leaving less
setenv LESS "-esSX"

if ( $?TERM != 0 ) then
if ( $TERM == "xterm-color" ) then
    set term=xterm
    source ~/.login
endif
endif
