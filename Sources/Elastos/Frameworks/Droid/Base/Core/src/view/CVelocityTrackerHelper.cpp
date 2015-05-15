
#include "ext/frameworkext.h"
#include "view/CVelocityTrackerHelper.h"
#include "view/VelocityTracker.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CVelocityTrackerHelper::Obtain(
    /* [out] */ IVelocityTracker** velocity)
{
	VALIDATE_NOT_NULL(velocity);

	AutoPtr<VelocityTracker> tracker = VelocityTracker::Obtain();
	*velocity = (IVelocityTracker*)tracker->Probe(EIID_IVelocityTracker);
	INTERFACE_ADDREF(*velocity);
	return NOERROR;
}

ECode CVelocityTrackerHelper::ObtainEx(
    /* [in] */ const String& strategy,
    /* [out] */ IVelocityTracker** velocity)
{
	VALIDATE_NOT_NULL(velocity);

	AutoPtr<VelocityTracker> tracker = VelocityTracker::Obtain(strategy);
	*velocity = (IVelocityTracker*)tracker->Probe(EIID_IVelocityTracker);
	INTERFACE_ADDREF(*velocity);
	return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos