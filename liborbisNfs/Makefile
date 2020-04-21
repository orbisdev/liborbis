ifndef Ps4Sdk
ifdef ps4sdk
Ps4Sdk := $(ps4sdk)
endif
ifdef PS4SDK
Ps4Sdk := $(PS4SDK)
endif
ifndef Ps4Sdk
$(error Neither PS4SDK, Ps4Sdk nor ps4sdk set)
endif
endif

target := ps4_lib
OutPath := lib
TargetFile := liborbisNfs
AllTarget = $(OutPath)/$(TargetFile).a

include $(Ps4Sdk)/make/ps4sdk.mk

CompilerFlags += -DHAVE_CONFIG_H -D_U_="__attribute__((unused))" -DNDEBUG
CompilerFlagsCpp += -DHAVE_CONFIG_H -D_U_="__attribute__((unused))" -DNDEBUG
IncludePath += -I./include/nfsc

$(OutPath)/$(TargetFile).a: $(ObjectFiles)
	$(dirp)
	$(archive)

install:
	@cp $(OutPath)/$(TargetFile).a $(Ps4Sdk)/lib
	@mkdir -p $(Ps4Sdk)/include/nfsc
	@cp include/nfsc/*.h $(Ps4Sdk)/include/nfsc
	@cp include/orbisNfs.h $(Ps4Sdk)/include
	@echo "$(TargetFile) Installed!"
