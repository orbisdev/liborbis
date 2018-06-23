modplayer sample for PlayStation 4
==================================
 
 What does this do?
===================
 A basic sample with modplayer, pad, and graphic support with liborbis


```
$ ./ps4sh
ps4sh version 1.0
/Users/bigboss/.ps4shrc: No such file or directory

log: [HOST][INFO]: [PS4SH] Ready
log: [PS4][INFO]: debugnet initialized
log: [PS4][INFO]: Copyright (C) 2010,2016 Antonio Jose Ramos Marquez aka bigboss @psxdev
log: [PS4][INFO]: ready to have a lot of fun...
log: [PS4][INFO]: [PS4LINK] Server request thread UID: 0x816209C0
log: [PS4][DEBUG]: [PS4LINK] Created ps4link_requests_sock: 14
log: [PS4][DEBUG]: [PS4LINK] bind to ps4link_requests_sock done
log: [PS4][DEBUG]: [PS4LINK] Ready for connection 1
log: [PS4][INFO]: [PS4LINK] Waiting for connection
log: [PS4][INFO]: [PS4LINK] Server command thread UID: 0x81620E40
log: [PS4][DEBUG]: [PS4LINK] Command Thread Started.
log: [PS4][DEBUG]: [PS4LINK] Created ps4link_commands_sock: 16
log: [PS4][DEBUG]: [PS4LINK] Command listener waiting for commands...
ps4sh> connect
log: [HOST][INFO]: [PS4SH] Connecting to fio ps4link ip 192.168.1.17
log: [HOST][INFO]: [PS4SH] PlayStation is listening at 192.168.1.17
log: [PS4][DEBUG]: [PS4LINK] Client connected from 192.168.1.3 port: 12482

log: [PS4][INFO]: [PS4LINK] sock ps4link_fileio set 15 connected 1
log: [PS4][DEBUG]: [ORBISLINK] Initialized and connected from pc/mac ready to receive commands
log: [PS4][INFO]: [PS4LINK] Waiting for connection
log: [PS4][DEBUG]: liborbispad scePadInit return 0
log: [PS4][DEBUG]: liborbispad sceUserServiceGetInitialUser return error 0x80960006
log: [PS4][DEBUG]: liborbispad scePadOpen return handle 0x 30a0500
log: [PS4][DEBUG]: liborbis2d sceVideoOutOpen return video handle 0x4e110100
log: [PS4][DEBUG]: sceKernelCreateEqueue return 0
log: [PS4][DEBUG]: liborbis2d sceVideoOutAddFlipEvent return 0
log: [PS4][DEBUG]: liborbis2d video memory initialized
log: [PS4][DEBUG]: liborbis2d orbis2dMalloc buffer 0, new pointer 210600000
log: [PS4][DEBUG]: liborbis2d orbis2dMalloc buffer 1, new pointer 210984000
log: [PS4][DEBUG]: liborbis2d orbis2dAllocDisplayBuffer done
log: [PS4][DEBUG]: liborbis2d sceVideoOutSetBufferAttribute done
log: [PS4][DEBUG]: liborbis2d sceVideoOutRegisterBuffers return 0x       0
log: [PS4][DEBUG]: liborbis2d initialized
log: [PS4][DEBUG]: [ORBISLINK] orbis2dInit return 1
log: [PS4][DEBUG]: [orbisAudio] sceAudioOutInit return 0
log: [PS4][DEBUG]: [orbisAudio] initialized!
log: [PS4][DEBUG]: [orbisAudio] buffers 0 for audio 0 500840 created
log: [PS4][DEBUG]: [orbisAudio] buffers 1 for audio 0 501860 created
log: [PS4][DEBUG]: [orbisAudio] buffers for audio channel 0 created
log: [PS4][DEBUG]: [orbisAudio] sceAudioOutOpen  1024 samples
log: [PS4][DEBUG]: [orbisAudio] sceAudioOutOpen handle  20000007
log: [PS4][DEBUG]: [orbisAudio] audio channel 0 thread UID: 0x81621BC0 created
log: [PS4][DEBUG]: orbisAudioChannelThread channel 0
log: [PS4][DEBUG]: orbisAudioChannelThread channel samples 0 1024
log: [PS4][DEBUG]: orbisAudioChannelThread channel samples 1 1024
log: [PS4][DEBUG]: orbisAudioChannelThread buffer 0 500840
log: [PS4][DEBUG]: orbisAudioChannelThread buffer 1 501860
log: [PS4][DEBUG]: [orbisAudio] orbisAudioChannelThread 0  1 ready to have a lot of fun...
log: [PS4][DEBUG]: [ORBISLINK] orbisAudioInitChannel return 0
log: [PS4][DEBUG]: [ORBISLINK]Loading homebrew.elf from host
log: [PS4][DEBUG]: [ORBISLINK] orbisExecUserElf called
log: [PS4][DEBUG]: [PS4LINK] file open req (host0:homebrew.elf, 0 0)
log: [HOST][DEBUG]: [PS4SH] Opening homebrew.elf flags 0
log: [HOST][DEBUG]: [PS4SH] Open return 7
log: [PS4][DEBUG]: [PS4LINK] file open reply received (ret 7)
log: [PS4][DEBUG]: [PS4LINK] file lseek req (fd: 7)
log: [HOST][DEBUG]: [PS4SH] 641528 result of lseek 0 offset 2 whence
log: [PS4][DEBUG]: [PS4LINK] ps4link_lseek_file: lseek reply received (ret 641528)
log: [PS4][DEBUG]: [PS4LINK] file lseek req (fd: 7)
log: [HOST][DEBUG]: [PS4SH] 0 result of lseek 0 offset 0 whence
log: [PS4][DEBUG]: [PS4LINK] ps4link_lseek_file: lseek reply received (ret 0)
log: [PS4][DEBUG]: [ORBISLINK] before orbisSysMmap
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: Reply said there's 641528 bytes to read (wanted 641528)
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 0  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 1  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 2  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 3  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 4  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 5  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 6  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 7  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 8  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 9  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 10  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 11  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 12  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 13  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 14  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 15  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 16  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 17  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 18  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 19  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 20  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 21  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 22  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 23  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 24  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 25  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 26  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 27  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 28  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 29  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 30  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 31  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 32  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 33  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 34  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 35  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 36  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 37  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 38  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 39  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 40  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 41  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 42  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 43  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 44  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 45  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 46  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 47  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 48  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 49  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 50  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 51  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 52  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 53  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 54  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 55  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 56  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 57  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 58  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 59  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 60  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 61  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 62  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 63  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 64  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 65  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 66  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 67  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 68  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 69  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 70  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 71  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 72  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 73  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 74  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 75  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 76  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 77  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 78  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 79  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 80  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 81  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 82  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 83  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 84  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 85  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 86  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 87  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 88  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 89  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 90  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 91  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 92  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 93  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 94  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 95  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 96  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 97  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 98  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 99  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 100  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 101  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 102  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 103  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 104  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 105  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 106  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 107  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 108  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 109  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 110  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 111  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 112  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 113  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 114  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 115  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 116  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 117  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 118  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 119  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 120  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 121  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 122  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 123  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 124  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 125  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 126  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 127  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 128  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 129  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 130  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 131  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 132  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 133  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 134  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 135  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 136  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 137  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 138  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 139  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 140  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 141  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 142  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 143  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 144  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 145  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 146  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 147  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 148  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 149  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 150  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 151  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 152  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 153  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 154  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 155  readed 6648
log: [PS4][DEBUG]: [PS4LINK] ps4link_file: file close req (fd: 7)
log: [PS4][DEBUG]: [PS4LINK] ps4link_close_file: close reply received (ret 0)
log: [PS4][DEBUG]: [ORBISLINK] orbisExecUserElf ready to run elf
log: [PS4][DEBUG]: [ORBISLINK] orbisUserRun malloc for argument
log: [PS4][DEBUG]: [ORBISLINK] orbisUserRun after malloc for argument
log: [PS4][DEBUG]: [ORBISLINK] orbisUserRun after ps4MemoryProtectedCreate
log: [PS4][DEBUG]: [ORBISLINK] orbisUserRun after ps4MemoryProtectedGetWritableAddress writable=21c600000
log: [PS4][DEBUG]: [ORBISLINK] orbisUserRun after ps4MemoryProtectedGetExecutableAddress executable=21c600000
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate in segments length=6
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate before elfLoaderInstantiate memcpy 21c600040 21052c040 336
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy before memcpy
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy after memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate after elfLoaderInstantiate memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate before elfLoaderInstantiate memcpy 21c600190 21052c190 25
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy before memcpy
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy after memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate after elfLoaderInstantiate memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate before elfLoaderInstantiate memcpy 21c600000 21052c000 414064
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy before memcpy
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy after memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate after elfLoaderInstantiate memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate before elfLoaderInstantiate memcpy 21c865170 210591170 152752
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy before memcpy
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy after memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate after elfLoaderInstantiate memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate before elfLoaderInstantiate orbisMemorySet
log: [PS4][DEBUG]: [ORBISLINK] orbisMemorySet before memset
log: [PS4][DEBUG]: [ORBISLINK] orbisMemorySet after memset
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate after elfLoaderInstantiate orbisMemorySet
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate before elfLoaderInstantiate memcpy 21c865170 210591170 256
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy before memcpy
log: [PS4][DEBUG]: [ORBISLINK] orbisMemoryCopy after memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderInstantiate after elfLoaderInstantiate memcpy
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderLoad  after elfLoaderInstantiate return=0
log: [PS4][DEBUG]: [ORBISLINK] elfLoaderLoad after elfLoaderRelocate return=0
log: [PS4][DEBUG]: [ORBISLINK] orbisUserRun after elfLoaderLoad return r=0 readable=21c600000 executable=21c600000
log: [PS4][DEBUG]: [ORBISLINK] orbisUserRun after set argument->main 21c635cc0
log: [PS4][DEBUG]: [ORBISLINK] orbisUserRun after elfDestroyAndFree
log: [PS4][DEBUG]: [ORBISLINK] New user elf thread UID: 0x81622040
log: [PS4][DEBUG]: [ORBISLINK] orbisUserMain Configuration pointer 81be500, pointer_conf string 81be500
log: [PS4][INFO]: debugnet already initialized using configuration from ps4link
log: [PS4][INFO]: debugnet_initialized=1 SocketFD=13 logLevel=3
log: [PS4][INFO]: ready to have a lot of fun...
log: [PS4][INFO]: ps4link already initialized using configuration from ps4link
log: [PS4][INFO]: ps4link_fileio_active=1
log: [PS4][INFO]: ps4link_cmdsio_active=1
log: [PS4][INFO]: ps4link_initialized=1
log: [PS4][INFO]: ps4link_requests_port=18193
log: [PS4][INFO]: ps4link_commands_port=18194
log: [PS4][INFO]: ps4link_debug_port=18194
log: [PS4][INFO]: ps4link_fileio_sock=15
log: [PS4][INFO]: ps4link_requests_sock=14
log: [PS4][INFO]: ps4link_commands_sock=16
log: [PS4][DEBUG]: [PS4LINK] Initialized and connected from pc/mac ready to receive commands
log: [PS4][DEBUG]: liborbisPad already initialized using configuration external
log: [PS4][DEBUG]: orbispad_initialized=1
log: [PS4][DEBUG]: ready to have a lot of fun...
log: [PS4][DEBUG]: liborbis2d already initialized using configuration external
log: [PS4][DEBUG]: orbis2d_initialized=1
log: [PS4][DEBUG]: ready to have a lot of fun...
log: [PS4][DEBUG]: liborbisaudio already initialized using configuration external
log: [PS4][DEBUG]: orbisaudio_initialized=1
log: [PS4][DEBUG]: ready to have a lot of fun...
log: [PS4][DEBUG]: [PS4LINK] file open req (host0:zweifeld.mod, 0 0)
log: [HOST][DEBUG]: [PS4SH] Opening zweifeld.mod flags 0
log: [HOST][DEBUG]: [PS4SH] Open return 7
log: [PS4][DEBUG]: [PS4LINK] file open reply received (ret 7)
log: [PS4][DEBUG]: [PS4LINK] file lseek req (fd: 7)
log: [HOST][DEBUG]: [PS4SH] 34148 result of lseek 0 offset 2 whence
log: [PS4][DEBUG]: [PS4LINK] ps4link_lseek_file: lseek reply received (ret 34148)
log: [PS4][DEBUG]: [PS4LINK] file lseek req (fd: 7)
log: [HOST][DEBUG]: [PS4SH] 0 result of lseek 0 offset 0 whence
log: [PS4][DEBUG]: [PS4LINK] ps4link_lseek_file: lseek reply received (ret 0)
log: [HOST][DEBUG]: [PS4SH] read 34148 bytes of file descritor 7
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: Reply said there's 34148 bytes to read (wanted 34148)
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 0  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 1  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 2  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 3  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 4  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 5  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 6  readed 4096
log: [PS4][DEBUG]: [PS4LINK] ps4link_read_file: chunk 7  readed 5476
log: [PS4][DEBUG]: [PS4LINK] ps4link_file: file close req (fd: 7)
log: [PS4][DEBUG]: [PS4LINK] ps4link_close_file: close reply received (ret 0)
log: [PS4][DEBUG]: In play 0
log: [PS4][DEBUG]: In play m_bPlaying 1
log: [PS4][DEBUG]: Left pressed
log: [PS4][DEBUG]: Left pressed
log: [PS4][DEBUG]: Left pressed
log: [PS4][DEBUG]: Left pressed
log: [PS4][DEBUG]: Left pressed
log: [PS4][DEBUG]: Left pressed
log: [PS4][DEBUG]: Left pressed
log: [PS4][DEBUG]: Left pressed
log: [PS4][DEBUG]: Left pressed
log: [PS4][DEBUG]: Right pressed
log: [PS4][DEBUG]: Right pressed
log: [PS4][DEBUG]: Right pressed
log: [PS4][DEBUG]: Right pressed
log: [PS4][DEBUG]: Right pressed
log: [PS4][DEBUG]: Right pressed
log: [PS4][DEBUG]: Right pressed
log: [PS4][DEBUG]: Up pressed
log: [PS4][DEBUG]: Up pressed
log: [PS4][DEBUG]: Up pressed
log: [PS4][DEBUG]: Up pressed
log: [PS4][DEBUG]: Up pressed
log: [PS4][DEBUG]: Down pressed
log: [PS4][DEBUG]: Down pressed
log: [PS4][DEBUG]: Down pressed
log: [PS4][DEBUG]: Down pressed
log: [PS4][DEBUG]: Down pressed
log: [PS4][DEBUG]: Down pressed
log: [PS4][DEBUG]: Down pressed
log: [PS4][DEBUG]: Square pressed
log: [PS4][DEBUG]: Circle pressed reset position and color red
log: [PS4][DEBUG]: Cross pressed rand color
log: [PS4][DEBUG]: [orbisAudio] orbisAudioChannelThread 1  exit...
log: [PS4][DEBUG]: Triangle pressed exit
log: [PS4][DEBUG]: [orbisAudio] closing audio handle
log: [PS4][DEBUG]: [orbisAudio] free buffers channel
log: [PS4][DEBUG]: [orbisAudio] finished
log: [PS4][DEBUG]: [ORBISLINK] orbisUserMain return (user): 0
log: [PS4][DEBUG]: [orbisAudio] finished
log: [PS4][DEBUG]: liborbis2d scePadClose return 0x       0
log: [PS4][DEBUG]: liborbisPad finished[PS4][DEBUG]: liborbis2d sceVideoOutClose return 0x       0
log: [PS4][DEBUG]: liborbis2d finished
log: [PS4][DEBUG]: [PS4LINK] Aborting ps4link_requests_sock
log: [PS4][INFO]: [PS4LINK] sceNetAccept error (0x80410104)
log: [PS4][DEBUG]: [PS4LINK] exit thread requests
log: [PS4][DEBUG]: [PS4LINK] closing fileio_sock
log: [HOST][INFO]: [PS4SH] tryng to reconnect to 192.168.1.17: 0
log: [HOST][INFO]: [PS4SH] tryng to reconnect to 192.168.1.17: 1
log: [HOST][INFO]: [PS4SH] tryng to reconnect to 192.168.1.17: 2
log: [HOST][ERROR]: [PS4SH] Impossible to connect to PlayStation 4 at 192.168.1.17
log: [HOST][INFO]: [PS4SH] Load PS4Link in your PlayStation, then run connect
log: [PS4][DEBUG]: [PS4LINK] closing server_request_sock
ps4sh>
```
