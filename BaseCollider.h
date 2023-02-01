#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "CollisionInfo.h"

class BaseCollider {
public:
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d* object) {
		this->object3d = object;
	}

	inline Object3d* GetObject3d() {
		return object3d;
	}

	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() {
		return shapeType;
	}

	inline void OnCollision(const CollisionInfo& info) {
		object3d->OnCollision(info);
	}

protected:
	Object3d* object3d = nullptr;

	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};