#include "driverAttributes.h"

namespace ff {

		DriverAttribute::DriverAttribute() noexcept {}

		DriverAttribute::~DriverAttribute() noexcept {
			if (mHandle) {
				glDeleteBuffers(1, &mHandle);
			}
		}

		DriverAttributes::DriverAttributes() noexcept {
			EventDispatcher::getInstance()->addEventListener("attributeDispose", this, &DriverAttributes::onAttributeDispose);
		}

		DriverAttributes::~DriverAttributes() noexcept {
			EventDispatcher::getInstance()->removeEventListener("attributeDispose", this, &DriverAttributes::onAttributeDispose);
		}

		auto DriverAttributes::remove(ID attributeID) noexcept -> void
		{
			auto iter = mAttributes.find(attributeID);
			if (iter != mAttributes.end()) {
				mAttributes.erase(iter);
			}
		}

		auto DriverAttributes::onAttributeDispose(const EventBase::Ptr& e) -> void
		{
			ID attrID = *((ID*)e->mpUserData);
			remove(attrID);
		}
}