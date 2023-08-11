#include "main.h"

void publish_auth(char *version, char *mac_address, char *series, char *subtype, char *xtal_freq);
void publish_system();
void publish_capabilities();
void publish_error_state(char *error, bool state);
void publish_settings();
void publish_errors();
void publish_log(char *log);
void publish_temp_log();
void publish_extra_options();
void publish_firmware_state(char *state);
