#ifndef __VMUSIC2_C__
#define __VMUSIC2_C__

#undef SERIAL_FUNCTION_NAME_PREFIX
#define SERIAL_FUNCTION_NAME_PREFIX vmusic2_
#include "serial.c"
#undef SERIAL_FUNCTION_NAME_PREFIX

#define VMUSIC2_STOP_CMD "VST\r"
#define VMUSIC2_PING_CMD "\r"

#define VMUSIC2_PING_CMD ""
#define VMUSIC2_STOP_CMD "VST"
#define VMUSIC2_PAUSE_CMD "VP"
#define VMUSIC2_RESUME_CMD "VP"

void
vmusic2_cmd(const char *cmd, int wait_for_prompt)
{
    vmusic2_serial_tx_string(cmd);
    vmusic2_serial_tx_byte(13);
    if (wait_for_prompt) {
	while (vmusic2_serial_rx_byte() != '>') {}
    }
}

void vmusic2_play(const char *, int);

void
vmusic2_init(void)
{
    vmusic2_serial_init();
    delay_loop(4000);
vmusic2_play("0", 1);
    vmusic2_cmd(VMUSIC2_PING_CMD, 1);
    vmusic2_cmd(VMUSIC2_STOP_CMD, 1);
}

void
vmusic2_volume(uns8 volume, int wait_for_prompt)
{
    if (volume > 0xfe) volume = 0xfe;
    vmusic2_serial_tx_string("VSV $");
    vmusic2_serial_tx_hex(volume);
    if (wait_for_prompt) {
	while (vmusic2_serial_rx_byte() != '>') {}
    }
}

void
vmusic2_wait_for_prompt()
{
    while (vmusic2_serial_rx_byte() != '>') {}
}

void
vmusic2_play(const char *base_name, int wait_for_start)
{
    vmusic2_serial_tx_string("VPF ");
    vmusic2_serial_tx_string(base_name);
    vmusic2_serial_tx_string(".MP3\r");
    if (wait_for_start) {
	char byte;

wfs_0:
	while (vmusic2_serial_rx_byte() != 'T') {}
wfs_1:
   	byte = vmusic2_serial_rx_byte();
	if (byte == 'T') goto wfs_1;
	if (byte != ' ') goto wfs_0;
	byte = vmusic2_serial_rx_byte();
	if (byte == 'T') goto wfs_1;
	if (byte != '$') goto wfs_0;
    }
}

#endif
