/*

  minimp3_test demo, liborbis implementation
  -------------------------------------------

  masterzorag, 2019
  based on the code at https://github.com/lieff/minimp3/blob/master/minimp3_test.c

  orbis have fixed audioOut settings: 48Hz, 2ch s16le, plays max 1024 samples per frame;
  on linux you can use libao and will play using default audio device:
  clang minimp3_test.c -lm

  more useful notes:

  // record from pulseaudio
  pacat --record -d alsa_output.pci-0000_00_01.1.hdmi-stereo.monitor | sox -t raw -r 44100 -b 16 -c 2 -e signed-integer - -t raw -r 48000 -e signed-integer --endian little -S -V3 -b 16 output.bin

  pacat --record -d alsa_output.pci-0000_00_01.1.hdmi-stereo.monitor | sox -t raw -r 44100 -b 16 -c 2 -e signed-integer - -r 48000 -e signed-integer --endian little -S -V3 -b 16 output.wav

  // convert wav to raw pcm s16le
  sox output.wav -c 2 -r 48000 -b 16 -e signed-integer --endian little --magic -t raw -S -V3 output.bin

  // rip and convert audio to stereo mp3, 48Hz
  ffmpeg -i input.mp4 -c:a libmp3lame -ac 2 -b:a 190k -ar 48000 outputfile.mp3

  // remove Video stuff info, can break id3v2 parsing
  ffmpeg -i input.mp3 -map 0 -map -0:v -c:a libmp3lame -ac 2 -ab 128k -ar 48000 -map_metadata 0 -id3v2_version 3 outputfile.mp3
*/

#define MP4_MODE

#define MINIMP3_IMPLEMENTATION
#define MINIMP3_ONLY_MP3
#define MINIMP3_NO_WAV
//#define MINIMP3_ONLY_SIMD
//#define MINIMP3_NONSTANDARD_BUT_LOGICAL
//#define MINIMP3_ALLOW_MONO_STEREO_TRANSITION
#include "minimp3_ex.h"

#include "minimp3_test.h"

/*
#include <ao/ao.h>
ao_device *device;
ao_sample_format format;
int default_driver;
*/

#include <debugnet.h>

#include <orbisAudio.h>
#include <audioout.h>
static void minimp3_PlayCallback(OrbisAudioSample *_buf2, unsigned int length,void *userdata);
static int  minimp3_playint_channel;
static int  m_bPlaying = 0;  // Set to true when an mp3 is being played

// samples are shorts, * 2 channels
static short      snd[ 1152 *2];      // for 'two step' filling of buffer below (can be useless)
static short play_buf[(1024 *2) *4];  // small fixed buffer to store decoded pcm s16le samples

static int16_t read16le(const void *p)
{
    const uint8_t *src = (const uint8_t *)p;
    return ((src[0]) << 0) | ((src[1]) << 8);
}

#ifndef MINIMP3_NO_WAV
static char *wav_header(int hz, int ch, int bips, int data_bytes)
{
    static char hdr[44] = "RIFFsizeWAVEfmt \x10\0\0\0\1\0ch_hz_abpsbabsdatasize";
    unsigned long nAvgBytesPerSec = bips*ch*hz >> 3;
    unsigned int nBlockAlign      = bips*ch >> 3;

    *(int32_t *)(void*)(hdr + 0x04) = 44 + data_bytes - 8;   /* File size - 8 */
    *(int16_t *)(void*)(hdr + 0x14) = 1;                     /* Integer PCM format */
    *(int16_t *)(void*)(hdr + 0x16) = ch;
    *(int32_t *)(void*)(hdr + 0x18) = hz;
    *(int32_t *)(void*)(hdr + 0x1C) = nAvgBytesPerSec;
    *(int16_t *)(void*)(hdr + 0x20) = nBlockAlign;
    *(int16_t *)(void*)(hdr + 0x22) = bips;
    *(int32_t *)(void*)(hdr + 0x28) = data_bytes;
    return hdr;
}
#endif

#ifdef MP4_MODE
typedef struct
{
    mp3dec_t *mp3d;
    mp3dec_file_info_t *info;
    size_t allocated;
} frames_iterate_data;

/// minimp3 globals
mp3dec_t            mp3d;
mp3dec_file_info_t  info;
frames_iterate_data d;
mp3dec_map_info_t   map_info;

// for debug, but useless
void dump(unsigned char *p)
{
    char msg[1024];
    sprintf(&msg[0], "dump @ %p\n", p);
    int l = strlen(msg);
    for(int j=0; j< 64/16; j++)
    {
        int i;
        for(i=0; i<16; i++)
        {
            sprintf(&msg[l + i *3], "%.2x ", *p++);
        }
        sprintf(&msg[l + i *3], "\n");
        debugNetPrintf(DEBUG,"%s", msg);
        l = 0;
    }
}

// keep track of filling buffer, in samples
static int          fill     = 0;
static unsigned int numframe = 0;

static int frames_iterate_cb(void *user_data, const uint8_t *frame, int frame_size, size_t offset, mp3dec_frame_info_t *info)
{
    (void)offset;
    frames_iterate_data *d = user_data;
    d->info->channels = info->channels;
    d->info->hz       = info->hz;
    d->info->layer    = info->layer;
    /*
    debugNetPrintf(DEBUG,"(fi_cb) %p frame:%p size:%db frame_offset:%d %d\n", d, frame, frame_size, (int)offset, (d->allocated - d->info->samples*sizeof(mp3d_sample_t)));
    */
    if(numframe %500 == 0)
    { debugNetPrintf(DEBUG,"%p samples %zu, %zu\n",
          &d->info->buffer, d->info->samples, (int)offset); }

    // decode frame
    int samples = mp3dec_decode_frame(d->mp3d, frame, frame_size,
                                      (short*)&play_buf[fill], // direct copy to dst, no 'two step'
                                      //d->info->buffer + d->info->samples, // dst
                                      info);
    // dump(&snd);

    // write to play buffer
    //memcpy(&play_buf[fill], &snd[0], 4608);  // we can do in two step, but wasteful
    fill += 1152 /*samples*/ *2;

    if(numframe %200 == 0)
    {
        debugNetPrintf(INFO,"decoded, memcpy(%p, %p, %zu); fill:%d\n",
             &play_buf[fill],
             &snd, 4608,
              fill);
    }

    if (samples)
    {
        d->info->samples += samples*info->channels;
    }
    if(numframe %200 == 0) { debugNetPrintf(INFO,"fill now: %d, free: %lu\n\n", fill, sizeof(play_buf) - (fill * 2)); }

    //sceKernelUsleep(100000);

    return 0;
}
#endif


static const uint8_t      *buf2;       // will be moved
static const uint8_t *orig_buf2;
static       size_t        buf2_size;  // will be consumed
static       size_t   orig_buf2_size;
static mp3dec_frame_info_t frame_info2;


static void minimp3_PlayCallback(OrbisAudioSample *_buf2, unsigned int length,void *pdata)
{
    int handle = orbisAudioGetHandle(minimp3_playint_channel);

    // debugNetPrintf(3,"Inside ModPlayCallback %d\n",length);
    short *_buf = (short *)_buf2;

    if (m_bPlaying == 1)
    {
        #ifdef CHUNKED_READS
        if((buf2  - orig_buf2) >= SIZE)  // feed the reading buffer
        {
            // move remaining bytes to head of reading buffer
            memcpy((void*)&map_info.buffer[0], (void*)&map_info.buffer[SIZE], SIZE);
            mp3dec_read_chunk(&map_info, SIZE);
            buf2 -= SIZE;
            /*debugNetPrintf(DEBUG,"read_chunk %p %d\n", buf2, buf2  - orig_buf2);*/
        }
        #endif

        // Playing , so mix up a buffer
        if(fill < 2048)
        {
            if(numframe %200 == 0) { debugNetPrintf(DEBUG,"fill: %d samples, %db, snd:%db, play_buf:%db\n",
                                                           fill, fill * sizeof(short), sizeof(snd), sizeof(play_buf)); }

            /* void mp3dec_iterate_buf(const uint8_t *buf, size_t buf_size, MP3D_ITERATE_CB callback, void *user_data) */
            do
            {
                int free_format_bytes = 0, frame_size = 0;
                int i = mp3d_find_frame(buf2, buf2_size, &free_format_bytes, &frame_size);
                buf2      += i;
                buf2_size -= i;
                if (i && !frame_size)
                    continue;
                if (!frame_size)
                    break;
                const uint8_t *hdr = buf2;
                frame_info2.channels = HDR_IS_MONO(hdr) ? 1 : 2;
                frame_info2.hz = hdr_sample_rate_hz(hdr);
                frame_info2.layer = 4 - HDR_GET_LAYER(hdr);
                frame_info2.bitrate_kbps = hdr_bitrate_kbps(hdr);
                frame_info2.frame_bytes = frame_size;

                //debugNetPrintf(DEBUG,"ficb %p %p %d %d\n", &d, hdr, frame_size, hdr - orig_buf2);
                //log: [PS4][DEBUG]: ficb 210962768 880179fca 576 32778

                if (frames_iterate_cb(&d, hdr, frame_size, hdr - orig_buf2, &frame_info2))
                    break;
                buf2      += frame_size;
                buf2_size -= frame_size;

                if(numframe %200 == 0)
                { debugNetPrintf(DEBUG,"info %d buf2:%p buf2_size:%zu %d %d\n",
                                      i, buf2, buf2_size, frame_info2.hz, frame_info2.channels); }

          break;  // don't loop all frames!!!
            } while (1);

        }

        /* write to audio device buffers */
      //for (count = 0; count < length * 2; count++) { *(_buf + count)  = play_buf[count]; }
        memcpy(_buf, &play_buf, length *2 * sizeof(short));

        // move remaining samples to head of buffer
        memcpy(&play_buf[0], &play_buf[1024 *2], sizeof(play_buf) - fill *2);
        /*
        debugNetPrintf(INFO,"  move,  memcpy(%p, %p, %zu); %d %d\n", &play_buf[0],
                                                                     &play_buf[1024 *2],
                                                               sizeof(play_buf) - fill *2,
                                                                     &play_buf[fill] - &play_buf[0]);
        */
        fill -= 1024 *2;

        if(buf2_size <= 0  // no more frames to decode
        || fill < 0)
        { debugNetPrintf(DEBUG,"buf2_size:%zu, fill: %d\n", buf2_size, fill);
          m_bPlaying = 0; }

    }
    else // Not Playing , so clear buffer
    {
        if(numframe %1000 == 0) { debugNetPrintf(DEBUG,"Inside minimp3_PlayCallback not playing m_bPlaying is %d\n",m_bPlaying); }

      //for (count = 0; count < length * 2; count++) { *(_buf + count)  = 0; }
        memset(_buf, 0, length *2 * sizeof(short));

        sleep(1);
    }
    numframe++;

    // wait for last data
    sceAudioOutOutput(handle, NULL);

    //sceKernelUsleep(100000);
}


void minimp3_Init(int channel)
{
    minimp3_playint_channel = channel;

    /// minimp3
    d.mp3d      = &mp3d;
    d.info      = &info;
    d.allocated = SIZE *2;  // twice the readed chunk
    mp3dec_init(&mp3d);
    memset(&info,        0, sizeof(info));
    memset(&frame_info2, 0, sizeof(frame_info2));
}


int minimp3_Load(char *input_file_name)
{
    // -- Setup for default driver --
    /*
    ao_initialize();
    default_driver = ao_default_driver_id();
    format.bits = 16;
    format.channels = 2;
    format.rate = 48000;
    format.byte_format = AO_FMT_LITTLE;

    // -- Open driver --
    device = ao_open_live(default_driver, &format, NULL );
    */

    // internally uses liborbisFile()!
    int ret = mp3dec_open_file(input_file_name, &map_info);

    if(ret == 0) { return 0; }

    // address buffer pointer and size for callback
    buf2      = map_info.buffer;
    buf2_size = map_info.size;

    //debugNetPrintf(DEBUG,"1. buf2: %p, buf2_size:%zu, fill: %d\n", buf2, buf2_size, fill);

    /* skip id3v2 */
    size_t id3v2size = mp3dec_skip_id3v2(buf2, buf2_size);
    debugNetPrintf(DEBUG,"id3v2size:%zu\n", id3v2size);
    if(id3v2size > buf2_size) return -1;
    orig_buf2      = buf2;
    buf2          += id3v2size;  // (we already read first chunk)
    buf2_size     -= id3v2size;
    orig_buf2_size = buf2_size;

#ifdef __AFL_HAVE_MANUAL_CONTROL
    __AFL_INIT();
    while (__AFL_LOOP(1000)) {
#endif
    /*
      we don't loop and decode whole song, we do by callback by controlling filling of a play_buffer
      the following facility is not used!

        decode_file(input_file_name, buf_ref, ref_size, file_out, wave_out);
    */
#ifdef __AFL_HAVE_MANUAL_CONTROL
    }
#endif

    /*
    // libao end
    ao_close(device);
    ao_shutdown();
    */
    //debugNetPrintf(DEBUG,"2. buf2: %p, buf2_size:%zu, fill: %d\n", buf2, buf2_size, fill);

    return 1;
}

int minimp3_Play(void)
{
    fill = 0;
    orbisAudioSetCallback(minimp3_playint_channel, minimp3_PlayCallback, 0);
    m_bPlaying = 1;
    return 1;
}

/*
void minimp3_Pause(int flag)
{
    m_bPlaying = (flag) ? 0 : 1;
}
*/

void minimp3_End(void)
{
    m_bPlaying = 0;
    orbisAudioSetCallback(minimp3_playint_channel, 0,0);

    mp3dec_close_file(&map_info);
}

void minimp3_Loop1(void)
{
    m_bPlaying = 0;

    // reset buffer pointer and size for callback
    buf2       = orig_buf2;
    buf2_size  = orig_buf2_size;
    fill       = 0;
    m_bPlaying = 1;
}

void minimp3_Loop(char *selected)
{
    minimp3_End();
    minimp3_Init(0);

    debugNetPrintf(DEBUG,"selected: %s\n", selected);

    int ret = minimp3_Load(selected);
    if(ret)
        minimp3_Play();
}
