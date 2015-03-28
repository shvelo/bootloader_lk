# top level project rules for the msm8226 project
#
LOCAL_DIR := $(GET_LOCAL_DIR)

TARGET := msm8226

MODULES += app/aboot

ifeq ($(TARGET_BUILD_VARIANT),user)
DEBUG := 0
else
DEBUG := 1
endif

EMMC_BOOT := 1
ENABLE_SDHCI_SUPPORT := 1

#enable power on vibrator feature
ENABLE_PON_VIB_SUPPORT := true

#DEFINES += WITH_DEBUG_DCC=1
DEFINES += WITH_DEBUG_UART=1
DEFINES += WITH_DEBUG_LOG_BUF=1
#DEFINES += WITH_DEBUG_FBCON=1
DEFINES += DEVICE_TREE=1
#DEFINES += MMC_BOOT_BAM=1
DEFINES += CRYPTO_BAM=1
DEFINES += ABOOT_IGNORE_BOOT_HEADER_ADDRS=1

DEFINES += ABOOT_FORCE_KERNEL_ADDR=0x00008000
DEFINES += ABOOT_FORCE_RAMDISK_ADDR=0x08800000
DEFINES += ABOOT_FORCE_TAGS_ADDR=0x08600000
DEFINES += IMAGE_VERIF_ALGO_SHA1=0
DEFINES += ABOOT_FORCE_KERNEL64_ADDR=0x00080000

#Disable thumb mode
ENABLE_THUMB := false

ifeq ($(ENABLE_PON_VIB_SUPPORT),true)
DEFINES += PON_VIB_SUPPORT=1
endif

ifeq ($(ENABLE_SDHCI_SUPPORT),1)
DEFINES += MMC_SDHCI_SUPPORT=1
endif
