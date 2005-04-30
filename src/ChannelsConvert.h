#ifndef __CHANNELS_CONVERT_H__
#define __CHANNELS_CONVERT_H__

#include "Channels.h"

namespace Gil {

    template<Channels FromChannel, Channels ToChannel>
    struct ChannelsConvert {
    };

    template<>
    struct ChannelsConvert<OneChannel, >

}

#endif
