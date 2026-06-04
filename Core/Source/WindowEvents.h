#pragma once

#include "Event.h"

#include <format>

namespace Rune {
	class Window;
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent(Window* window)
			: m_Window(window)
		{
		}


		EVENT_CLASS_TYPE(WindowClose)
		inline Window* WindowGet() const { return m_Window; }
	private:
		Window* m_Window;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(Window* window, uint32_t width, uint32_t height)
			: m_Window(window), m_Width(width), m_Height(height) 
		{
		}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			return std::format("WindowResizeEvent: {}, {}", m_Width, m_Height);
		}

		EVENT_CLASS_TYPE(WindowResize)
	private:
		uint32_t m_Width, m_Height;
		Window* m_Window;
	};
}