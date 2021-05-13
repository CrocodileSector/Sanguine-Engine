#pragma once

#include "imgui.h"

#include "Sanguine/Core/Layer.h"
#include "Sanguine/Events/Event.h"
#include "Sanguine/Events/MouseEvent.h"
#include "Sanguine/Events/KeyEvent.h"
#include "Sanguine/Events/AppEvent.h"

namespace Sanguine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach()		 override;
		void OnDetach()		 override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetDarkTheme();
		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = false;
		float m_Time;
	};
}