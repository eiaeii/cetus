#pragma once

namespace terra
{
	class IModule
	{
	public:
		IModule() = default;
		virtual ~IModule() = default;

		virtual bool Running() const { return true; }
		virtual void Stop(bool run) {}

		virtual bool Awake() = 0;
		virtual bool Init() = 0;
		virtual void Exit() = 0;
		virtual void Destroy() = 0;


		virtual void PreUpdate() = 0;
		virtual void Update() = 0;
		virtual void PostUpdate() = 0;
	};

}