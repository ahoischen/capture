#---------------------------------------------------------------------------------
# The configuration is a little bit further down; a few functions have to be imported
# first.
#---------------------------------------------------------------------------------
MAKEMODULES_DIR	:= make

# Utility functions.
include $(MAKEMODULES_DIR)/Utility

# Asserts the existence of crucial variables such as DEVKITARM.
include $(MAKEMODULES_DIR)/AssertEnvironment

# Asserts that this makefile is being passed all required variables.
#include $(MAKEMODULES_DIR)/AssertParameters

#---------------------------------------------------------------------------------
# REGULAR CONFIGURATION
# All these values are defined to be either a possibly exported value or the
# second parameter.
#- VARIABLE_NAME ----- IMMEDIATE ?= ----- VARIABLE_NAME ------- VALUE ------------
BUILD_TARGET		:= $(call or_default, BUILD_TARGET, 		build)

BUILD_OUTPUT		:= $(call or_default, BUILD_OUTPUT, 		$(notdir $(CURDIR)))

BUILD_SOURCES		:= $(call or_default, BUILD_SOURCES, 		$(shell find source -type d -print))

BUILD_INCLUDES		:= $(call or_default, BUILD_INCLUDES, 		include)

BUILD_DATA			:= $(call or_default, BUILD_DATA, 			data)

BUILD_PRODUCTCODE	:= $(call or_default, BUILD_PRODUCTCODE,	CTR-P-CTAP)
BUILD_ROMFS_DIR		:= $(call or_default, BUILD_ROMFS_DIR,		romfs)
BUILD_UNIQUEID		:= $(call or_default, BUILD_UNIQUEID, 		6d40a6)

BUILD_ARCH			:= $(call or_default, BUILD_ARCH, 			-march-armv6k \
																-mtune=mpcore \
																-mfloat-abi=hard \
																-mtp=soft)

BUILD_CFLAGS		:= $(call or_default, BUILD_CFLAGS,			-g -Wall -O2 -mword-relocations \
																-fomit-frame-pointer -ffunction-sections) \
																$(BUILD_ARCH)

BUILD_CXXFLAGS		:= $(call or_default, BUILD_CXXFLAGS,		$(BUILD_CFLAGS) \
																-fno-rtti \
																-fno-exceptions \
																-std=gnu++11)
BUILD_ASFLAGS		:= $(call or_default, BUILD_ASFLAGS,		-g $(BUILD_ARCH))
BUILD_LDFLAGS		?= -specs=3dsx.specs -g $(BUILD_ARCH) -Wl,Map,$(notdir $*.map)

BUILD_LIBS			:= $(call or_default, BUILD_LIBS,			-lctru -lm)
BUILD_LIBDIRS		:= $(call or_default, BUILD_LIBDIRS,		$(CTRULIB))

# BUILD_NO_3DSX		:= 1
# BUILD_NO_SMDH		:= 1
# BUILD_NO_CIA		:= 1
# BUILD_NOT_CCI		:= 1

#---------------------------------------------------------------------------------
# TEST CONFIGURATION
# These parameters apply to the TEST_TARGET only.
# SOURCES, INCLUDES, DATA and ROMFS specify additional directories to be
# included. ROMFS merges directories in the BUILD directory, overwriting files
# from the regular builds with those in TEST_ROMFS.
# PRODUCTCODE and UNIQUEID replace the values for normal builds.
# *FLAGS gets overwritten. LIBDS and LIBDIRS are appended to their BUILD_
# counterparts.
#- VARIABLE_NAME ----- IMMEDIATE ?= ----- VARIABLE_NAME ------- VALUE ------------
TEST_TARGET			:= $(call or_default, TEST_TARGET, 			test)

TEST_OUTPUT			:= $(call or_default, TEST_OUTPUT,	 		$(notdir $(CURDIR)))_test

TEST_SOURCES		:= $(call or_default, TEST_SOURCES, 		test \
																$(sort $(wildcard test/**/)))

TEST_INCLUDES		:= $(call or_default, TEST_INCLUDES, 		Catch/single_include \
																cpptoml/include)

TEST_DATA			:= $(call or_default, TEST_DATA, 			test-data)

TEST_PRODUCTCODE	:= $(call or_default, TEST_PRODUCTCODE, 	CTR-P-CTAP)
TEST_ROMFS_DIR		:= $(call or_default, TEST_ROMFS_DIR,		romfs)
TEST_UNIQUEID		:= $(call or_default, TEST_UNIQUEID, 		6b40a6)

TEST_ARCH			:= $(call or_default, TEST_ARCH, 			-march-armv6k \
																-mtune=mpcore \
																-mfloat-abi=hard \
																-mtp=soft)

TEST_CFLAGS		:= $(call or_default, TEST_CFLAGS,				-g -Wall -O2 -mword-relocations \
																-fomit-frame-pointer -ffunction-sections) \
																$(TEST_ARCH)

TEST_CXXFLAGS		:= $(call or_default, TEST_CXXFLAGS,		$(TEST_CFLAGS) \
																-fno-rtti \
																-std=gnu++17)
TEST_ASFLAGS		:= $(call or_default, TEST_ASFLAGS,			-g $(TEST_ARCH))
TEST_LDFLAGS		?= -specs=3dsx.specs -g $(TEST_ARCH) -Wl,Map,$(notdir $*.map)

TEST_LIBS			:= $(call or_default, TEST_LIBS,			)
TEST_LIBDIRS		:= $(call or_default, TEST_LIBDIRS,			)

# TEST_NO_3DSX		:= 1
# TEST_NO_SMDH		:= 1
# TEST_NO_CIA		:= 1
# TEST_NO_CCI		:= 1

#- VARIABLE_NAME ----- IMMEDIATE ?= ----- VARIABLE_NAME ------- VALUE ------------
INTERMEDIATE_DIR	:= $(call or_default, INTERMEDIATE_DIR,		build)
OUTPUT_DIR			:= $(call or_default, OUTPUT_DIR,			output)
OFILES_DIR			:= $(call or_default, OFILES_DIR,			$(INTERMEDIATE_DIR)/build)
PATCHED_MAKE_DIR	:= $(call or_default, PATCHED_MAKE_DIR,		$(INTERMEDIATE_DIR)/makefiles)
_3DS_RULES			:= $(call or_default, _3DS_RULES,			$(PATCHED_MAKE_DIR)/3ds-rules)
BASE_RULES			:= $(call or_default, BASE_RULES,			$(PATCHED_MAKE_DIR)/base_rules)
DEPSDIR				:= $(call or_default, DEPSDIR,				$(INTERMEDIATE_DIR)/deps)
TEST_ROMFS_TEMPDIR	:= $(call or_default, TEST_ROMFS_TEMPDIR,	$(INTERMEDIATE_DIR)/test-romfs)
DUMMY_APP_ICON		:= $(call or_default, DUMMY_APP_ICON,		$(INTERMEDIATE_DIR)/dummy-app-icon)
#---------------------------------------------------------------------------------

BUILD_3DSX_NAME 	:= $(BUILD_OUTPUT).3dsx
BUILD_SMDH_NAME		:= $(BUILD_OUTPUT).smdh
BUILD_CIA_NAME		:= $(BUILD_OUTPUT).cia
BUILD_CCI_NAME		:= $(BUILD_OUTPUT).cci

TEST_3DSX_NAME		:= $(TEST_OUTPUT).3dsx
TEST_SMDH_NAME		:= $(TEST_OUTPUT).smdh
TEST_CIA_NAME		:= $(TEST_OUTPUT).cia
TEST_CCI_NAME		:= $(TEST_OUTPUT).cci

# Eval_no_target creates the variale $1_$2 and assigns it $1_$2_NAME if $1_NO_$2 is empty.
# The first call would be equivalent to:
# BUILD_3DSX := $(if $(BUILD_NO_3DSX),,$(BUILD_3DSX_NAME))
$(call eval_no_target,BUILD,3DSX)
$(call eval_no_target,BUILD,SMDH)
$(call eval_no_target,BUILD,CIA)
$(call eval_no_target,BUILD,CCI)
$(call eval_no_target,TEST,3DSX)
$(call eval_no_target,TEST,SMDH)
$(call eval_no_target,TEST,CIA)
$(call eval_no_target,TEST,CCI) 

$(call gen_3dsxflags,BUILD)
$(call gen_3dsxflags,TEST)

####################################
# TODO: HFILES IS NOT YET BEING GENERATED CORRECTLY
#	HFILES is currently in the same directory as its sources.
#	To change this I'd need to understand what the rule for
#	this filetype does.
####################################
create_filelists = \
	$(eval $(1)_CFILES			:= $(call get_files, $(value $(1)_SOURCES),c)) \
	$(eval $(1)_CPPFILES		:= $(call get_files, $(value $(1)_SOURCES),cpp)) \
	$(eval $(1)_SFILES			:= $(call get_files, $(value $(1)_SOURCES),s)) \
	$(eval $(1)_PICAFILES		:= $(call get_files, $(value $(1)_SOURCES),v.pica)) \
	$(eval $(1)_SHLISTFILES		:= $(call get_files, $(value $(1)_SOURCES),shlist)) \
	$(eval $(1)_BINFILES		:= $(call get_files, $(value $(1)_DATA),*)) \
	\
	$(eval $(1)_OFILES_SOURCES	:= $(foreach file,$($1_CPPFILES:.cpp=.o) $($1_CFILES:.c=.o) $($1_SFILES:.s=.o),$(OFILES_DIR)/$(file))) \
	$(eval $(1)_OFILES_BIN		:= $(foreach file,$(addsuffix .o,$($(1)_BINFILES)) $($1_PICAFILES:.v.pica=.shbin.o) $($1_SHLISTFILES:.shlist=.shbin.o),$(OFILES_DIR)/$(file))) \
	$(eval $(1)_OFILES 			:= $($1_OFILES_BIN) $($1_OFILES_SOURCES)) \
	$(eval $(1)_HFILES			:= $($1_PICAFILES:.v.pica=_shbin.h) $($1_SHLISTFILES:.shlist=_shbin.h) $(addsuffix .h,$(subst .,_,$($1_BINFILES)))) \
	$(eval $(1)_INCLUDE			:= $(foreach dir,$($1_INCLUDES),-I$(dir)) \
		$(foreach dir,$($1_LIBDIRS),-I$(dir)/include) -I$($1_BUILD))

LD					:= $(if $(CPPFILES),$(CXX),$(CC))

$(call create_filelists,BUILD)
$(call create_filelists,TEST)

#---------------------------------------------------------------------------------
# MERGING BUILD AND TEST VARS
# Some variables for TEST are supposed to include their BUILD counterparts.
# I didn't figure out a way to keep the TEST_OFILES line DRY...
#---------------------------------------------------------------------------------
TEST_OFILES_SOURCES				+= $(filter-out %/main.o,$(BUILD_OFILES_SOURCES))
TEST_OFILES_BIN					+= $(BUILD_OFILES_BIN)
TEST_OFILES						:= $(TEST_OFILES_BIN) $(TEST_OFILES_SOURCES)
TEST_HFILES						+= $(BUILD_OFILES)
TEST_INCLUDE					+= $(BUILD_INCLUDE)

#---------------------------------------------------------------------------------
# DEFINE REQUIRED DIRECTORIES
#---------------------------------------------------------------------------------
REQUIRED_DIRS := \
$(dir $(BUILD_OUTPUT)) \
$(dir $(TEST_OUTPUT)) \
$(dir $(BUILD_OFILES)) \
$(dir $(TEST_OFILES)) \
$(INTERMEDIATE_DIR) \
$(OUTPUT_DIR) \
$(PATCHED_MAKE_DIR) \
$(dir $(_3DS_RULES)) \
$(dir $(BASE_RULES)) \
$(DEPSDIR) \
$(foreach dir,$(shell find $(BUILD_ROMFS) $(TEST_ROMFS) -type d -print),$(TEST_ROMFS_TEMPDIR)/$(dir)) \
$(dir $(DUMMY_APP_ICON))

#---------------------------------------------------------------------------------
# TARGETS
#---------------------------------------------------------------------------------

.PHONY: $(BUILD_TARGET) $(TEST_TARGET) all clean

all: $(BUILD_TARGET) $(TEST_TARGET)

clean:
	@$(RM) -r $(INTERMEDIATE_DIR)
	@$(RM) -r $(OUTPUT_DIR)

$(BUILD_TARGET): $(BUILD_3DSX) $(BUILD_SMDH) $(BUILD_CIA) $(BUILD_CCI) | $(REQUIRED_DIRS)
$(TEST_TARGET): $(TEST_3DSX) $(TEST_SMDH) $(TEST_CIA) $(TEST_CCI) | $(REQUIRED_DIRS)

#---------------------------------------------------------------------------------
# ROMFS GENERATION
# TEST_ROMFS is just build_romfs dir if they match and a temporary location
# otherwise.
# Both romfs depend on all their contents, so a target must be rebuilt if their
# romfs changes.
# TEST_ROMFS_TEMPDIR is buit by layering TEST_ROMFS_DIR on top of BUILD_ROMFS_DIR.
#---------------------------------------------------------------------------------

BUILD_ROMFS := $(BUILD_ROMFS_DIR)
TEST_ROMFS := $(if $(filter $(BUILD_ROMFS_DIR),$(TEST_ROMFS_DIR)),$(BUILD_ROMFS_DIR),$(TEST_ROMFS_TEMPDIR))

$(BUILD_ROMFS_DIR): $(wildcard $(BUILD_ROMFS)/**/)
$(TEST_ROMFS_DIR): $(wildcard $(TEST_ROMFS)/**/)
$(TEST_ROMFS_TEMPDIR): $(BUILD_ROMFS_DIR) $(TEST_ROMFS_DIR)
	@shopt -s nullglob && \
	 cp -r $(BUILD_ROMFS_DIR)/* $(BUILD_ROMFS_DIR)/.[^.]* $@ && \
	 cp -r $(TEST_ROMFS_DIR)/* $(TEST_ROMFS_DIR)/.[^.]* $@ && \
	 echo Rebuilt test-romfs.


#---------------------------------------------------------------------------------
# DIRECTORY GENERATION
# This section ensures that all intermediate and output directories are available.
#---------------------------------------------------------------------------------

$(REQUIRED_DIRS):
	@mkdir -p $@

#---------------------------------------------------------------------------------
# DEPENDENCY INJECTION
# Target-Specific variables are also in effect for all prerequisites of the specified
# target. This means they are inherited and can thus be set on the phony target.
#---------------------------------------------------------------------------------

VARS_TO_OVERRIDE := \
	OUTPUT \
	PRODUCTCODE \
	ROMFS \
	UNIQUEID \
	APP_TITLE \
	APP_DESCRIPTION \
	APP_AUTHOR \
	APP_ICON \
	ARCH \
	CFLAGS \
	CXXFLAGS \
	ASFLAGS \
	LDFLAGS \
	LIBS \
	LIBDIRS \
	_3DSXFLAGS \
	OFILES
$(call override_values,$(VARS_TO_OVERRIDE))

# Nullifies the APP_ICON prerequisite of %.smdh
APP_ICON = $(DUMMY_APP_ICON)
$(APP_ICON):
	touch $@

#---------------------------------------------------------------------------------
# CIA AND CCI GENERATION
# Builds cia and cci files using makerom and bannertool.
#---------------------------------------------------------------------------------
MAKEROMFLAGS = -rsf $(RFS_FILE) -elf $*.elf -DROMFS=$(ROMFS) -DTITLE=$(TITLE) \
	-DPRODUCTCODE=$(PRODUCTCODE) -DUNIQUEID=$(UNIQUEID)

%.cia: %.bnr %.elf
	@$(MAKEROM) -f cia -o $@ $(MAKEROMFLAGS)

%.cci: %.bnr %.elf
	@$(MAKEROM) -f cci -o $@ $(MAKEROMFLAGS)

%.bnr:
	@$(BANNERTOOL) makebanner -i $(BANNER_IMAGE) -a $(BANNER_AUDIO) -o $@

#---------------------------------------------------------------------------------
# MAKEFILE PATCHING
# See https://github.com/ahoischen/capture/issues/19 for a needlessly long writeup
# of the reasons behind patching base_rules. In short: I don't like where it puts
# stuff. The reason why I'm patching 3ds_rules is that it also includes base_rules.
#---------------------------------------------------------------------------------

$(BASE_RULES): $(DEVKITARM)/base_rules
	@sed 's|^%|$(OFILES_DIR)/%|g' $< > $@

$(_3DS_RULES): $(DEVKITARM)/3ds_rules
	@sed '/include/d' $< > $@

include $(BASE_RULES)
include $(_3DS_RULES)

#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
# rules for assembling GPU shaders
#---------------------------------------------------------------------------------
define shader-as
	$(eval CURBIN := $*.shbin)
	$(eval DEPSFILE := $(DEPSDIR)/$*.shbin.d)
	echo "$(CURBIN).o: $< $1" > $(DEPSFILE)
	echo "extern const u8" `(echo $(CURBIN) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"_end[];" > `(echo $(CURBIN) | tr . _)`.h
	echo "extern const u8" `(echo $(CURBIN) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"[];" >> `(echo $(CURBIN) | tr . _)`.h
	echo "extern const u32" `(echo $(CURBIN) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`_size";" >> `(echo $(CURBIN) | tr . _)`.h
	picasso -o $(CURBIN) $1
	bin2s $(CURBIN) | $(AS) -o $*.shbin.o
endef

%.shbin.o %_shbin.h : %.v.pica %.g.pica
	@echo $(notdir $^)
	@$(call shader-as,$^)

%.shbin.o %_shbin.h : %.v.pica
	@echo $(notdir $<)
	@$(call shader-as,$<)

%.shbin.o %_shbin.h : %.shlist
	@echo $(notdir $<)
	@$(call shader-as,$(foreach file,$(shell cat $<),$(dir $<)$(file)))