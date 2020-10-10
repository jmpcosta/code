#!/bin/bash

# -----------------------------------------------------------
#
# Purpose: This script is intended to serve as a template 
#          for scripts that processe parameters.
#
# Usage:   <script> command subcommand parameters
#
# Author:  Joao Costa
# Repo:    https://github.com/jmpcosta/code
#
# -----------------------------------------------------------



# -----------------------------------------------------------
#
# Global Variables
#
# -----------------------------------------------------------



# -----------------------------------------------------------
#
# Derived Variables
#
# -----------------------------------------------------------

SCRIPT_NAME="$0"

# -----------------------------------------------------------
#
# Constants 
#
# -----------------------------------------------------------

# Error codes
ERROR_NONE=0

# -----------------------------------------------------------
#
#
# Functions
#
#
# -----------------------------------------------------------



# -----------------------------------------------------------
# Inform the operation of the script command line options
# -----------------------------------------------------------

display_usage()
{
 printf "\n\t${SCRIPT_NAME}\t<command> <subcommand> <parameter list>\n"
 printf "\n\tWhere:\n"
 printf "\t\tcommand 1:    <Explanation of what the command does>\n"
 printf "\t\t\tsubcommand 1: <Explanation of what the subcommand does>\n"
 printf "\t\t\tsubcommand N: <Explanation of what the subcommand does>\n"
 printf "\n"
 printf "\t\tcommand N:    <Explanation of what the command does>\n"
 printf "\t\t\tsubcommand 1: <Explanation of what the subcommand does>\n"
 printf "\t\t\tsubcommand N: <Explanation of what the subcommand does>\n"
 
 return 0
}


# -----------------------------------------------------------
# Execute the subcommands
# -----------------------------------------------------------

# Each sub command needs a new function
execute_subcmd1()
{
 typeset -i rc=$ERROR_NONE
 typeset    cmd="$1"

 shift

 case "$cmd" in
		'option1') # do something
				rc=$?;;

		'optionN') # do something
				rc=$?;;

		*	 ) 	# Wrong invocation pattern, help user
				display_usage;; 
 esac

 return $rc
}

# -----------------------------------------------------------
# Execute the command
# -----------------------------------------------------------

execute_command()
{
 typeset -i rc=$ERROR_NONE
 typeset    cmd="$1"

 shift

 case "$cmd" in
		'option1') # do something
				rc=$?;;

		'optionN') # do something
				rc=$?;;

		*	 ) 	# Wrong invocation pattern, help user
				display_usage;; 
 esac

 return $rc
}


# -----------------------------------------------------------
#  MAIN function
# -----------------------------------------------------------

main()
{
 typeset -i rc=1	# Assume error

 if [ $# -lt 1 ]; then
      display_usage
 else
      execute_command "$@"
 fi

 rc=$?

 return $rc
}

# Call the main function
main "$@"
exit $?

