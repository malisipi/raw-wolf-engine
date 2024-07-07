// You need to define DISCORD_CLIENT_ID before including <raw_wolf_engine.hpp>
// If you not, Raw Wolf Engine will disable Discord Rich Presence support
// #define DISCORD_CLIENT_ID <client-id>
//
// However Discord Rich Presence will still need to setted by game.
// A code example is placed in below about how to set it by hand.
//
//   raw_wolf::discord::set_rich_presence([](struct DiscordActivity* activity){
//       sprintf(activity->details, "They are details");
//       sprintf(activity->state, "They are state");
//       sprintf(activity->assets.large_image, "large_image");
//       sprintf(activity->assets.large_text, "Large Image Description");
//   });
//
// Also if you use the header from raw_wolf_engine (not as separated),
// you must to call the set_rich_presence function after `raw_wolf::init();`
// Because of `raw_wolf::init();` will also initalize the Discord Rich Presence
//
#include "discord_game_sdk.h"

#define DISCORD_REQUIRE(x) ({\
    if(x != DiscordResult_Ok){\
        raw_wolf::discord::alive = false;\
        printf("Discord Loop was finished\n");\
    }\
});

namespace raw_wolf::discord {
    bool alive = true;
    struct IDiscordCore* core;
    struct IDiscordActivityManager* activities;
    struct IDiscordApplicationManager* application;
    struct DiscordActivity activity;

    void init(DiscordClientId client_id){
        struct DiscordCreateParams params;
        DiscordCreateParamsSetDefault(&params);
        params.client_id = client_id;
        params.flags = DiscordCreateFlags_NoRequireDiscord;
        DISCORD_REQUIRE(DiscordCreate(DISCORD_VERSION, &params, &core));

        if(alive){
            activities = core->get_activity_manager(core);
            application = core->get_application_manager(core);

            // Init activity here
            memset(&activity, 0, sizeof(activity));
        }
    }

    void set_rich_presence(std::function<void(struct DiscordActivity*)> activity_setter){
        if(alive){
            activity_setter(&activity);
            activities->update_activity(activities, &activity, NULL, [](void* data, enum EDiscordResult result){
                DISCORD_REQUIRE(result);
            });
        }
    }

    void loop(){
        if(alive){
            DISCORD_REQUIRE(core->run_callbacks(core));
        }
    }
}
