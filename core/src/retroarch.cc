#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "libretro.h"
#include "machine.h"

const   int         CPU_FREQ     = 4000000;
const   int         FPS          = 72;
const   uint16_t    INPUT_CART_N = 0b1000000000;

static  Machine::State          machine_state;
static  uint8_t                 cartridge_memory[0x200000];
static  uint32_t                framebuffer[64*96];
static  uint16_t                input_state;
static  int                     insert_countdown;

// Callbacks
static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;

unsigned retro_api_version(void) { return RETRO_API_VERSION; }

// Cheats
void retro_cheat_reset(void) {}
void retro_cheat_set(unsigned index, bool enabled, const char *code) {}

// Load a cartridge
bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) { return false; }

bool retro_load_game(const struct retro_game_info *info)
{
    update_inputs(machine_state, input_state |= INPUT_CART_N);

    if (info && info->data) { // ensure there is ROM data
        const uint8_t* data = (const uint8_t*)info->data;
        int offset = 0;

        if (data[0] == 'M' && data[1] == 'N') {
            offset = 0x2100;
        } else {
            offset = 0;
        }

        // Note: this does not properly handle
        for (int i = 0; i < info->size; i++) {
            cartridge_memory[(i + offset) % sizeof(cartridge_memory)] = data[i];
        }

        insert_countdown = 2;
    }

    return true;
}

// Unload the cartridge
void retro_unload_game(void) { 
    // TODO: CLEAR CART INSERT FALG
    memset(cartridge_memory, 0xFF, sizeof(cartridge_memory));
}

extern "C" void debug_print(const void* data) {
    log_cb(RETRO_LOG_DEBUG, (const char*) data);
}

extern "C" void flip_screen(void* lcd) {
    uint8_t* pixels = (uint8_t*)lcd;
    uint32_t* out = framebuffer;

    memcpy(framebuffer, lcd, sizeof(framebuffer));
}

extern "C" uint8_t cpu_read_cart(Machine::State& cpu, uint32_t address) {
    return cartridge_memory[address % sizeof(cartridge_memory)];
}

extern "C" void cpu_write_cart(Machine::State& cpu, uint8_t data, uint32_t address) {
}


unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }

// libretro unused api functions
void retro_set_controller_port_device(unsigned port, unsigned device) {}


void *retro_get_memory_data(unsigned id) { return NULL; }
size_t retro_get_memory_size(unsigned id){ return 0; }

// Serialisation methods
size_t retro_serialize_size(void) { return 0; }
bool retro_serialize(void *data, size_t size) { return false; }
bool retro_unserialize(const void *data, size_t size) { return false; }

// End of retrolib
void retro_deinit(void) {}

// libretro global setters
void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;
    bool no_rom = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {}
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

void retro_init(void)
{
    /* set up some logging */
    struct retro_log_callback log;
    unsigned level = 4;

    if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
        log_cb = log.log;
    else
        log_cb = NULL;

    // the performance level is guide to frontend to give an idea of how intensive this core is to run
    environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);

    insert_countdown = 0;
    update_inputs(machine_state, input_state = 0b1111111111);
    cpu_reset(machine_state);
}


/*
 * Tell libretro about this core, it's name, version and which rom file types it supports.
 */
void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name = "Minimon";
    info->library_version = "0.1.0";
    info->need_fullpath = false;
    info->valid_extensions = "bin|min"; // file types supported
}

/*
 * Tell libretro about the AV system; the fps, sound sample rate and the
 * resolution of the display.
 */
void retro_get_system_av_info(struct retro_system_av_info *info) {
    memset(info, 0, sizeof(*info));
    info->timing.fps            = FPS;
    info->timing.sample_rate    = 44100;    // 44.1kHz
    info->geometry.base_width   = 96;
    info->geometry.base_height  = 64;
    info->geometry.max_width    = 96;
    info->geometry.max_height   = 64;
    info->geometry.aspect_ratio = 96.0f / 64.0f;

    retro_pixel_format pixel_format = RETRO_PIXEL_FORMAT_XRGB8888;
    environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixel_format);
}

void retro_reset(void)
{
    cpu_reset(machine_state);
}

void retro_run(void)
{
    if (insert_countdown > 0 && --insert_countdown) {
        update_inputs(machine_state, input_state &= ~INPUT_CART_N);
    }

    cpu_advance(machine_state, CPU_FREQ / FPS);
    // TODO: FEED AUDIO
    video_cb(framebuffer, 96, 64, 64 * sizeof(uint32_t));
}
