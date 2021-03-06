
#ifndef __CHANDLERBASE_H__
#define __CHANDLERBASE_H__

#include "__Org_Xml_Sax_CHandlerBase.h"
#include "HandlerBase.h"

namespace Org {
namespace Xml {
namespace Sax {

CarClass(CHandlerBase)
    : public HandlerBase
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Sax
} // namespace Xml
} // namespace Org

#endif // __CHANDLERBASE_H__
