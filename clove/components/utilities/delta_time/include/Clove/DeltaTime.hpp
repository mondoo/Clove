#pragma once

namespace garlic::clove {
    class DeltaTime {
        //VARIABLES
    private:
        float deltaSeconds{ 0.0f };

        //FUNCTIONS
    public:
        DeltaTime();
        DeltaTime(float deltaSeconds);

        DeltaTime(DeltaTime const &other);
        DeltaTime(DeltaTime &&other) noexcept;

        DeltaTime &operator=(DeltaTime const &other);
        DeltaTime &operator=(DeltaTime &&other) noexcept;

        ~DeltaTime();

        float getDeltaSeconds() const;

        operator float() const;
    };
}