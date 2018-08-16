#SDL2-ttf-2.0.14 port for ORBIS
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
TargetFile := libSDL2_ttf
AllTarget = $(OutPath)/$(TargetFile).a


include $(Ps4Sdk)/make/ps4sdk.mk

IncludePath += -I$(Ps4Sdk)/include/SDL2


$(OutPath)/$(TargetFile).a: $(ObjectFiles)
	$(dirp)
	$(archive)

install:
	@cp $(OutPath)/$(TargetFile).a $(Ps4Sdk)/lib
	@mkdir -p $(Ps4Sdk)/include/SDL2
	@cp include/*.h $(Ps4Sdk)/include/SDL2
	@echo "Installed!"
