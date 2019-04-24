#pragma once

#include "Event.hpp"

namespace clv{
	class WindowResizeEvent : public Event{
		//VARIABLES
	private:
		unsigned int width = 0;
		unsigned int height = 0;

		//FUNCTIONS
	public:
		CLV_API WindowResizeEvent(unsigned int inWidth, unsigned int inHeight);

		CLV_API inline unsigned int getWidth() const;
		CLV_API inline unsigned int getHeight() const;

		CLV_API virtual std::string toString() const override;

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
	};

	class WindowCloseEvent : public Event{
		//FUNCTIONS
	public:
		CLV_API WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
	};

	class AppTickEvent : public Event{
		//FUNCTIONS
	public:
		CLV_API AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
	};

	class AppUpdateEvent : public Event{
		//FUNCTIONS
	public:
		CLV_API AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
	};

	class AppRenderEvent : public Event{
		//FUNCTIONS
	public:
		CLV_API AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
	};
}