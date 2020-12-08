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

# Ensure that system locations are first searched, for security reasons,
# when locating a command referenced in this script

PATH=/bin:/usr/bin:/sbin:/usr/sbin:$PATH


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


# Logging 
SYSLOG_DEFAULT_SEVERITY="INFO"
SYSLOG_DEFAULT_FACILITY="USER"
SYSLOG_DEFAULT_TAG="INFO"
SYSLOG_DEFAULT_LOGGER="logger"



# -----------------------------------------------------------
#
#
# Functions
#
#
# -----------------------------------------------------------



# -----------------------------------------------------------
#
# Purpose: Send information to the system log
#
# Input:
#        1 - Message to log
#        2 - Optional severity (man syslog)
#
# Note: The following environment variables change the behaviour
#       of this shell function:
#       SYSLOG_SEVERITY - Redefines the default severity
#       SYSLOG_FACILITY - Redefines the default facility
#       SYSLOG_TAG      - Redefines the default log tag
#       SYSLOG_LOGGER   - Redefines the default logger tool
#       
# -----------------------------------------------------------

syslog()
{
 typeset message="$1"
 typeset severity="$2"
 typeset tag=""

 if [ -z "$message" ]; then
      return  # No message, no action
 fi

 # If no severity is associated with the message, use a default one
 if [ -z "$severity" ]; then
      severity=${SYSLOG_SEVERITY:-$SYSLOG_DEFAULT_SEVERITY}
 fi
 
 # If a tag is defined, use it 
 if [ -n "$SYSLOG_TAG" ]; then
      tag="-t $SYSLOG_TAG"
 fi

 ${SYSLOG_LOGGER:-$SYSLOG_DEFAULT_LOGGER} -i "$tag" -p ${SYSLOG_FACILITY:-$SYSLOG_DEFAULT_FACILITY}.${severity} "$message"
}


