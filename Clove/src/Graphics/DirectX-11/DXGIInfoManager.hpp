#pragma once

#include <wrl.h>

struct IDXGIInfoQueue;

namespace clv{
	namespace gfx{
		class DXGIInfoManager{
			//VARIABLES
		private:
			unsigned long long next = 0u;
			Microsoft::WRL::ComPtr<IDXGIInfoQueue> DXGIInfoQueue;

			//FUNCTIONS
		public:
			DXGIInfoManager();
			DXGIInfoManager(const DXGIInfoManager* other) = delete;
			DXGIInfoManager(DXGIInfoManager&& other) noexcept;
			DXGIInfoManager& operator=(const DXGIInfoManager& other) = delete;
			DXGIInfoManager& operator=(DXGIInfoManager&& other) noexcept;
			~DXGIInfoManager();

			void set() noexcept;
			std::vector<std::string> getMessages() const;
		};
	}
}