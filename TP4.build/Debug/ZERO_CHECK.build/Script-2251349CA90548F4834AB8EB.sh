#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/apple/Desktop/PRALG/TP4
  make -f /Users/apple/Desktop/PRALG/TP4/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/apple/Desktop/PRALG/TP4
  make -f /Users/apple/Desktop/PRALG/TP4/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/apple/Desktop/PRALG/TP4
  make -f /Users/apple/Desktop/PRALG/TP4/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/apple/Desktop/PRALG/TP4
  make -f /Users/apple/Desktop/PRALG/TP4/CMakeScripts/ReRunCMake.make
fi

