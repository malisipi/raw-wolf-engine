#define MINIAUDIO_IMPLEMENTATION
#include <thirdparty/miniaudio.h>

namespace raw_wolf::audio {
    void audio_manager_data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
        ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
        if (pDecoder == NULL) {
            return;
        }
        ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    };

    typedef class audio_manager {
        public:
        ma_decoder decoder;
        ma_device device;
        const int OK = 0;
        const int FAILED = -1;
        char* file;

        int load_file(char* file){
            this->file = file;
            if (ma_decoder_init_file(file, NULL, &decoder) != MA_SUCCESS){
                std::cout << "\"" << file << "\" file cannot opened. File may unsupported or not exist.\n";
                return FAILED;
            }

            ma_device_config device_config = ma_device_config_init(ma_device_type_playback);
            device_config.playback.format   = decoder.outputFormat;
            device_config.playback.channels = decoder.outputChannels;
            device_config.sampleRate        = decoder.outputSampleRate;
            device_config.dataCallback      = audio_manager_data_callback;
            device_config.pUserData         = &decoder;

            if (ma_device_init(NULL, &device_config, &device) != MA_SUCCESS) {
                std::cout << "Failed to open playback device while trying to open \"" << file << "\" file\n";
                ma_decoder_uninit(&decoder);
                return FAILED;
            }
            return OK;
        };

        void free(){
            ma_device_uninit(&device);
            ma_decoder_uninit(&decoder);
        };

        int play(){
            if(ma_device_start(&device) != MA_SUCCESS) {
                std::cout << "Failed to start playback device while trying to open \"" << file << "\" file\n";
                free();
                return FAILED;
            }
            return OK;
        };

        int pause(){
            if(ma_device_stop(&device) != MA_SUCCESS) {
                std::cout << "Failed to stop playback device while trying to open \"" << file << "\" file\n";
                free();
                return FAILED;
            }
            return OK;
        }

        void set_loop(bool looped){
            if(looped){
                ma_data_source_set_looping(&decoder, MA_TRUE);
            } else {
                ma_data_source_set_looping(&decoder, MA_FALSE);
            };
        };
    } manager;
}
