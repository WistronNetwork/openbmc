SRCREV = "f763cd2e39ffce9b10191402243e8704794f08ff"

DEPENDS:remove:wistron-net = "nlf-native "

# Workaround
# Network access from task are disabled by default on Yocto 3.5
# https://git.yoctoproject.org/poky/tree/documentation/migration-guides/migration-3.5.rst#n25
do_compile[network] = "1"

do_find_node_licenses[noexec] = "1"
