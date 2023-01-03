#include <stdint.h>

static uint32_t maximize_dark20Width = 20;
static uint32_t maximize_dark20Height = 20;
static uint32_t maximize_dark20BPP = 4;

static uint8_t maximize_dark20Pixels[] = {
    80,  255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 50,  255, 255, 255, 209, 255, 255, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    254, 255, 255, 255, 208, 255, 255, 255, 50,  255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 208, 255, 255, 255, 227, 255, 255, 255, 138, 255,
    255, 255, 136, 255, 255, 255, 136, 255, 255, 255, 136, 255, 255, 255, 136, 255, 255, 255, 136, 255, 255, 255, 136,
    255, 255, 255, 136, 255, 255, 255, 136, 255, 255, 255, 138, 255, 255, 255, 227, 255, 255, 255, 208, 255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 253, 255, 255, 255, 131, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 131, 255, 255, 255, 253, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 255, 255, 255, 255, 128, 255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 128, 255, 255, 255, 255, 255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    255, 255, 255, 255, 128, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 128, 255, 255, 255, 255, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 255, 255, 255, 255, 128, 255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 128, 255, 255, 255, 255, 255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 255,
    255, 255, 255, 128, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 128, 255, 255, 255, 255, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 255, 255, 255, 255, 128, 255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 128, 255, 255, 255, 255, 255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 255, 255,
    255, 255, 128, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    128, 255, 255, 255, 255, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 255, 255, 255, 255, 128, 255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 128, 255, 255, 255, 255, 255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 255, 255, 255,
    255, 128, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 128,
    255, 255, 255, 255, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 253, 255, 255, 255, 131, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 131, 255, 255, 255, 253, 255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 208, 255, 255, 255,
    227, 255, 255, 255, 125, 255, 255, 255, 119, 255, 255, 255, 119, 255, 255, 255, 119, 255, 255, 255, 119, 255, 255,
    255, 119, 255, 255, 255, 119, 255, 255, 255, 119, 255, 255, 255, 119, 255, 255, 255, 125, 255, 255, 255, 227, 255,
    255, 255, 208, 255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 50,  255, 255, 255, 208, 255, 255, 255, 254, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 254, 255, 255, 255, 208, 255, 255, 255, 49,  255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255,
    255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255,
    255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,
    255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
    0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0};
