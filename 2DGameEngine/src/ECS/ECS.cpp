#include <vector>
#include "ECS.h"
#include "../Logger/Logger.h"

int IComponent::nextId = 0;

void System::AddEntityToSystem(Entity entity) {
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
		return entity == other;
	}), entities.end());
	
}

Entity Registry::CreateEntity() {
	int entityId = numEntities++;

	Entity entity { entityId };
	entitiesToBeAdded.insert(entity);

	if (entityId >= entityComponentSignatures.size()) {
		entityComponentSignatures.resize(entityId + 1);
	}

	Logger::Log("Entity created with id = " + std::to_string(entityId));

	return entity;
}

void Registry::Update() {
	// TODO: Add the entities that are waiting to be created to the active systems
	// TODO: Remove the entities that are waiting to be created to the active systems
	for (auto entity : entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}

	entitiesToBeAdded.clear();
}

void Registry::AddEntityToSystems(Entity entity) {
	const auto entityId = entity.GetId();

	// entityComponentSignature math with systemComponentSignature
	const auto& entityComponentSignature = entityComponentSignatures[entityId];
	
	
	for (auto& system: systems) {
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested) {
			system.second->AddEntityToSystem(entity);
		}
	}
}


