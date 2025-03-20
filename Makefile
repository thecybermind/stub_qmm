# STUB_QMM - Example QMM Plugin
# Copyright 2004-2025
# https://github.com/thecybermind/stub_qmm/
# 3-clause BSD license: https://opensource.org/license/bsd-3-clause
# Created By: Kevin Masterson < cybermind@gmail.com >

# STUB_QMM Makefile

include Config.mak

CC=g++

BASE_CFLAGS=-Wall -pipe -m32 -isystem ../qmm_sdks -isystem ../qmm2

BROOT=linux
BR=$(BROOT)/release
BD=$(BROOT)/debug

B_Q3A=_q3a
B_RTCWMP=_rtcwmp
B_JAMP=_jamp
B_JK2MP=_jk2mp
B_RTCWSP=_rtcwsp
B_WET=_wet

OBJR_Q3A=$(SRC_FILES:%.cpp=$(BR)$(B_Q3A)/%.o)
OBJD_Q3A=$(SRC_FILES:%.cpp=$(BD)$(B_Q3A)/%.o)
OBJR_RTCWMP=$(SRC_FILES:%.cpp=$(BR)$(B_RTCWMP)/%.o)
OBJD_RTCWMP=$(SRC_FILES:%.cpp=$(BD)$(B_RTCWMP)/%.o)
OBJR_JAMP=$(SRC_FILES:%.cpp=$(BR)$(B_JAMP)/%.o)
OBJD_JAMP=$(SRC_FILES:%.cpp=$(BD)$(B_JAMP)/%.o)
OBJR_JK2MP=$(SRC_FILES:%.cpp=$(BR)$(B_JK2MP)/%.o)
OBJD_JK2MP=$(SRC_FILES:%.cpp=$(BD)$(B_JK2MP)/%.o)
OBJR_RTCWSP=$(SRC_FILES:%.cpp=$(BR)$(B_RTCWSP)/%.o)
OBJD_RTCWSP=$(SRC_FILES:%.cpp=$(BD)$(B_RTCWSP)/%.o)
OBJR_WET=$(SRC_FILES:%.cpp=$(BR)$(B_WET)/%.o)
OBJD_WET=$(SRC_FILES:%.cpp=$(BD)$(B_WET)/%.o)

DEBUG_CFLAGS=$(BASE_CFLAGS) -g -pg
RELEASE_CFLAGS=$(BASE_CFLAGS) -DNDEBUG -O2 -fomit-frame-pointer -pipe -ffast-math -falign-loops=2 -falign-jumps=2 -falign-functions=2 -fno-strict-aliasing -fstrength-reduce 

SHLIBCFLAGS=-fPIC
SHLIBLDFLAGS=-shared -m32 -fPIC

help:
	@echo STUB_QMM supports the following make rules:
	@echo all - builds release versions for all supported games
	@echo all_d - builds debug versions for all supported games
	@echo q3a - builds release version for Quake 3 Arena
	@echo q3a_d - builds debug version for Quake 3 Arena
	@echo jamp - builds release version for Jedi Knight: Jedi Academy
	@echo jamp_d - builds debug version for Jedi Knight: Jedi Academy
	@echo jk2mp - builds release version for Jedi Knight II: Jedi Outcast
	@echo jk2mp_d - builds debug version for Jedi Knight II: Jedi Outcast
	@echo rtcwmp - builds release version for Return to Castle Wolfenstein \(MP\)
	@echo rtcwmp_d - builds debug version for Return to Castle Wolfenstein \(MP\)
	@echo rtcwsp - builds release version for Return to Castle Wolfenstein \(SP\)
	@echo rtcwsp_d - builds debug version for Return to Castle Wolfenstein \(SP\)
	@echo wet - builds release version for RtCW: Enemy Territory
	@echo wet_d - builds debug version for RtCW: Enemy Territory
	@echo clean - cleans all output files and directories
	
all: q3a rtcwmp jamp jk2mp rtcwsp wet

all_d: q3a_d rtcwmp_d jamp_d jk2mp_d rtcwsp_d wet_d

q3a: $(BR)/$(BINARY).so
q3a_d: $(BD)/$(BINARY).so

rtcwmp: $(BR)$(B_RTCWMP)/$(BINARY).so
rtcwmp_d: $(BD)$(B_RTCWMP)/$(BINARY).so

jamp: $(BR)$(B_JAMP)/$(BINARY).so
jamp_d: $(BD)$(B_JAMP)/$(BINARY).so

jk2mp: $(BR)$(B_JK2MP)/$(BINARY).so
jk2mp_d: $(BD)$(B_JK2MP)/$(BINARY).so

rtcwsp: $(BR)$(B_RTCWSP)/$(BINARY).so
rtcwsp_d: $(BD)$(B_RTCWSP)/$(BINARY).so

wet: $(BR)$(B_WET)/$(BINARY).so
wet_d: $(BD)$(B_WET)/$(BINARY).so

#quake 3 arena
$(BR)$(B_Q3A)/$(BINARY).so: $(BR)$(B_Q3A) $(OBJR_Q3A)
	$(CC) $(RELEASE_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJR_Q3A)
  
$(BD)$(B_Q3A)/$(BINARY).so: $(BD)$(B_Q3A) $(OBJD_Q3A)
	$(CC) $(DEBUG_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJD_Q3A)
  
#return to castle wolfenstein (multiplayer)
$(BR)$(B_RTCWMP)/$(BINARY).so: $(BR)$(B_RTCWMP) $(OBJR_RTCWMP)
	$(CC) $(RELEASE_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJR_RTCWMP)
  
$(BD)$(B_RTCWMP)/$(BINARY).so: $(BD)$(B_RTCWMP) $(OBJD_RTCWMP)
	$(CC) $(DEBUG_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJD_RTCWMP)

#jedi academy
$(BR)$(B_JAMP)/$(BINARY).so: $(BR)$(B_JAMP) $(OBJR_JAMP)
	$(CC) $(RELEASE_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJR_JAMP)
  
$(BD)$(B_JAMP)/$(BINARY).so: $(BD)$(B_JAMP) $(OBJD_JAMP)
	$(CC) $(DEBUG_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJD_JAMP)

#jedi knight 2
$(BR)$(B_JK2MP)/$(BINARY).so: $(BR)$(B_JK2MP) $(OBJR_JK2MP)
	$(CC) $(RELEASE_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJR_JK2MP)
  
$(BD)$(B_JK2MP)/$(BINARY).so: $(BD)$(B_JK2MP) $(OBJD_JK2MP)
	$(CC) $(DEBUG_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJD_JK2MP)

#return to castle wolfenstein (single player)
$(BR)$(B_RTCWSP)/$(BINARY).so: $(BR)$(B_RTCWSP) $(OBJR_RTCWSP)
	$(CC) $(RELEASE_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJR_RTCWSP)
  
$(BD)$(B_RTCWSP)/$(BINARY).so: $(BD)$(B_RTCWSP) $(OBJD_RTCWSP)
	$(CC) $(DEBUG_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJD_RTCWSP)

#wolfenstein: enemy territory
$(BR)$(B_WET)/$(BINARY).so: $(BR)$(B_WET) $(OBJR_WET)
	$(CC) $(RELEASE_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJR_WET)
  
$(BD)$(B_WET)/$(BINARY).so: $(BD)$(B_WET) $(OBJD_WET)
	$(CC) $(DEBUG_CFLAGS) $(SHLIBLDFLAGS) -o $@ $(OBJD_WET)


$(BR)$(B_Q3A)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(RELEASE_CFLAGS) $(Q3A_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<
  
$(BD)$(B_Q3A)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(DEBUG_CFLAGS) $(Q3A_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<

$(BR)$(B_RTCWMP)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(RELEASE_CFLAGS) $(RTCWMP_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<
  
$(BD)$(B_RTCWMP)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(DEBUG_CFLAGS) $(RTCWMP_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<

$(BR)$(B_JAMP)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(RELEASE_CFLAGS) $(JAMP_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<
  
$(BD)$(B_JAMP)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(DEBUG_CFLAGS) $(JAMP_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<

$(BR)$(B_JK2MP)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(RELEASE_CFLAGS) $(JK2MP_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<
  
$(BD)$(B_JK2MP)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(DEBUG_CFLAGS) $(JK2MP_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<

$(BR)$(B_RTCWSP)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(RELEASE_CFLAGS) $(RTCWSP_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<
  
$(BD)$(B_RTCWSP)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(DEBUG_CFLAGS) $(RTCWSP_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<

$(BR)$(B_WET)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(RELEASE_CFLAGS) $(WET_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<
  
$(BD)$(B_WET)/%.o: %.cpp $(HDR_FILES)
	$(CC) $(DEBUG_CFLAGS) $(WET_FLAGS) $(SHLIBCFLAGS) -o $@ -c $<

$(BR)$(B_Q3A):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi

$(BD)$(B_Q3A):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi
	
$(BR)$(B_RTCWMP):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi

$(BD)$(B_RTCWMP):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi
	
$(BR)$(B_JAMP):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi

$(BD)$(B_JAMP):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi

$(BR)$(B_JK2MP):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi

$(BD)$(B_JK2MP):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi

$(BR)$(B_RTCWSP):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi

$(BD)$(B_RTCWSP):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi
	
$(BR)$(B_WET):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi

$(BD)$(B_WET):
	@if [ ! -d $(BROOT) ];then mkdir $(BROOT);fi
	@if [ ! -d $(@) ];then mkdir $@;fi
	
clean:
	@rm -rf $(BD)$(B_Q3A) $(BR)$(B_Q3A) $(BD)$(B_RTCWMP) $(BR)$(B_RTCWMP) $(BD)$(B_JAMP) $(BRA)$(B_JAMP) $(BD)$(B_JK2MP) $(BR)$(B_JK2MP) $(BD)$(B_RTCWSP) $(BR)$(B_RTCWSP) $(BD)$(B_WET) $(BR)$(B_WET)
