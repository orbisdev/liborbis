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
TargetFile := libfreetype-gl2
AllTarget = $(OutPath)/$(TargetFile).a

include $(Ps4Sdk)/make/ps4sdk.mk

IncludePath += -I$(Ps4Sdk)/include/freetype2 -I$(Ps4Sdk)/include/freetype2/freetype/config

$(OutPath)/$(TargetFile).a: $(ObjectFiles)
	$(dirp)
	$(archive)

install:
	@cp $(OutPath)/$(TargetFile).a $(Ps4Sdk)/lib
	@mkdir -p $(Ps4Sdk)/include/freetype-gl2
	@cp include/*.h $(Ps4Sdk)/include/freetype-gl2
	@echo "Installed!"
