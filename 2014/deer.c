#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pi-usb.h"
#include "maestro.h"
#include "piface.h"
#include "talking-skull.h"
#include "wav.h"

#define SERVO_ID 0
#define SERVO_MIN 20
#define SERVO_MAX 90

#define EYES 7
#define INTER_SONG_MS 2000

static double gain = 1;
static maestro_t *maestro;
static piface_t *piface;

static void
update_servo(void *unused, double pos)
{
    pos *= gain;
    if (pos > 100) pos = 100;
    maestro_set_servo_pos(maestro, SERVO_ID, pos * (SERVO_MAX - SERVO_MIN) / 100 + SERVO_MIN);
    piface_set(piface, EYES, pos > 50);
}

int
main(int argc, char **argv)
{
    wav_t *song, *servo;
    audio_meta_t meta_servo;
    audio_config_t audio_cfg;
    audio_device_t audio_dev;
    audio_t *audio;
    talking_skull_t *talking_skull;
    unsigned char *servo_data;
    size_t n_servo_data;

    pi_usb_init();

    piface = piface_new();
    if ((maestro = maestro_new()) == NULL) {
	fprintf(stderr, "couldn't find a recognized device.\n");
	exit(1);
    }

    maestro_set_servo_is_inverted(maestro, SERVO_ID, 1);

    while (argc > 1 && argv[1][0] == '-' && argv[1][1] == '-') {
	if (strcmp(argv[1], "--") == 0) {
	    argc -= 1;
	    argv += 1;
	    break;
	}
	if (argc > 2 && strcmp(argv[1], "--gain") == 0) {
	    gain = atof(argv[2]);
	    argc -= 2;
	    argv += 2;
	} else {
	    fprintf(stderr, "usage: [--gain X] audio.wav servo.wav\n");
	}
    }

    song = wav_new(argv[1]);
    if (! song) {
	perror(argv[1]);
	exit(1);
    }

    servo = wav_new(argv[2]);
    if (! servo) {
	perror(argv[2]);
	exit(1);
    }

    meta_servo = wav_get_meta(servo);
    servo_data = wav_get_raw_data(servo, &n_servo_data);
    talking_skull = talking_skull_new(&meta_servo, false, update_servo, NULL);

    audio_device_init_playback(&audio_dev);
    audio_config_init_default(&audio_cfg);
    wav_configure_audio(song, &audio_cfg);

    printf("Config from wav file: ");
    audio_config_print(&audio_cfg, stdout);
    printf("\n");

    audio = audio_new(&audio_cfg, &audio_dev);

    if (! audio) {
	perror("audio_new");
	exit(1);
    }

    audio_set_volume(audio, 100);

    while (true) {
	talking_skull_play(talking_skull, servo_data, n_servo_data);
	wav_play(song, audio);
	ms_sleep(INTER_SONG_MS);
    }

    return 0;
}