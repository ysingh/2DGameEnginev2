#ifndef MOVEMENTSTSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"

class MovementSystem: public System {
public:
	MovementSystem() {
		// TODO: RequireComponent<TransformComponent>();
		// TODO: RequireComponent<VelocityComponent>();
	}
	void Update(double deltaTime) {
		// TODO: 
		// Loop over all entities that the system is interested in:
		//for (auto entity : GetEntities()) {
		//	TODO: Update all entities
		//}
	}
};

#endif
