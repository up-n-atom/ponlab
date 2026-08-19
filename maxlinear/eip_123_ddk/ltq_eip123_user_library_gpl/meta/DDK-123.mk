libcal.a: Build/build_linux
	cd $< && $(MAKE) $(AM_MAKEFLAGS) libcal.a
	cp $</libcal.a $@





















