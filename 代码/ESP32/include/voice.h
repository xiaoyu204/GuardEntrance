#pragma once
#include <HTTPClient.h>
#include <SdFat.h>
#include "cmd.h"
#include "api.h"
#include "record.h"
#include "mymqtt.h"

#define SOUND_THRESHOLD 3000 // 根据实际环境调整阈值

class TokenAPI : private API
{
private:
    String API_KEY    = "MeWnMuFsysfU6vBABXM7F4T0";
    String SECRET_KEY = "e33NcaPRX5bDyK09PUPHlOhdMP1JfA6s";

    virtual void parseInfo(Client &client) override;

public:
    const char *access_token;

    TokenAPI(const char *host, int port) : API(host, port) {};
    void getToken();
};

class VoiceAPI
{
private:
    TokenAPI      tokenAPI;
    Record        rec;
    SdFs         &sd;
    Attributes_T &attributes;

public:
    VoiceAPI(const char *host, int port, SdFs &sd, Attributes_T &attributes)
        : tokenAPI(host, port), sd(sd), attributes(attributes)
    {
    }
    bool        isSomeoneApproach();
    void        parseInfo(String response);
    void        getIdentification(SdFs &sd, const char *wavFilePath);
    void        sendExecuteCMD(String identification);
    void        keepListening();
    inline void leaveMessage(const char *messageFile);
};
