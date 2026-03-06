#pragma once

/* Build full telemetry JSON
   Returns malloc'ed string (must free after publish)
*/
char *build_device_json(void);