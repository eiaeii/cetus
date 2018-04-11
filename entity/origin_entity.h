#pragma once

#include "core.h"
#include "origin_module_interface.h"

namespace terra
{
	class OriginComponent;
    class OriginEntity : public IModule
    {
    private:
        FGuid guid_;
		bool running_;
        std::unordered_map<int, std::unique_ptr<OriginComponent>> components_;
        static OriginEntity* default_instance_;
	public:
		static OriginEntity* Get();
		static void Destroy();

    public:
        OriginEntity(const FGuid& unique_id);
        virtual ~OriginEntity();

		virtual bool Awake() override;
		virtual bool Init() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void Exit() override;

		bool IsRunning() const { return running_; }
		void StopRunning() { running_ = false; }

        FGuid GetEntityID() const { return guid_; }

        template <typename C, typename... Args>
        auto Add(Args&&... args) -> OriginEntity&;

        template <typename Arg>
        auto Remove() -> OriginEntity&;
        template <typename Arg0, typename... Args>
        auto Remove() -> typename std::enable_if<sizeof...(Args) != 0, OriginEntity&>::type;

        template <typename C, typename... Args>
        auto Replace(Args&&... args) -> OriginEntity&;

        template <typename C>
        auto Get() const -> C*;
        template <typename... Args>
        auto Get() const -> typename std::enable_if<(sizeof...(Args) != 1), std::tuple<Args*...>>::type;

        template <typename Arg>
        bool Has() const;
        template <typename Arg0, typename... Args>
        auto Has() const -> typename std::enable_if<sizeof...(Args) != 0, bool>::type;

        OriginComponent* GetComponent(const int idx) const;

    private:
        OriginEntity& AddComponent(const int idx, std::unique_ptr<OriginComponent> component);
        OriginEntity& RemoveComponent(const int idx);
        OriginEntity& ReplaceComponent(const int idx, std::unique_ptr<OriginComponent> component);
        bool HasComponent(const int idx) const;
        void ReplaceWith(const int idx, std::unique_ptr<OriginComponent> component);

        void DestroyAllComponent();
    };

    template <typename C, typename... Args>
    auto OriginEntity::Add(Args&&... args) -> OriginEntity&
    {
        return AddComponent(ComponentIdPool::index<C>(),
                            std::make_unique<OriginComponent>(std::forward<Args>(args)...));
    }

    template <typename Arg>
    auto OriginEntity::Remove() -> OriginEntity&
    {
        return RemoveComponent(ComponentIdPool::index<Arg>());
    }
    template <typename Arg0, typename... Args>
    auto OriginEntity::Remove() -> typename std::enable_if<sizeof...(Args) != 0, OriginEntity&>::type
    {
        return RemoveComponent(ComponentIdPool::index<Arg0>()), Remove<Args...>();
    }

    template <typename C, typename... Args>
    auto OriginEntity::Replace(Args&&... args) -> OriginEntity&
    {
        return ReplaceComponent(ComponentIdPool::index<C>(),
                                std::make_unique<OriginComponent>(std::forward<Args>(args)...));
    }

    template <typename C>
    auto OriginEntity::Get() const -> C*
    {
        return static_cast<C*>(GetComponent(ComponentIdPool::index<C>()));
    }

    template <typename... Args>
    auto OriginEntity::Get() const ->
        typename std::enable_if<(sizeof...(Args) != 1), std::tuple<Args*...>>::type
    {
        using ValueType = std::tuple<Args*...>;
        if (Has<Args...>()) {
            return ValueType(Get<Args>()...);
        }
        return ValueType();
    }

    template <typename Arg>
    bool OriginEntity::Has() const
    {
        return HasComponent(ComponentIdPool::index<Arg>());
    }

    template <typename Arg0, typename... Args>
    auto OriginEntity::Has() const -> typename std::enable_if<sizeof...(Args) != 0, bool>::type
    {
        return HasComponent(ComponentIdPool::index<Arg0>()) && Has<Args...>();
    }

}