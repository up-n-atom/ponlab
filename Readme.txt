README:


This CD includes TR-143 based speed test implementation.
Below are the two components -
Kernel module name : speedtest.ko
Application name   : tr143diagtool

Preparing UGW CD
---------------
    1. Unzip the CD:
        unzip UGW-x.x.x.x-SW-CD.zip

    2. Enter into UGW-x.x.x.x-SW-CD/ folder
        cd UGW-x.x.x.x-SW-CD/

    3. Extract the installer:
        ./install.sh
       Read the license agreement. Move text down with 'enter' or 'spacebar' key and Accept the license agreement by typing "yes".
       The installer extracts 'ugw_sw' folder with sources.

    4. Enter into source:
        cd ugw_sw/

    5. Prepare the source:
        ./ugw-prepare-all.sh
       This scripts prepares UGW CD. This step needs active internet connection as it downloads various packages from Internet.
       The step generates 'openwrt' folder which will be the toplevel for builds.

Compiling Sources
-----------------
    1. Enter in openwrt/ folder.
        cd openwrt/

    2. Select a model by:-
        ./scripts/ltq_change_environment.sh switch
       Choose AX6000_2000_ETH_11AX_SEC model from the list. Keyin the 08-index number and press enter.

	3. Enable the kmod-speedtest-driver and tr143diagtool for AXEPOINT model :
		a) with dns_resolver support in kernel:
			~#make menuconfig
			i)	 Enable kmod-dnsresolver (Kernel modules -> Network Support -> kmod-dnsresolver).
			ii)  Enable kmod-speedtest-driver (Kernel modules -> MaxLinear -> kmod-speedtest-driver).
					kmod-speedtest-driver enable keyutils. keyutils package provides request-key and key.dns_resolver binaries required for dns_resolver.
			iii) Enable tr143diagtool (MaxLinear -> UGW Framework -> tr143diagtool)
		b) without dns_resolver support in kernel:
			~#make menuconfig
			i)	Enable kmod-speedtest-driver (Kernel modules -> MaxLinear -> kmod-speedtest-driver)
			ii) Enable tr143diagtool (MaxLinear -> UGW Framework -> tr143diagtool)

	4. Build the image
		$make -j24

Steps to run the speed test with tr143diagtool
----------------------------------------------
	1. In DUT flash the image and load module:
		a) While flashing image disable the watchdog from uboot prompt
			~#set custom 'no_watchdog=1'
		b) Disable the prplmesh after booting
			~#/etc/init.d/prplmesh stop
		c) Load the speedtest module using
			~#insmod speedtest

	2. Take help of tr143daigtool help to configure and run the speed test:
		$tr143diagtool -h or tr143diagtool --help

	3. Configure TR143 speedtest parameters:
		tr143diagtool config support below parameter.
		a) -t : this is for test type which user want to perform.
				for download it should be 1 and for upload it should be 0.
		b) -c : number of connection it should be less then max_conn proc value.
		c) -m : this option is for transfer mode if it is file based transfer 0 and if time based 1.
		d) -v : this is the value of transfer mode if mode is time based tarnsfer then value should be in seconds and
				if mode is file based transfer mode then value should be in B, KB, GB.
		e) -s : this is for server ip. if url is given no need to give server ip.
		f) -p : this is for server port. if url is given not needed.
		g) -u : this option is for url. if server ip and server port is given then no need to give url.
		h) -w : WAN interface if not given default interface will be taken.
		g) -i : WAN ip address if not given default route will be taken.
		sample command =>
		~#tr143diagtool -C -t 0 -c 1 -m 0 -v 100GB -u http://www.speedtest.com/index.html
		~#tr143diagtool -C -t 1 -c 1 -m 0 -v 100GB -u http://192.168.1.22/index.html
		~#tr143diagtool -C -t 0 -c 1 -m 1 -v 30sec -u http://10.10.200.2/index.html
		~#tr143diagtool -C -t 1 -c 1 -m 1 -v 30sec -u http://www.speedtest.com/index.html
		~#tr143diagtool -C -t 0 -c 1 -m 1 -v 30sec -s 200.215.51.1 -p 80

	4. Start the speed test:
		a) Start without notify:
			~#tr143diagtool -S or ~#tr143diagtool --start
			it will start the test and if want to check the BOM and EOM time use notify.
		b) Start with notify :
			it will start the test and it is a blocking call so it will block until BOM or EOM time
			if -b and -e option called respectivly.
			~#tr143diagtool -S -b (notify BOM time)
			~#tr143diagtool -S -e (notify EOM time)

	5. Notify the BOM time or EOM time :
		User can check the BOM time and EOM time using below command.
		make sure notify is called after start and before result call.
		For BOM time:
			~#tr143diagtool -N -b
		For EOM time:
			~#tr143diagtool -N -e

	6. Fetch the result:
		User can check all parameters like BOMTime, EOMTime, speedtest throughput etc. using result command.
		~#tr143diagtool -R

	7. Abort the speed test:
		Using abort call user can stop the test whenever it is needed.
		~#tr143diagtool -A

Speed test kernel module configuration parameters
-------------------------------------------------
A proc file is provided to configure the kernel module

/proc/tr143diagtool/<config_param>
where <config_param> equals one of the below
	1. max_duration:
		This parameter used to limit the duration to run the tool.
		To config use -> ~#echo 100 > /proc/tr143diagtool/max_duration.
		To check max_duration value -> ~#cat /proc/tr143diagtool/max_duration.

	2. max_size:
		This parameter used to limit the file size using for upload and download.
		To config use -> ~#echo 10737418240 > /proc/tr143diagtool/max_size(value should be in bytes).
		To check max_size value -> ~#cat /proc/tr143diagtool/max_size.

	3. max_conn:
		This parameter used to limit the number of connection.
		To config use -> ~#echo 10 > /proc/tr143diagtool/max_conn.
		To check max_conn value -> ~#cat /proc/tr143diagtool/max_conn.

	4. page_order:
		Page order is to specify the page size.
		Page size will be (2^page_order * PAGE_SIZE).
		Default page order is 1.

	5. buffer_size:
		To check buffer_size -> ~#cat /proc/tr143diagtool/buffer_size.

To configure the proc entry:
	~#echo <param_value> > /proc/tr143diagtool/<proc_entry>
	sample command =>
	~#echo 100 > /proc/tr143diagtool/max_duration
