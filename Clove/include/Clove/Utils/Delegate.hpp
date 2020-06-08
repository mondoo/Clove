#pragma once

namespace clv::utl {
	struct DelegateHandle {
		//TYPES
	public:
		using IdType = int32_t;

		struct Binder {
			std::function<void(IdType)> unbind;
		};

		//VARIABLES
	private:
		static constexpr IdType INVALID_ID{ -1 };
		std::optional<IdType> id{};

		std::weak_ptr<Binder> binder;

		//FUNCTIONS
	public:
		DelegateHandle() = default;
		DelegateHandle(IdType id, std::weak_ptr<Binder> binder)
			: id(id)
			, binder(std::move(binder)) {
		}

		DelegateHandle(const DelegateHandle& other) = delete;
		DelegateHandle(DelegateHandle&& other) noexcept
			: id(std::move(other.id))
			, binder(std::move(other.binder)) {
		}

		DelegateHandle& operator=(const DelegateHandle& other) = delete;
		DelegateHandle& operator=(DelegateHandle&& other) {
			reset();
			id	   = std::move(other.id);
			binder = std::move(other.binder);
			return *this;
		}

		~DelegateHandle() {
			reset();
		}

		bool isValid() const {
			return id.has_value();
		}

		void reset() {
			if(auto lock = binder.lock()) {
				lock->unbind(getId());
			}
			id.reset();
		}

		IdType getId() const {
			return id.value_or(INVALID_ID);
		}
	};
}

namespace clv::utl {
	template<typename FunctionPrototype>
	class SingleCastDelegate {
		//VARIABLES
	private:
		std::function<FunctionPrototype> functionPointer;

		//FUNCTIONS
	public:
		SingleCastDelegate();

		SingleCastDelegate(const SingleCastDelegate& other) = delete;
		SingleCastDelegate(SingleCastDelegate&& other) noexcept;

		SingleCastDelegate& operator=(const SingleCastDelegate& other) = delete;
		SingleCastDelegate& operator=(SingleCastDelegate&& other) noexcept;

		~SingleCastDelegate();

		template<typename RetType, typename ObjectType, typename... Args>
		void bind(RetType (ObjectType::*function)(Args...), ObjectType* object);
		template<typename BindFunctionPrototype>
		void bind(BindFunctionPrototype&& function);

		void unbind();

		bool isBound() const;
		operator bool() const;

		template<typename... Args>
		auto broadcast(Args&&... args) const;
	};

	template<typename FunctionPrototype>
	class MultiCastDelegate {
		//VARIABLES
	private:
		std::shared_ptr<DelegateHandle::Binder> handleBinder;

		std::unordered_map<DelegateHandle::IdType, std::function<FunctionPrototype>> functionPointers;

		inline static DelegateHandle::IdType nextId = 0;

		//FUNCTIONS
	public:
		MultiCastDelegate();

		MultiCastDelegate(const MultiCastDelegate& other) = delete;
		MultiCastDelegate(MultiCastDelegate&& other) noexcept;

		MultiCastDelegate& operator=(const MultiCastDelegate& other) = delete;
		MultiCastDelegate& operator=(MultiCastDelegate&& other) noexcept;

		~MultiCastDelegate();

		template<typename RetType, typename ObjectType, typename... Args>
		[[nodiscard]] DelegateHandle bind(RetType (ObjectType::*function)(Args...), ObjectType* object);
		template<typename BindFunctionPrototype>
		[[nodiscard]] DelegateHandle bind(BindFunctionPrototype&& function);

		void unbind(DelegateHandle::IdType id);
		void unbindAll();

		template<typename... Args>
		void broadcast(Args&&... args) const;
	};
}

#include "Delegate.inl"