#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "wb.h"

static char buf[1024];

static int forward[2][2] = {{ -1, -1 }, { 6, 7 }};
static int backward[2][2] = { { -1, -1 }, {5, 8 }};

static void set(int *on, int *off, float duty)
{
    wb_pwm(WB_OUTPUT(1, off[0]), 0);
    wb_pwm(WB_OUTPUT(1, off[1]), 0);
    wb_pwm(WB_OUTPUT(1, on[0]), duty);
    if (duty == 1) ms_sleep(1);	/* temp hack until I get a bigger power supply? */
    wb_pwm(WB_OUTPUT(1, on[1]), duty);
}

static void
wait_for_trigger(void)
{
    if (fgets(buf, sizeof(buf), stdin) == NULL || feof(stdin)) {
	fprintf(stderr, "EOF\n");
	exit(1);
    }
}

static void
wait_until_middle(void)
{
	ms_sleep(3700);
}

int
main(int argc, char **argv)
{
    if (wb_init() < 0) {
	fprintf(stderr, "Failed to initialize wb\n");
	exit(1);
    }

    set(backward[1], forward[1], 0.25);
    wait_until_start_position();

    while (true) {
	wait_for_trigger();
	track_play(trumpet);
	set(forward[1], backward[1], 1);
	play_asynchronously(gallop);
	wait_until_middle();
	cancel_async_playback();
	track_play(crash);
	set(forward[1], backward[1], .25);
	play_asynchronously(trot);
	ms_sleep(TROT_MS);
	/* gradual deceleration needed here? */
	set(forward[1], backward[1], 0);
	ms_sleep(100);
	play_asynchronously(beep);
	set(backward[1], forward[1], 0.35);
	wait_until_start_position();
	set(forward[1], backward[1], 0);
    }

    return 0;
}
