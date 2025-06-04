
#pragma once
#include "Platform/Window.h"
#include "Gfx/IGfxSwapChain.h"
namespace RealSix
{
	class IGfxCommandBuffer;
	class IGfxDevice
	{
	public:
		IGfxDevice() = default;
		virtual ~IGfxDevice() = default;

		static IGfxDevice *Create(Window *window);

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual IGfxCommandBuffer *GetCurrentBackCommandBuffer() const = 0;

		IGfxSwapChain *GetSwapChain() const { return mSwapChain.get(); }
	protected:
		std::unique_ptr<IGfxSwapChain> mSwapChain;
	};
}