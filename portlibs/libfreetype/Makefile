# freetype-2.9.1 port for ORBIS
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
TargetFile := libfreetype
AllTarget = $(OutPath)/$(TargetFile).a


include $(Ps4Sdk)/make/ps4sdk.mk

IncludePath += -I. -Iinclude -Iinclude/freetype/config
CompilerFlags += -DFT_CONFIG_CONFIG_H="<ftconfig.h>" -DFT2_BUILD_LIBRARY -DFT_CONFIG_MODULES_H="<ftmodule.h>" -DFT_CONFIG_OPTIONS_H="<ftoption.h>" -DFT_ADVANCES_H="<freetype/ftadvanc.h>"


$(OutPath)/$(TargetFile).a: $(ObjectFiles)
	$(dirp)
	$(archive)

install:
	@cp $(OutPath)/$(TargetFile).a $(Ps4Sdk)/lib
	@cp include/ft2build.h $(Ps4Sdk)/include
	@cp -r include/freetype $(Ps4Sdk)/include
	@echo "Installed!"
