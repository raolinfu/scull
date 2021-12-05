#!/bin/sh
# $Id: scull_load,v 1.4 2004/11/03 06:19:49 rubini Exp $
module="raolinhu"
device="raolinhu"
mode="664"

# Group: since distributions do it differently, look for wheel or use staff
if grep -q '^staff:' /etc/group; then
    group="staff"
else
    group="wheel"
fi

# insmod ./$module.ko $* || exit 1

# retrieve major number
awk "\$2==\"raolinhu\" {print \$1}" /proc/devices
major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)

# rm -f /dev/${device}[0-3]
# mknod /dev/${device}0 c $major 0
# mknod /dev/${device}1 c $major 1
# mknod /dev/${device}2 c $major 2
# mknod /dev/${device}3 c $major 3
# ln -sf ${device}0 /dev/${device}
# chgrp $group /dev/${device}[0-3]
# chmod $mode  /dev/${device}[0-3]
