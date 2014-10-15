#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include "audio.h"
#include "maestro.h"
#include "mem.h"
#include "pi-usb.h"
#include "piface.h"
#include "server.h"
#include "talking-skull.h"

static maestro_t *maestro;
static piface_t *piface;

#define SERVO_ID 0
#define SERVO_MIN 20
#define SERVO_MAX 90
#define EYES 7

#define HISTORY_EPSILON 2
#define N_HISTORY 20
#define GAIN_TARGET 75

static double history[N_HISTORY];
static size_t history_i;
static double sum_history;
static bool history_full;

static pthread_mutex_t speak_lock;
static audio_t *out;
static talking_skull_t *skull;

static double gain = 10;

static void
servo_update(void *unused, double pos)
{
    if (pos > HISTORY_EPSILON) {
        sum_history = sum_history - history[history_i] + pos;
        history[history_i] = pos;
        history_i = (history_i + 1) % N_HISTORY;
	if (history_i == 0) history_full = true;
	if (history_full) {
	    gain = GAIN_TARGET / (sum_history / N_HISTORY);
	}
    }

    pos *= gain;
    if (pos > 100) pos = 100;
    if (maestro) {
	maestro_set_servo_pos(maestro, SERVO_ID, pos * (SERVO_MAX - SERVO_MIN) / 100 + SERVO_MIN);
	//piface_set(s->p, EYES, pos > 50);
    } else {
	printf("servo: %.0f\n", pos);
    }

    piface_set(piface, EYES, pos >= 50);
}

static void
play_one_buffer(void *buffer, size_t size)
{
    pthread_mutex_lock(&speak_lock);
    talking_skull_play(skull, buffer, size);
    if (! audio_play_buffer(out, buffer, size)) {
	fprintf(stderr, "Failed to play buffer!\n");
	exit(1);
    }
    pthread_mutex_unlock(&speak_lock);
}

static char *
remote_event(void *unused, const char *command)
{
    unsigned char *data;
    size_t i, j;

fprintf(stderr, "remote: [%s]\n", command);
    if (strncmp(command, "play ", 5) != 0) {
	return strdup("Invalid command");
    }

    data = malloc(strlen(command));
    for (i = 5; command[i]; i++, j++) {
	if (command[i] == '\\') {
	    switch(command[++i]) {
	    case '\\': data[j] = '\\'; break;
	    case 'n': data[j] = '\n'; break;
	    case '0': data[j] = '\0'; break;
	    default:
		free(data);
		return strdup("corrupt data");
	    }
	} else {
	    data[j] = data[i];
	}
    }

    play_one_buffer(data, j);
    free(data);

    return strdup(SERVER_OK);
}

int
main(int argc, char **argv)
{
    unsigned char *buffer;
    size_t size;
    audio_t *in;
    audio_config_t cfg;
    audio_meta_t meta;
    audio_device_t in_dev, out_dev;
    server_args_t server_args;
    pthread_t server_thread;

    pi_usb_init();
    if ((maestro = maestro_new()) == NULL) {
        fprintf(stderr, "couldn't find a recognized device, disabling skull.\n");
    } else {
        maestro_set_servo_is_inverted(maestro, SERVO_ID, true);
	maestro_set_servo_speed(maestro, SERVO_ID, 0);
    }

    piface = piface_new();

    pthread_mutex_init(&speak_lock, NULL);
    server_args.port = 5555;
    server_args.command = remote_event;
    server_args.state = NULL;

    pthread_create(&server_thread, NULL, server_thread_main, &server_args);

    audio_config_init_default(&cfg);
    audio_device_init_playback(&in_dev);
    audio_device_init_capture(&out_dev);

    out = audio_new(&cfg, &in_dev);
    in = audio_new(&cfg, &out_dev);

    if (! out) {
	perror("out");
	fprintf(stderr, "Failed to initialize playback\n");
	exit(1);
    }

    if (! in) {
	perror("in");
	fprintf(stderr, "Failed to initialize capture\n");
	exit(1);
    }

    audio_set_volume(in, 10);
    audio_set_volume(out, 100);

    size = audio_get_buffer_size(in);
    assert(audio_get_buffer_size(out) == size);

    buffer = fatal_malloc(size);

    audio_meta_init_from_config(&meta, &cfg);
    skull = talking_skull_new(&meta, false, servo_update, NULL);

    fprintf(stderr, "Copying from capture to play using %u byte buffers\n", size);

    while (audio_capture_buffer(in, buffer)) {
	play_one_buffer(buffer, size);
    }

    fprintf(stderr, "Failed to capture buffer!\n");

    audio_destroy(in);
    audio_destroy(out);
    free(buffer);

    return 0;
}

