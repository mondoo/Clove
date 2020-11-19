#pragma once

#include <memory>

struct AppWrapper;

namespace wrapper {
    class Application {
        private:
            std::unique_ptr<AppWrapper> appWrapper;

        public:
            Application();
            ~Application();

            void shutdown();
            bool tick();
    };
}