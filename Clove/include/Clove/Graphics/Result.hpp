#pragma once

namespace clv::gfx {
    //TODO: Have Success and Error then OR those with other codes so you can just check for succes or error
    enum class Result {
        Success,
        Success_SwapchainSuboptimal,

        Error_SwapchainOutOfDate,

        Unkown
    };
}