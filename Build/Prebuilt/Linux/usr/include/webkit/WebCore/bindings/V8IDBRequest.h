/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#if ENABLE(INDEXED_DATABASE)

#ifndef V8IDBRequest_h
#define V8IDBRequest_h

#include "IDBRequest.h"
#include "V8DOMWrapper.h"
#include "WrapperTypeInfo.h"
#include <wtf/text/StringHash.h>
#include <v8.h>
#include <wtf/HashMap.h>

namespace WebCore {

class V8IDBRequest {

public:
    static bool HasInstance(v8::Handle<v8::Value> value);
    static v8::Persistent<v8::FunctionTemplate> GetRawTemplate();
    static v8::Persistent<v8::FunctionTemplate> GetTemplate();
    static IDBRequest* toNative(v8::Handle<v8::Object> object)
    {
        return reinterpret_cast<IDBRequest*>(object->GetPointerFromInternalField(v8DOMWrapperObjectIndex));
    }
    inline static v8::Handle<v8::Object> wrap(IDBRequest*);
    static void derefObject(void*);
    static WrapperTypeInfo info;
    static ActiveDOMObject* toActiveDOMObject(v8::Handle<v8::Object>);
    static const int eventListenerCacheIndex = v8DefaultWrapperInternalFieldCount + 0;
    static const int internalFieldCount = v8DefaultWrapperInternalFieldCount + 1;
private:
    static v8::Handle<v8::Object> wrapSlow(IDBRequest*);
};


v8::Handle<v8::Object> V8IDBRequest::wrap(IDBRequest* impl)
{
        v8::Handle<v8::Object> wrapper = getActiveDOMObjectMap().get(impl);
        if (!wrapper.IsEmpty())
            return wrapper;
    return V8IDBRequest::wrapSlow(impl);
}

inline v8::Handle<v8::Value> toV8(IDBRequest* impl)
{
    if (!impl)
        return v8::Null();
    return V8IDBRequest::wrap(impl);
}
inline v8::Handle<v8::Value> toV8(PassRefPtr< IDBRequest > impl)
{
    return toV8(impl.get());
}
}

#endif // V8IDBRequest_h
#endif // ENABLE(INDEXED_DATABASE)
