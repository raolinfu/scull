#!/bin/sh

# 模块名和设备名
module="helloworld"
device="raolinhu"
mode="664"

if grep -q '^staff:' /etc/group; then
    group="staff"
else
    group="wheel"
fi

insmod $module.ko $* || exit 1

# retrieve major number
# awk "\$2==\"raolinhu\" {print \$1}" /proc/devices
major=$(awk "\$2==\"$device\" {print \$1}" /proc/devices)
echo major:$major

rm -f /dev/${device}[0-3]
# 现在只有一个设备
mknod /dev/${device}0 c $major 0

# mknod /dev/${device}1 c $major 1
# 创建符号链接
# ln -sf ${device}0 /dev/${device}
# 设置组权限 暂时不设置
# chgrp $group /dev/${device}[0-3]

# 更改权限
chmod $mode  /dev/${device}[0-3]
