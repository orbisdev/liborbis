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

target ?= ps4_elf
TargetFile=homebrew.elf

###################################

#Libraries += -lfreetype -ldebugnet -lps4link -lelfloader -lorbis2d -lpng -lz -lorbisPad -lorbisAudio -lmod -lorbisFileBrowser -lorbisXbmFont -lSceNet_stub -lSceAudioOut_stub -lSceVideoOut_stub -lSceSystemService_stub  -lSceUserService_stub -lSceGnmDriver_stub -lScePad_stub -lSceLibcInternal_stub -lSceIme_stub -lSceSysmodule_stub

include $(Ps4Sdk)/make/ps4sdk.mk
LinkerFlags += -lfreetype2 -ldebugnet -lps4link -lelfloader -lorbis2d -lorbisPad -lorbisAudio -lorbisXbmFont -lmod -lSceNet_stub -lSceAudioOut_stub -lSceVideoOut_stub -lSceSystemService_stub -lSceUserService_stub -lSceGnmDriver_stub -lScePad_stub -lkernel_stub

//CompilerFlags += -DUSE_SSE2 -msse2 -ffast-math -mllvm -unroll-count=4 -mllvm -force-vector-width=4 -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -fsave-optimization-record -loop-vectorize
//CompilerFlags += -DUSE_SSE2 -msse2 -ffast-math -mllvm -unroll-count=4 -mllvm -force-vector-width=4

//CompilerFlags += -D__AVX__ -mavx -ffast-math -mllvm -unroll-count=8 -mllvm -force-vector-width=8 -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -fsave-optimization-record -fvectorize
CompilerFlags += -D__AVX__ -mavx -ffast-math -mllvm -unroll-count=8 -mllvm -force-vector-width=8

IncludePath += -I$(Ps4Sdk)/include/freetype2





