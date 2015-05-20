
#include "CXmlPullParserFactoryHelper.h"
#include "XmlPullParserFactory.h"

namespace Org {
namespace Xmlpull {
namespace V1 {

ECode CXmlPullParserFactoryHelper::NewInstance(
    /* [out] */ IXmlPullParserFactory** xmlPullParserFac)
{
    return XmlPullParserFactory::NewInstance(xmlPullParserFac);
}

ECode CXmlPullParserFactoryHelper::NewInstance(
    /* [in] */ const String& classNames,
 // /* [in] */  Class context,
    /* [out] */ IXmlPullParserFactory** xmlPullParserFac)
{
    return XmlPullParserFactory::NewInstance(classNames, xmlPullParserFac);
}

} // namespace V1
} // namespace Xmlpull
} // namespace Org
