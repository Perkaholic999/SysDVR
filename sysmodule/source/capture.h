#pragma once
#include <switch.h>

#define STREAM_PACKET_MAGIC_VIDEO 0xDDDDDDDD
#define STREAM_PACKET_MAGIC_AUDIO 0xEEEEEEEE

/*
	This is higher than what suggested on switchbrw to fix https://github.com/exelix11/SysDVR/issues/91,
	See the comment in ReadVideoStream()
*/
#define VbufSz 0x50000

/*
	Audio is 16bit pcm at 48000hz stereo. In official software it's read in 0x1000 chunks
	that's 1024 samples per chunk (2 bytes per sample and stereo so divided by 4)
	(1 / 48000) * 1024 is 0,02133333 seconds per chunk.
	Smaller buffer sizes don't seem to work, only tested 0x400 and grc fails with 2212-0006
*/
#define AbufSz 0x1000

/*
	Audio batching adds some delay to the audio streaming in excange for less pressure on
	the USB and network protocols. A batching of 1 halves the number of audio transfers while
	adding about a frame of delay.
	This is acceptable as grc:d already doesn't provide real time audio.
	To disable support set the following to 0
*/
#define MaxABatching 2

typedef struct {
	u32 Magic;
	u32 DataSize;
	u64 Timestamp; //Note: timestamps are in usecs
} PacketHeader;

_Static_assert(sizeof(PacketHeader) == 16); //Ensure no padding, PACKED triggers a warning

typedef struct {
	PacketHeader Header;
	u8 Data[VbufSz];
} VideoPacket;

_Static_assert(sizeof(VideoPacket) == sizeof(PacketHeader) + VbufSz);

typedef struct {
	PacketHeader Header;
	u8 Data[AbufSz * (1 + MaxABatching)];
} AudioPacket;

_Static_assert(sizeof(AudioPacket) == sizeof(PacketHeader) + AbufSz * (1 + MaxABatching));

extern VideoPacket VPkt;
extern AudioPacket APkt;

Result CaptureInitialize();
void CaptureFinalize();

void CaptureSetAudioBatching(int batch);
int CaptureGetAudioBatching();

void CaptureResetStaticDropThreshold();
void CaptureSetStaticDropThreshold(int maxConsecutive);
int CaptureGetStaticDropThreshold();

// Captures video with grc:d, if no game is running this blocks and there's no way to terminate the call
bool CaptureReadVideo();

// Captures audio with grc:d, if no game is running this blocks and there's no way to terminate the call
bool CaptureReadAudio();