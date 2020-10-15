#include "Root/Delegate/DelegateHandle.hpp"

namespace garlic::inline root {
    DelegateHandle::DelegateHandle() = default;

    DelegateHandle::DelegateHandle(IdType id, std::weak_ptr<Proxy> proxy)
        : id(id)
        , proxy(std::move(proxy)) {
    }

    DelegateHandle::DelegateHandle(DelegateHandle&& other) noexcept = default;

    DelegateHandle& DelegateHandle::operator=(DelegateHandle&& other) noexcept {
        if(isValid()) {
            reset();
        }

        id    = std::move(other.id);
        proxy = std::move(other.proxy);
        return *this;
    }

    DelegateHandle::~DelegateHandle() {
        reset();
    }

    bool DelegateHandle::isValid() const {
        return id.has_value() && !proxy.expired();
    }

    void DelegateHandle::reset() {
        if(auto proxyLock = proxy.lock()) {
            proxyLock->unbind(*this);
        } else {
            id.reset();
        }
    }
}