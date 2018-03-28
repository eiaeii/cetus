#pragma once

namespace terra
{
	class IModule
	{
	public:
		IModule() = default;
		virtual ~IModule() = default;

		virtual void Awake() = 0;
		virtual void Init() = 0;
		virtual void PreUpdate() = 0;
		virtual void Update() = 0;
		virtual void PostUpdate() = 0;
		virtual void Exit() = 0;
		virtual void Destroy() = 0;
	};

}