#pragma once
#include <string>
#include <string_view>
#include <SDL3/SDL.h>
#include "Math/Vector2.h"
#include "Logger/Logger.h"
#include "Platform/Window.h"
namespace RealSix
{
	class SDL3Window : public Window
	{
	public:
		SDL3Window();
		~SDL3Window() override;

		void SetTitle(std::string_view str) override;
		std::string_view GetTitle() const override;
		void Resize(const Vector2u32 &extent) override;
		void Resize(uint32_t w, uint32_t h) override;
		Vector2u32 GetSize() override;
		float GetAspect() override;

		SDL_Window *GetHandle() const;

		void Show() override;
		void Hide() override;

	private:
		SDL_Window *mHandle;
		std::string mTitle = "RealSix";

		bool mIsShown;
	};

}