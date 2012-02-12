/* Arcan-fe, scriptable front-end engine
 *
 * Arcan-fe is the legal property of its developers, please refer
 * to the COPYRIGHT file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef _HAVE_ARCAN_FRAMESERVER
#define _HAVE_ARCAN_FRAMESERVER

#define ARCAN_FRAMESERVER_VCACHE_LIMIT 4
#define ARCAN_FRAMESERVER_ACACHE_LIMIT 24
#define ARCAN_FRAMESERVER_DEFAULT_VTHRESH_SKIP 60
#define ARCAN_FRAMESERVER_ABUFFER_SIZE 4 * 1024
#define ARCAN_FRAMESERVER_IGNORE_SKIP_THRESH 450

#define VID_FD 3
#define AUD_FD 4
#define CTRL_FD 5

struct arcan_ffmpeg_context;

enum arcan_playstate {
	ARCAN_PASSIVE = 0,
	ARCAN_PLAYING = 1,
	ARCAN_PAUSED = 2,
	ARCAN_FINISHED = 3,
	ARCAN_SUSPENDED = 4
};

typedef struct {
	/* video */
	uint16_t width;
	uint16_t height;
	uint16_t bpp;
	uint8_t sformat;
	uint8_t dformat;
	uint16_t vskipthresh;

	/* audio */
	uint16_t samplerate;
	uint8_t channels;
	uint8_t format;
	uint16_t vfthresh;

	bool ready;
} arcan_frameserver_meta;

typedef struct {
/* video / audio properties used */
	arcan_frameserver_meta desc;
	frame_queue vfq, afq;
	
/* original filename, needed for reloading */
	char* source;
	
/*  OS- specific, defined in general.h */
	shm_handle shm;
	int key; /*shmid */

	arcan_aobj_id aid;
	arcan_vobj_id vid;

/* used for plyaing, pausing etc. */
	enum arcan_playstate playstate;
	int64_t lastpts;
	int64_t starttime;
	bool loop;

/* set if color space conversion is done in process or not */
	bool extcc;
	unsigned int lastntr;

	/* timing */
	uint32_t vfcount;
	double bpms;
	double audioclock;

	process_handle child;
	bool child_alive;
} arcan_frameserver;

/* contains both structures for managing movie- playback,
 * both video and audio support functions */

/* all the necessary ffmpeg- foreplay */
arcan_frameserver* arcan_frameserver_spawn_server(char* fname, bool extcc, bool loop, arcan_frameserver* res);

/* enable the forked process to start decoding */
arcan_errc arcan_frameserver_playback(arcan_frameserver*);
arcan_errc arcan_frameserver_pause(arcan_frameserver*, bool syssusp);
arcan_errc arcan_frameserver_resume(arcan_frameserver*);
void arcan_frameserver_dropsemaphores(arcan_frameserver*);
void arcan_frameserver_tick_control(arcan_frameserver*);
void arcan_frameserver_dropsemaphores_keyed(char*);
bool arcan_frameserver_check_frameserver(arcan_frameserver*);

/* override the defauult queue opts (may be necessary for some frame-server sources */
void arcan_frameserver_queueopts_override(unsigned short vcellcount, unsigned short abufsize, unsigned short acellcount);
void arcan_frameserver_queueopts(unsigned short* vcellcount, unsigned short* acellcount, unsigned short* abufsize);

ssize_t arcan_frameserver_shmvidcb(int fd, void* dst, size_t ntr);
ssize_t arcan_frameserver_shmaudcb(int fd, void* dst, size_t ntr);

/* return a callback function for retrieving appropriate video-feeds */
int8_t arcan_frameserver_videoframe(enum arcan_ffunc_cmd cmd, uint8_t* buf, uint32_t s_buf, uint16_t width, uint16_t height, uint8_t bpp, unsigned src, vfunc_state vstate);

/* return a callback function for retrieving appropriate audio-feeds */
arcan_errc arcan_frameserver_audioframe(void* aobj, arcan_aobj_id id, unsigned buffer, void* tag);

/* stop playback and free resources associated with a movie */
arcan_errc arcan_frameserver_free(arcan_frameserver*, bool loop);
#endif
