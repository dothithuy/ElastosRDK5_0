
#ifndef __CCOMPONENTNAME_H__
#define __CCOMPONENTNAME_H__

#include "_CComponentName.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CComponentName)
{
public:
    CARAPI constructor();

    /**
     * Create a new component identifier.
     *
     * @param pkg The name of the package that the component exists in.  Can
     * not be null.
     * @param cls The name of the class inside of <var>pkg</var> that
     * implements the component.  Can not be null.
     */
    CARAPI constructor(
        /* [in] */ const String& pkg,
        /* [in] */ const String& cls);

    /**
     * Create a new component identifier from a Context and class name.
     *
     * @param pkg A Context for the package implementing the component,
     * from which the actual package name will be retrieved.
     * @param cls The name of the class inside of <var>pkg</var> that
     * implements the component.
     */
    CARAPI constructor(
        /* [in] */ IContext* pkg,
        /* [in] */ const String& cls);

    /**
     * Create a new component identifier from a Context and Class object.
     *
     * @param pkg A Context for the package implementing the component, from
     * which the actual package name will be retrieved.
     * @param cls The Class object of the desired component, from which the
     * actual class name will be retrieved.
     */
    CARAPI constructor(
        /* [in] */ IContext* pkg,
        /* [in] */ IClassInfo* cls);

    CARAPI Clone(
        /* [out] */ IComponentName** newComponentName);

    /**
     * Return the package name of this component.
     */
    CARAPI GetPackageName(
        /* [out] */ String* packageName);

    /**
     * Return the class name of this component.
     */
    CARAPI GetClassName(
        /* [out] */ String* className);

    /**
     * Return the class name, either fully qualified or in a shortened form
     * (with a leading '.') if it is a suffix of the package.
     */
    CARAPI GetShortClassName(
        /* [out] */ String* shortClassName);

    /**
     * Return a String that unambiguously describes both the package and
     * class names contained in the ComponentName.  You can later recover
     * the ComponentName from this string through
     * {@link #unflattenFromString(String)}.
     *
     * @return Returns a new String holding the package and class names.  This
     * is represented as the package name, concatenated with a '/' and then the
     * class name.
     *
     * @see #unflattenFromString(String)
     */
    CARAPI FlattenToString(
        /* [out] */ String* name);

    /**
     * The same as {@link #flattenToString()}, but abbreviates the class
     * name if it is a suffix of the package.  The result can still be used
     * with {@link #unflattenFromString(String)}.
     *
     * @return Returns a new String holding the package and class names.  This
     * is represented as the package name, concatenated with a '/' and then the
     * class name.
     *
     * @see #unflattenFromString(String)
     */
    CARAPI FlattenToShortString(
        /* [out] */ String* name);

    static CARAPI UnflattenFromString(
        /* [in] */ const String& str,
        /* [out] */ IComponentName** componentName);

    /**
     * Return string representation of this class without the class's name
     * as a prefix.
     */
    CARAPI ToShortString(
        /* [out] */ String* name);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI Equals(
        /* [in] */ IComponentName* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI CompareTo(
        /* [in] */ IComponentName* that,
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    static CARAPI ReadFromParcel(
        /* [in] */ IParcel* source,
        /* [out] */ IComponentName** component);

    static CARAPI WriteToParcel(
        /* [in] */ IComponentName* component,
        /* [in] */ IParcel* dest);

private:
    String mPackage;
    String mClass;
};

} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CCOMPONENTNAME_H__
