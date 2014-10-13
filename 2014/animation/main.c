#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "gpio.h"
#include "piface.h"
#include "server.h"
#include "util.h"
#include "animation/lights.h"

#define N_BUTTONS 5
#define N_GPIOS   5

#define SPIDER_ID   0
#define ZOMBIE_ID   1
#define GATER_ID    2
#define SNAKE_ID    3
#define SNAKE_SPIT_ID 4
#define QUESTION_ID 4

#define SPIDER      "spider"
#define ZOMBIE      "zombie"
#define GATER       "gater"
#define SNAKE       "snake"
#define SNAKE_SPIT  "snake-spit"

static gpio_table_t gpio_table[N_GPIOS] = {
    [SPIDER_ID]      = { SPIDER,      5, 1 },
    [ZOMBIE_ID]      = { ZOMBIE,      6, 1 },
    [GATER_ID]       = { GATER,      13, 1 },
    [SNAKE_ID]       = { SNAKE,      19, 1 },
    [SNAKE_SPIT_ID]  = { SNAKE_SPIT, 26, 1 },
};

#define N_GPIO_TABLE (sizeof(gpio_table) / sizeof(gpio_table[0]))

static lights_t *lights;
static piface_t *piface;
static gpio_t *gpio;
static pthread_mutex_t event_lock;

static void
do_attack(unsigned id)
{
printf("attacking on %d\n", id);
}

static void
do_popup(unsigned id)
{
printf("popup on %d\n", id);
}

static void
do_snake(void)
{
printf("snake\n");
}

static void
do_question(void)
{
printf("question\n");
}

static void
do_prop(unsigned id)
{
    switch (id) {
    case SPIDER_ID: do_attack(id); break;
    case ZOMBIE_ID: do_popup(id); break;
    case GATER_ID:  do_attack(id); break;
    case SNAKE_ID:  do_snake(); break;
    case QUESTION_ID: do_question(); break;
    }
}

static void
handle_event(unsigned i)
{
    pthread_mutex_lock(&event_lock);
    lights_select(lights, i);
    do_prop(i);
    pthread_mutex_unlock(&event_lock);
}

static void
remote_event(void *unused, const char *command)
{
    if (strcmp(command, SPIDER) == 0) {
	do_prop(SPIDER_ID);
    } else if (strcmp(command, ZOMBIE) == 0) {
	do_prop(ZOMBIE_ID);
    } else if (strcmp(command, GATER) == 0) {
	do_prop(GATER_ID);
    } else if (strcmp(command, SNAKE) == 0) {
	do_prop(SNAKE_ID);
    } else if (strcmp(command, "question") == 0) {
	do_prop(QUESTION_ID);
    } else {
	fprintf(stderr, "Invalid net command: [%s]\n", command);
    }
}

int
main(int argc, char **argv)
{
    unsigned i, button;
    server_args_t server_args;
    pthread_t server_thread;

    piface = piface_new();
    lights = lights_new(piface);
    gpio = gpio_new(gpio_table, N_GPIO_TABLE);

    pthread_mutex_init(&event_lock, NULL);

    server_args.port = 5555;
    server_args.command = remote_event;
    server_args.state = NULL;

    pthread_create(&server_thread, NULL, server_thread_main, &server_args);

    while (true) {
	lights_chase(lights);
	button = piface_wait_for_input(piface);
	for (i = 0; i < N_BUTTONS; i++) {
	    if (PIFACE_IS_SELECTED(button, i)) {
		handle_event(i);
	    }
	}
	ms_sleep(1000);
    }

    pthread_mutex_destroy(&event_lock);
    lights_destroy(lights);
    piface_destroy(piface);

    return 0;
}
