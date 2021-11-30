#include <type_traits>
#include <typeinfo>

namespace clove {
    template<typename GeometryPassType>
    GeometryPass::Id GeometryPass::getId() {
        static_assert(std::is_base_of_v<GeometryPass, GeometryPassType>, "Type passed is not derived from GeometryPass!");
        return typeid(GeometryPassType).hash_code();
    };

    void GeometryPass::addJob(Job *job) {
        jobs.emplace_back(job);
    }

    void GeometryPass::flushJobs() {
        jobs.clear();
    }

    std::vector<GeometryPass::Job *> const &GeometryPass::getJobs() const {
        return jobs;
    }
}