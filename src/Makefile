#
# OMNeT++/OMNEST Makefile for BusModel
#
# This file was generated with the command:
#  opp_makemake -f --deep
#

# Name of target to be created (-o option)
TARGET = BusModel$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -Iapplications \
    -Iapplications/can \
    -Iapplications/can/sink \
    -Iapplications/can/source \
    -Iapplications/flexray \
    -Ibuffer \
    -Ibuffer/can \
    -Ibus \
    -Ibus/can \
    -Ibus/flexray \
    -Ilinklayer \
    -Ilinklayer/can \
    -Ilinklayer/can/messages \
    -Ilinklayer/flexray \
    -Inodes \
    -Inodes/can \
    -Inodes/flexray \
    -Ischeduler \
    -Ischeduler/flexray \
    -Isynchronisation \
    -Isynchronisation/flexray

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/applications/can/sink/CanTrafficSinkAppBase.o \
    $O/applications/can/source/CanTrafficSourceAppBase.o \
    $O/applications/flexray/FRApp.o \
    $O/buffer/can/Buffer.o \
    $O/buffer/can/OutputBuffer.o \
    $O/buffer/can/InputBuffer.o \
    $O/bus/BusPort.o \
    $O/bus/can/CanID.o \
    $O/bus/can/CanBusApp.o \
    $O/bus/flexray/FRTopologyPort.o \
    $O/linklayer/NodePort.o \
    $O/linklayer/can/CanPortInput.o \
    $O/linklayer/can/CanPortOutput.o \
    $O/linklayer/flexray/FRPort.o \
    $O/nodes/can/CanNodeApp.o \
    $O/nodes/can/Stats.o \
    $O/nodes/can/MOB.o \
    $O/scheduler/flexray/FRScheduler.o \
    $O/scheduler/flexray/SchedulerEvent.o \
    $O/synchronisation/flexray/FRSync.o \
    $O/linklayer/can/messages/arb_m.o \
    $O/linklayer/can/messages/err_m.o \
    $O/linklayer/can/messages/dataframe_m.o \
    $O/linklayer/can/messages/ack_m.o \
    $O/linklayer/can/messages/candataframe_m.o \
    $O/linklayer/flexray/FRFrame_m.o \
    $O/scheduler/flexray/SchedulerMessage_m.o \
    $O/scheduler/flexray/SchedulerMessageEvents_m.o

# Message files
MSGFILES = \
    linklayer/can/messages/arb.msg \
    linklayer/can/messages/err.msg \
    linklayer/can/messages/dataframe.msg \
    linklayer/can/messages/ack.msg \
    linklayer/can/messages/candataframe.msg \
    linklayer/flexray/FRFrame.msg \
    scheduler/flexray/SchedulerMessage.msg \
    scheduler/flexray/SchedulerMessageEvents.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f BusModel BusModel.exe libBusModel.so libBusModel.a libBusModel.dll libBusModel.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f applications/*_m.cc applications/*_m.h
	$(Q)-rm -f applications/can/*_m.cc applications/can/*_m.h
	$(Q)-rm -f applications/can/sink/*_m.cc applications/can/sink/*_m.h
	$(Q)-rm -f applications/can/source/*_m.cc applications/can/source/*_m.h
	$(Q)-rm -f applications/flexray/*_m.cc applications/flexray/*_m.h
	$(Q)-rm -f buffer/*_m.cc buffer/*_m.h
	$(Q)-rm -f buffer/can/*_m.cc buffer/can/*_m.h
	$(Q)-rm -f bus/*_m.cc bus/*_m.h
	$(Q)-rm -f bus/can/*_m.cc bus/can/*_m.h
	$(Q)-rm -f bus/flexray/*_m.cc bus/flexray/*_m.h
	$(Q)-rm -f linklayer/*_m.cc linklayer/*_m.h
	$(Q)-rm -f linklayer/can/*_m.cc linklayer/can/*_m.h
	$(Q)-rm -f linklayer/can/messages/*_m.cc linklayer/can/messages/*_m.h
	$(Q)-rm -f linklayer/flexray/*_m.cc linklayer/flexray/*_m.h
	$(Q)-rm -f nodes/*_m.cc nodes/*_m.h
	$(Q)-rm -f nodes/can/*_m.cc nodes/can/*_m.h
	$(Q)-rm -f nodes/flexray/*_m.cc nodes/flexray/*_m.h
	$(Q)-rm -f scheduler/*_m.cc scheduler/*_m.h
	$(Q)-rm -f scheduler/flexray/*_m.cc scheduler/flexray/*_m.h
	$(Q)-rm -f synchronisation/*_m.cc synchronisation/*_m.h
	$(Q)-rm -f synchronisation/flexray/*_m.cc synchronisation/flexray/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc applications/*.cc applications/can/*.cc applications/can/sink/*.cc applications/can/source/*.cc applications/flexray/*.cc buffer/*.cc buffer/can/*.cc bus/*.cc bus/can/*.cc bus/flexray/*.cc linklayer/*.cc linklayer/can/*.cc linklayer/can/messages/*.cc linklayer/flexray/*.cc nodes/*.cc nodes/can/*.cc nodes/flexray/*.cc scheduler/*.cc scheduler/flexray/*.cc synchronisation/*.cc synchronisation/flexray/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/applications/can/sink/CanTrafficSinkAppBase.o: applications/can/sink/CanTrafficSinkAppBase.cc \
	applications/can/sink/CanTrafficSinkAppBase.h \
	buffer/can/Buffer.h \
	buffer/can/InputBuffer.h \
	linklayer/can/messages/candataframe_m.h
$O/applications/can/source/CanTrafficSourceAppBase.o: applications/can/source/CanTrafficSourceAppBase.cc \
	applications/can/source/CanTrafficSourceAppBase.h \
	buffer/can/Buffer.h \
	linklayer/can/messages/candataframe_m.h
$O/applications/flexray/FRApp.o: applications/flexray/FRApp.cc \
	applications/flexray/FRApp.h \
	linklayer/flexray/FRFrame_m.h \
	linklayer/flexray/FRPort.h \
	scheduler/flexray/FRScheduler.h \
	scheduler/flexray/SchedulerEvent.h \
	scheduler/flexray/SchedulerMessageEvents_m.h \
	scheduler/flexray/SchedulerMessage_m.h \
	synchronisation/flexray/FRSync.h
$O/buffer/can/Buffer.o: buffer/can/Buffer.cc \
	buffer/can/Buffer.h \
	linklayer/can/messages/candataframe_m.h
$O/buffer/can/InputBuffer.o: buffer/can/InputBuffer.cc \
	buffer/can/Buffer.h \
	buffer/can/InputBuffer.h \
	linklayer/can/messages/candataframe_m.h
$O/buffer/can/OutputBuffer.o: buffer/can/OutputBuffer.cc \
	applications/can/source/CanTrafficSourceAppBase.h \
	buffer/can/Buffer.h \
	buffer/can/OutputBuffer.h \
	bus/BusPort.h \
	bus/can/CanBusApp.h \
	bus/can/CanID.h \
	linklayer/can/messages/ack_m.h \
	linklayer/can/messages/arb_m.h \
	linklayer/can/messages/candataframe_m.h \
	linklayer/can/messages/err_m.h
$O/bus/BusPort.o: bus/BusPort.cc \
	applications/can/source/CanTrafficSourceAppBase.h \
	buffer/can/Buffer.h \
	buffer/can/OutputBuffer.h \
	bus/BusPort.h \
	bus/can/CanBusApp.h \
	bus/can/CanID.h \
	linklayer/can/messages/ack_m.h \
	linklayer/can/messages/arb_m.h \
	linklayer/can/messages/candataframe_m.h \
	linklayer/can/messages/err_m.h
$O/bus/can/CanBusApp.o: bus/can/CanBusApp.cc \
	applications/can/source/CanTrafficSourceAppBase.h \
	buffer/can/Buffer.h \
	buffer/can/OutputBuffer.h \
	bus/BusPort.h \
	bus/can/CanBusApp.h \
	bus/can/CanID.h \
	linklayer/can/messages/ack_m.h \
	linklayer/can/messages/arb_m.h \
	linklayer/can/messages/candataframe_m.h \
	linklayer/can/messages/err_m.h
$O/bus/can/CanID.o: bus/can/CanID.cc \
	bus/can/CanID.h
$O/bus/flexray/FRTopologyPort.o: bus/flexray/FRTopologyPort.cc \
	bus/flexray/FRTopologyPort.h
$O/linklayer/NodePort.o: linklayer/NodePort.cc \
	linklayer/NodePort.h \
	nodes/can/CanNodeApp.h
$O/linklayer/can/CanPortInput.o: linklayer/can/CanPortInput.cc \
	linklayer/can/CanPortInput.h \
	linklayer/can/CanPortOutput.h \
	linklayer/can/messages/candataframe_m.h \
	linklayer/can/messages/err_m.h
$O/linklayer/can/CanPortOutput.o: linklayer/can/CanPortOutput.cc \
	linklayer/can/CanPortOutput.h \
	linklayer/can/messages/candataframe_m.h \
	linklayer/can/messages/err_m.h
$O/linklayer/can/messages/ack_m.o: linklayer/can/messages/ack_m.cc \
	linklayer/can/messages/ack_m.h
$O/linklayer/can/messages/arb_m.o: linklayer/can/messages/arb_m.cc \
	linklayer/can/messages/arb_m.h
$O/linklayer/can/messages/candataframe_m.o: linklayer/can/messages/candataframe_m.cc \
	linklayer/can/messages/candataframe_m.h
$O/linklayer/can/messages/dataframe_m.o: linklayer/can/messages/dataframe_m.cc \
	linklayer/can/messages/dataframe_m.h
$O/linklayer/can/messages/err_m.o: linklayer/can/messages/err_m.cc \
	linklayer/can/messages/err_m.h
$O/linklayer/flexray/FRFrame_m.o: linklayer/flexray/FRFrame_m.cc \
	linklayer/flexray/FRFrame_m.h
$O/linklayer/flexray/FRPort.o: linklayer/flexray/FRPort.cc \
	applications/flexray/FRApp.h \
	linklayer/flexray/FRFrame_m.h \
	linklayer/flexray/FRPort.h \
	scheduler/flexray/FRScheduler.h \
	scheduler/flexray/SchedulerEvent.h \
	scheduler/flexray/SchedulerMessageEvents_m.h \
	scheduler/flexray/SchedulerMessage_m.h \
	synchronisation/flexray/FRSync.h
$O/nodes/can/CanNodeApp.o: nodes/can/CanNodeApp.cc
$O/nodes/can/MOB.o: nodes/can/MOB.cc \
	nodes/can/MOB.h
$O/nodes/can/Stats.o: nodes/can/Stats.cc \
	nodes/can/Stats.h
$O/scheduler/flexray/FRScheduler.o: scheduler/flexray/FRScheduler.cc \
	applications/flexray/FRApp.h \
	linklayer/flexray/FRFrame_m.h \
	linklayer/flexray/FRPort.h \
	scheduler/flexray/FRScheduler.h \
	scheduler/flexray/SchedulerEvent.h \
	scheduler/flexray/SchedulerMessageEvents_m.h \
	scheduler/flexray/SchedulerMessage_m.h \
	synchronisation/flexray/FRSync.h
$O/scheduler/flexray/SchedulerEvent.o: scheduler/flexray/SchedulerEvent.cc \
	scheduler/flexray/SchedulerEvent.h \
	scheduler/flexray/SchedulerMessage_m.h
$O/scheduler/flexray/SchedulerMessageEvents_m.o: scheduler/flexray/SchedulerMessageEvents_m.cc \
	scheduler/flexray/SchedulerEvent.h \
	scheduler/flexray/SchedulerMessageEvents_m.h \
	scheduler/flexray/SchedulerMessage_m.h
$O/scheduler/flexray/SchedulerMessage_m.o: scheduler/flexray/SchedulerMessage_m.cc \
	scheduler/flexray/SchedulerMessage_m.h
$O/synchronisation/flexray/FRSync.o: synchronisation/flexray/FRSync.cc \
	applications/flexray/FRApp.h \
	linklayer/flexray/FRFrame_m.h \
	linklayer/flexray/FRPort.h \
	scheduler/flexray/FRScheduler.h \
	scheduler/flexray/SchedulerEvent.h \
	scheduler/flexray/SchedulerMessageEvents_m.h \
	scheduler/flexray/SchedulerMessage_m.h \
	synchronisation/flexray/FRSync.h

