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
# TOOLING CONFIGURATION
# All these values are defined to be either a possibly exported value or the
# second parameter.
#- VARIABLE_NAME ----- IMMEDIATE ?= ----- VARIABLE_NAME ------- VALUE ------------
$(call config_option_s,i,	MAKEROM,			makerom)
$(call config_option_s,i,	BANNERTOOL,			bannertool)
$(call config_option_s,i,	INTERMEDIATE_DIR,	build)
$(call config_option_s,i,	OUTPUT_DIR,			output)
$(call config_option_s,i,	OFILES_DIR,			$(INTERMEDIATE_DIR)/build)
$(call config_option_s,i,	PATCHED_MAKE_DIR,	$(INTERMEDIATE_DIR)/makefiles)
$(call config_option_s,i,	DEPSDIR,			$(INTERMEDIATE_DIR)/deps)
$(call config_option_s,i,	TEST_ROMFS_TEMPDIR,	$(INTERMEDIATE_DIR)/test-romfs)
#---------------------------------------------------------------------------------
# REGULAR CONFIGURATION
#----------------------- NAME --- VALUE FOR REGULAR BUILDS ---- VALUE FOR TEST BUILDS
$(call config_option_t,i,TARGET,	build,						test)
$(call config_option_t,i,OUTPUT,	$(notdir $(CURDIR)),		$(notdir $(CURDIR))_capture)
$(call config_option_t,i,SOURCES,	$(shell find source -type d -print), $(shell find test -type d -print))
$(call config_option_t,i,INCLUDES,	include Catch/single_include cpptoml/include, cpptoml/include, Catch/single_include)
$(call config_option_t,i,DATA,		data,						test-data)
$(call config_option_t,i,PRODUCTCODE,	CTR-P-CTAP,				CTR-P-CTAP)
$(call config_option_t,i,ROMFS_DIR,	romfs,						romfs)
$(call config_option_t,i,UNIQUEID,	6d40a6,						6b40a6)
$(call config_option_t,i,RSFFILE,	assets/Application.rsf,		assets/Application.rsf)
$(call config_option_t,i,APP_TITLE,	$(notdir $(CURDIR)),		$(notdir $(CURDIR))_capture)
$(call config_option_t,i,APP_DESCRIPTION,	Built with devkitARM & libctru,	Built with devkitARM & libctru)
$(call config_option_t,i,APP_AUTHOR,	Unspecified Author,		Unspecified Author)
$(call config_option_t,i,APP_ICON,	$(CTRULIB)/default_icon.png,	$(CTRULIB)/default_icon.png)
$(call config_option_t,i,BANNER_IMAGE,	assets/image.png,		assets/image.png)
$(call config_option_t,i,BANNER_AUDIO,	assets/audio.wav,		assets/audio.wav)
$(call config_option_t,i,ARCH,			-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft)
$(call config_option_t,i,CFLAGS,		-g -Wall -O2 -mword-relocations -ffunction-sections $(BUILD_ARCH), \
										-g -Wall -O2 -mword-relocations -ffunction-sections $(TEST_ARCH))

$(call config_option_t,,CXXFLAGS,		$$(BUILD_CFLAGS) -DCATCH_CONFIG_NO_POSIX_SIGNALS -DCATCH_CONFIG_COLOUR_ANSI -DCATCH_CONFIG_CONSOLE_WIDTH=50 -std=gnu++17 \
										$$(TEST_CFLAGS)	-DCATCH_CONFIG_NO_POSIX_SIGNALS -DCATCH_CONFIG_COLOUR_ANSI -DCATCH_CONFIG_CONSOLE_WIDTH=50 -std=gnu++17)
$(call config_option_t,i,ASFLAGS,		-g $(BUILD_ARCH),		-g $(TEST_ARCH))
$(call config_option_t,i,LDFLAGS,		-specs=3dsx.specs -g $(BUILD_ARCH) -Wl$(,)-Map$(,)$(OUTPUT_DIR)/$(notdir $*.map), \
										-specs=3dsx.specs -g $(TEST_ARCH)  -Wl$(,)-Map$(,)$(OUTPUT_DIR)/$(notdir $*.map))
$(call config_option_t,i,LIBS,			-lctru -lm,				)
$(call config_option_t,i,LIBDIRS,		$(CTRULIB),				)



.DEFAULT_GOAL 		:= all

_3DS_RULES			:= $(PATCHED_MAKE_DIR)/3ds-rules
BASE_RULES			:= $(PATCHED_MAKE_DIR)/base-rules
include $(BASE_RULES)
include $(_3DS_RULES)


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
		$(foreach dir,$($1_LIBDIRS),-I$(dir)/include) -I$($1_BUILD)) \
	$(eval $(1)_LIBPATHS		:= $(foreach dir,$($(1)_LIBDIRS),-L$(dir)/lib))


$(call create_filelists,BUILD)
$(call create_filelists,TEST)

$(info $(BUILD_INCLUDE))
$(info $(BUILD_INCLUDES))

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
TEST_LIBPATHS					+= $(BUILD_LIBPATHS)

#---------------------------------------------------------------------------------
# Finalizing CFLAGS
#---------------------------------------------------------------------------------

BUILD_CFLAGS					+= $(BUILD_INCLUDE) -DARM11 -D_3DS 
TEST_CFLAGS 					+= $(TEST_INCLUDE) -DARM11 -D_3DS

#---------------------------------------------------------------------------------
# CREATE REQUIRED DIRECTORIES
#---------------------------------------------------------------------------------
REQUIRED_DIRS := $(sort \
$(dir $(BUILD_OUTPUT)) \
$(dir $(TEST_OUTPUT)) \
$(dir $(BUILD_OFILES)) \
$(dir $(TEST_OFILES)) \
$(INTERMEDIATE_DIR) \
$(OUTPUT_DIR) \
$(PATCHED_MAKE_DIR) \
$(dir $(_3DS_RULES)) \
$(dir $(BASE_RULES)) \
$(subst $(OFILES_DIR),$(DEPSDIR),$(sort $(dir $(BUILD_OFILES) $(TEST_OFILES)))) \
$(dir $(DUMMY_APP_ICON)))

_MKDIRS := $(shell mkdir -p $(REQUIRED_DIRS))
#---------------------------------------------------------------------------------
# TARGETS
#---------------------------------------------------------------------------------

.PHONY: $(BUILD_TARGET) $(TEST_TARGET) all clean

all: $(BUILD_TARGET) $(TEST_TARGET)

clean:
	@$(RM) -r $(INTERMEDIATE_DIR)
	@$(RM) -r $(OUTPUT_DIR)

$(BUILD_TARGET): $(BUILD_3DSX) $(BUILD_SMDH) $(BUILD_CIA) $(BUILD_CCI)
$(TEST_TARGET): $(TEST_3DSX) $(TEST_SMDH) $(TEST_CIA) $(TEST_CCI)

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
	 mkdir -p $(sort $(dir $^))
	 cp -r $(BUILD_ROMFS_DIR)/* $(BUILD_ROMFS_DIR)/.[^.]* $@ && \
	 cp -r $(TEST_ROMFS_DIR)/* $(TEST_ROMFS_DIR)/.[^.]* $@ && \
	 echo Rebuilt test-romfs.

#---------------------------------------------------------------------------------
# DEPENDENCY INJECTION
# Target-Specific variables are also in effect for all prerequisites of the specified
# target. This means they are inherited and can thus be set on the phony target.
#---------------------------------------------------------------------------------

BUILD_LD				= $(if $(BUILD_CPPFILES),$(CXX),$(CC))
TEST_LD					= $(if $(TEST_CPPFILES),$(CXX),$(CC))

VARS_TO_OVERRIDE := \
	OUTPUT \
	PRODUCTCODE \
	ROMFS \
	UNIQUEID \
	APP_TITLE \
	APP_DESCRIPTION \
	APP_AUTHOR \
	APP_ICON \
	RSFFILE \
	BANNER_AUDIO \
	BANNER_IMAGE \
	ARCH \
	CFLAGS \
	CXXFLAGS \
	ASFLAGS \
	LDFLAGS \
	LIBS \
	LIBPATHS \
	_3DSXFLAGS \
	OFILES \
	LD
$(call override_values,$(VARS_TO_OVERRIDE))

$(BUILD_OUTPUT).elf: $(BUILD_OFILES)
$(TEST_OUTPUT).elf: $(TEST_OFILES)

# Nullifies the APP_ICON prerequisite of %.smdh
DUMMY_APP_ICON := $(INTERMEDIATE_DIR)/dummy-app-icon
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

$(BASE_RULES): $(DEVKITARM)/base_rules | $(dir $(BASE_RULES))
	@sed 's|^%|$(OFILES_DIR)/%|g' $< > $@

$(_3DS_RULES): $(DEVKITARM)/3ds_rules | $(dir $(_3DS_RULES))
	@sed '/include/d' $< > $@

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