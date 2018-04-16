#pragma once

namespace terra
{
	class IUpdatable
	{
	public:
		IUpdatable() = default;
		virtual ~IUpdatable() = default;

		virtual bool Awake() = 0;
		virtual bool Init() = 0;
		virtual void PreUpdate() = 0;
		virtual void Update() = 0;
		virtual void PostUpdate() = 0;
		virtual void Exit() = 0;

		virtual bool CanUpdate() = 0;
		virtual void SetUpdatable(bool update) = 0;
	};

}