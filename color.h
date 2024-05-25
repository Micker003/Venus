#include <stdbool.h>
#include <stdint.h>
#include <libpynq.h>

void color_init(const io_t pin_out, const io_t pin_s0, const io_t pin_s1,const io_t pin_s2, const io_t pin_s3 );

char color_read(const io_t pin_out, const io_t pin_s2, const io_t pin_s3 );