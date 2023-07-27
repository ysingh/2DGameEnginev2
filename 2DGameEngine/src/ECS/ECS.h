#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>

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

class Registry {

};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

#endif
