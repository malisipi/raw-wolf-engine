#ifdef RAW_WOLF_MULTI_USER
#define CROW_MAIN
    #include "thirdparty/crow_all.h"
    #define RAW_WOLF_NETWORKING
#endif

#ifdef RAW_WOLF_NETWORKING
    #include <curl/curl.h>

    namespace raw_wolf::networking {
        namespace {
            size_t WriteCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
                data->append(ptr, size*nmemb);
                return size * nmemb;
            }
        }

        std::string fetch(const std::string &url) {
            CURL* curl = curl_easy_init();
            std::string data;

            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

                CURLcode res = curl_easy_perform(curl);
                if (res != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                    return (std::string)"";
                }

                curl_easy_cleanup(curl);
            }

            return data;
        }

        #ifdef RAW_WOLF_MULTI_USER
            namespace routing {
                crow::SimpleApp router;
                bool is_alive = false;
                int port = 5000;

                #define raw_wolf_networking_routing_handle(url, handler) CROW_ROUTE(raw_wolf::networking::routing::router, url)(handler);

                void run(int port){
                    raw_wolf::networking::routing::port = port;
                    raw_wolf::networking::routing::is_alive = true;
                    raw_wolf::networking::routing::router.port(raw_wolf::networking::routing::port).run();
                    raw_wolf::networking::routing::is_alive = false;
                };

                #define raw_wolf_networking_routing_run(__port__) std::thread raw_wolf_networking_routing_thread ([](){raw_wolf::networking::routing::run(__port__);})
            }
        #endif

    }
#endif
