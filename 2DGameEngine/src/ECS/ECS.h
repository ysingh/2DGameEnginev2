#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>


constexpr unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
	static int nextId;
};

template <typename TComponent>
class Component: public IComponent {
	static int GetId() {
		static auto id = nextId++;
		return id;
	}
};

class Entity {
private:
	int id;
public:
	Entity(int id) : id(id) {};
	Entity(const Entity& entity) = default;
	Entity& operator =(const Entity& other) = default;
	bool operator ==(const Entity& other) const { return id == other.id; }
	bool operator <(const Entity& other) const { return id < other.id;  }
	const int GetId() const { return id; }

};

class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;
public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	const std::vector<Entity>& GetSystemEntities() const { return entities; }
	const Signature& GetComponentSignature() const { return componentSignature; }

	template <typename TComponent> void RequireComponent();
};

class IPool {
public:
	virtual ~IPool() = 0 {}
};

template <typename TComponent>
class Pool: IPool {
private:
	std::vector<TComponent> data;
public:
	Pool(int size = 100) {
		data.resize(size);
	}

	~Pool() = default;

	bool IsEmpty() const {
		return data.empty();
	}

	int GetSize() const {
		return data.size();
	}

	void Resize(int n) {
		data.resize(n);
	}

	void Clear() {
		data.clear();
	}

	void Add(TComponent& component) {
		data.push_back(component);
	}

	void Set(int index, TComponent& component) {
		data[index] = component;
	}

	void Remove(int entityId) {
		data.erase(entityId);
	}

	TComponent& Get(int index) {
		return static_cast<TComponent&>(data[index]);
	}
};

class Registry {
private:
	int numEntities = 0;

	// Set of entities flagged to be added or removed in the current frame
	std::set<Entity> entitiesToBeAdded = {};
	std::set<Entity> entitiesToBeKilled;
	// vector index = componentId
	std::vector<IPool*> componentPools;
	// vector index = entityId
	std::vector<Signature> entityComponentSignatures;
	std::unordered_map<std::type_index, System*> systems;
public:
	Registry() = default;

	void Update();

	Entity CreateEntity();

	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity);
	
	/*
	void KillEntity(Entity entity);
	*/

	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;

	void AddEntityToSystems(Entity entity);
};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ... args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// if this is a new component, it will not have a pool
	// or space in the component pools array to insert that pool
	// resize the array for the pool for this component
	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	// if this is a new pool
	// we would have already created space in the component pools array from
	// the check above, but there will be no pool at this id
	// create a new pool and add it to component pools
	if (!componentPools[componentId]) {
		Pool<TComponent>* newComponentPool = new Pool<TComponent>();
		componentPools[componentId] = newComponentPool;
	}

	// Get the component pool for the given component by using it's id as the index
	// as thats the scheme we decided to use
	Pool<TComponent>* componentPool = Pool<TComponent>(componentPools[componentId]);

	// Now that we have a component pool
	// Remember that we use the entity id to index into this pool
	// if the current entity id is > pool size, this means we don't have space for all entities in our pool.
	// resize pool so we have room to insert a component of this specific type for any entity
	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(numEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityId, newComponent);

	// So we are adding a component to an entity
	// We need to change the signature of our entity
	entityComponentSignatures[entityId].set(componentId);
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	/*if (componentId >= componentPools.size() || !componentPools[componentId]) {
		return;
	}

	Pool<TComponent>* componentPool = Pool<TComponent>(componentPools[componentId]);
	if (entityId >= componentPool->GetSize() || !componentPool[entityId]) {
		return;
	}

	componentPool->Remove(entityId);*/

	// For now we leave the memory as is and just set the signature to false
	entityComponentSignatures[entityId].set(componentId, false);

}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

		/*if (componentId >= componentPools.size() || !componentPools[componentId]) {
			return false;
		}

		Pool<TComponent>* componentPool = Pool<TComponent>(componentPools[componentId]);
		if (entityId >= componentPool->GetSize() || !componentPool[entityId]) {
			return false;
		}

		return true;*/

		// Can also just check the signature - which is what i will do because
		// of how removeComponent is written
		return entityComponentSignatures[entityId].test(componentId);
}

template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
	TSystem* system(new TSystem(std::forward<TArgs>(args)...));
	systems.insert(
		std::make_pair(
			std::type_index(typeid(TSystem)),
			system));
}

template <typename TSystem>
void Registry::RemoveSystem() {
	auto system = systems.find(std::type_index(typeid(TSystem)));

	// maybe 
	// systems.erase(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const {
	// other way
	// return systems.count(typeid(TSystem)) == 1;
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
	
}

template <typename TSystem>
TSystem& Registry::GetSystem() const {
	// other way
	// return systems.at(std::type_index(typeid(TSystem)));
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}


#endif
