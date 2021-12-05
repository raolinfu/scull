#!/bin/sh
module="helloworld"
device="raolinhu"

# invoke rmmod with all arguments we got
rmmod $module $* || exit 1

# Remove stale nodes

rm -f /dev/${device} /dev/${device}[0-3] 


