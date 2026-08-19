## Description:
 
This is repository of profiles for all Maxlinear supported hardware platforms. 

Select the appropriate profile to build images for any supported Maxlinear platform, as described in the QuickStart steps below.
 
## QuickStart:
 
Below are sample steps to build the Mxl UPDK Software for UPDK-9.1.95 Release.
 
Complete these steps to create the basic software build for the OSPv1,OSPv2,URX851‑HDK‑2, MxL25641‑HDK‑2, with OpenWrt 23.05:
 
#### 1. Clone prplOS repository from prpl Foundation:

git clone -b prplware-v4.0.1 https://gitlab.com/prpl-foundation/prplos/prplos

#### 2. Select the configuration to build and synchronize files for building the UPDK Software:

cd prplos

git checkout 8b82aba028174269755b82fdbc03ae77cd40265c

#### 3. Clone the prplos_mxl_profiles repository to obtain the required profile for any platforms.

git clone -b updk_9.1.100 git@github.com:maxlinear/mxl_prplos_profiles.git

cp mxl_prplos_profiles/profiles/* profiles/

#### 4. Run the command which corresponds to the model:

a) OSPv2

./scripts/gen_config.py mxl_x86_osp_tb341_v2 mxl_wlan_hostap_ng_wav700 prpl

b) MxL25641‑HDK‑2 and URX851‑HDK‑2

./scripts/gen_config.py mxl_x86_mb_urx.yml mxl_wlan_hostap_ng_wav700 prpl

#### 5. Type below command to start the build
make -j8

#### Note: Built images are located in the prplos/bin/targets/intel_x86/lgm/ folder.
#### The U-Boot images are located in the prplos/bin/targets/intel_x86/lgm/uboot-intel-x86 folder.
 
#### For more details please refer to the prpl foundation link below.
 
https://gitlab.com/prpl-foundation/prplos/prplos/-/wikis/Maxlinear-Open-Service-Platform/OSPv2
## Image Details:

#### OSPV2 Image details:

prplos-intel_x86-lgm-PRPL_OSP_TB341_v2-osp_tb341_v2_wav700_eth_fullimage.img  **->** **Fullimage for ETH WAN.**

prplos-intel_x86-lgm-PRPL_OSP_TB341_v2-osp_tb341_v2_wav700_pon_fullimage.img  **->** **Fullimage for PON WAN.**

prplos-intel_x86-lgm-PRPL_OSP_TB341_v2-osp_tb341_v2_wav700_eth.dtb 	          **->** **Devicetree for ETH WAN.**

prplos-intel_x86-lgm-PRPL_OSP_TB341_v2-osp_tb341_v2_wav700_pon.dtb 	          **->** **Devicetree for PON WAN.**

ext4.fs 								                                                              **->** **ext4 file system.**

u-boot-plus-spl-emmc.bin 						                                              **->** **U-Boot binary file.**

u-boot-recovery.asc 							                                                   **->** **U-Boot recovery file**

#### MxL25641‑HDK‑2 Image details:

prplos-intel_x86-lgm-PRPL_MB_URX641_wav700_eth_fullimage.img  **->** **Fullimage for ETH WAN.**

prplos-intel_x86-lgm-PRPL_MB_URX641_wav700_pon_fullimage.img  **->** **Fullimage for PON WAN.**

prplos-intel_x86-lgm-PRPL_MB_URX-641.dtb 	                    **->** **Devicetree for ETH WAN.**

prplos-intel_x86-lgm-PRPL_MB_URX641_pon.dtb	                  **->** **Devicetree for PON WAN.**

ext4.fs 								                                              **->** **ext4 file system.**

u-boot-plus-spl-emmc.bin 						                               **->** **U-Boot binary file.**

u-boot-recovery.asc 							                                   **->** **U-Boot recovery file**

