all: checkmakefiles src/fico4omnet/features.h 
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile src/fico4omnet/features.h

MAKEMAKE_OPTIONS := -f --deep -o FiCo4OMNeT -O out -pFiCo4OMNeT 

makefiles: src/fico4omnet/features.h makefiles-so

makefiles-so:
	@FEATURE_OPTIONS=$$(./fico4omnet_featuretool options -f -l) && cd src && opp_makemake --make-so $(MAKEMAKE_OPTIONS) $$FEATURE_OPTIONS

makefiles-lib:
	@FEATURE_OPTIONS=$$(./fico4omnet_featuretool options -f -l) && cd src && opp_makemake --make-lib $(MAKEMAKE_OPTIONS) $$FEATURE_OPTIONS

makefiles-exe:
	@FEATURE_OPTIONS=$$(./fico4omnet_featuretool options -f -l) && cd src && opp_makemake $(MAKEMAKE_OPTIONS) $$FEATURE_OPTIONS

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '======================================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '======================================================================='; \
	echo; \
	exit 1; \
	fi

# generate an include file that contains all the WITH_FEATURE macros according to the current enablement of features
src/fico4omnet/features.h: $(wildcard .oppfeaturestate) .oppfeatures
	@./fico4omnet_featuretool defines >src/fico4omnet/features.h

doxy:
	doxygen doxy.cfg