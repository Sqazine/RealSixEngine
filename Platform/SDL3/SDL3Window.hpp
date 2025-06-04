#pragma once
#include "String.hpp"

#include <SDL3/SDL.h>
#include "Math/Vector2.hpp"
#include "Core/Logger.hpp"
#include "Platform/Window.hpp"
namespace RealSix
{
	class SDL3Window : public Window
	{
	public:
		SDL3Window();
		~SDL3Window() override;

		void SetTitle(StringView str) override;
		StringView GetTitle() const override;
		void Resize(const Vector2u32 &extent) override;
		void Resize(uint32_t w, uint32_t h) override;
		Vector2u32 GetSize() override;
		float GetAspect() override;

		SDL_Window *GetHandle() const;

		void Show() override;
		void Hide() override;

	private:
		SDL_Window *mHandle;
		String mTitle = "RealSix";

		bool mIsShown;
	};

}