platform_check_image() {
        return 0
}

platform_do_upgrade() {
        CI_UBIPART="system_sw"
        CI_ROOTPART="rootfsA"
        CI_KERNPART="kernelA"

        nand_upgrade_tar "$1"
}

