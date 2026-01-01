#include "App.hpp"
#include "Core/Logger.hpp"
#include "Core/Config.hpp"
#include "Platform/PlatformInfo.hpp"

namespace RealSix
{
	void App::Run()
	{
		SetupConfig();
		Init();
		while (mState != AppState::QUIT)
		{
			PreTick();
			if (mState == AppState::TICK)
			{
				Tick();
				Draw();
			}
			PostTick();
		}
		CleanUp();
	}

	void App::Quit()
	{
		mState = AppState::QUIT;
	}

	Window *App::GetWindow() const
	{
		return mWindow.get();
	}

	InputSystem *App::GetInputSystem() const
	{
		return mInputSystem.get();
	}

	Renderer *App::GetRenderer() const
	{
		return mRenderer.get();
	}

	void App::SetupConfig()
	{
		// Setup default config
		AppConfig::GetInstance().SetRefreshOnlyWindowIsActive(true);
		GfxConfig::GetInstance().SetBackend(GfxBackend::VULKAN).SetVSync(false).SetBackBufferCount(GfxBackBufferCount::TWO).SetMsaa(Msaa::X1);
	}

	void App::Init()
	{
		PlatformInfo::GetInstance().Init();

		mWindow.reset(Window::Create());
		mWindow->Show();

		mInputSystem.reset(InputSystem::Create());

		mRenderer = std::make_unique<Renderer>(mWindow.get());
	}

	void App::Tick()
	{
	}

	void App::Render()
	{
		mRenderer->Render();
	}

	void App::RenderGizmo()
	{
	}

	void App::RenderUI()
	{
	}

	void App::CleanUp()
	{
		PlatformInfo::GetInstance().CleanUp();
	}

	void App::PreTick()
	{
		mInputSystem->PreTick(mWindow.get());

		if (AppConfig::GetInstance().IsRefreshOnlyWindowIsActive() && GetWindow()->HasEvent(Window::Event::MIN))
			mState = AppState::PAUSE;
		if (mState == AppState::PAUSE && GetWindow()->HasEvent(Window::Event::ENTER | Window::Event::EXPOSE))
			mState = AppState::TICK;
	}

	void App::PostTick()
	{
		if (mWindow->HasEvent(Window::Event::CLOSE))
			Quit();

		mWindow->ClearEvent();
		mInputSystem->PostTick();
	}

	void App::Draw()
	{
		mRenderer->BeginFrame();
		Render();
		RenderGizmo();
		RenderUI();
		mRenderer->EndFrame();
	}
}