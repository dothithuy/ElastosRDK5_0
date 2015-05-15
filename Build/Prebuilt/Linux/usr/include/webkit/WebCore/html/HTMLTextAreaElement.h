/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2010 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef HTMLTextAreaElement_h
#define HTMLTextAreaElement_h

#include "HTMLFormControlElement.h"

namespace WebCore {

class BeforeTextInsertedEvent;
class VisibleSelection;

class HTMLTextAreaElement : public HTMLTextFormControlElement {
public:
    static PassRefPtr<HTMLTextAreaElement> create(const QualifiedName&, Document*, HTMLFormElement*);

    int cols() const { return m_cols; }
    int rows() const { return m_rows; }

    bool shouldWrapText() const { return m_wrap != NoWrap; }

    virtual WTF::String value() const;
    void setValue(const WTF::String&);
    WTF::String defaultValue() const;
    void setDefaultValue(const WTF::String&);
    int textLength() const { return value().length(); }
    virtual int maxLength() const;
    void setMaxLength(int, ExceptionCode&);
    bool valueMissing(const WTF::String& value) const { return isRequiredFormControl() && !disabled() && !readOnly() && value.isEmpty(); }
    bool tooLong(const WTF::String&, NeedsToCheckDirtyFlag) const;
    bool isValidValue(const WTF::String&) const;

    void rendererWillBeDestroyed();

    void setCols(int);
    void setRows(int);

    bool lastChangeWasUserEdit() const;

    void cacheSelection(int s, int e) { m_cachedSelectionStart = s; m_cachedSelectionEnd = e; };

private:
    HTMLTextAreaElement(const QualifiedName&, Document*, HTMLFormElement*);

    enum WrapMethod { NoWrap, SoftWrap, HardWrap };

    void handleBeforeTextInsertedEvent(BeforeTextInsertedEvent*) const;
    static WTF::String sanitizeUserInputValue(const WTF::String&, unsigned maxLength);
    void updateValue() const;
    void setNonDirtyValue(const WTF::String&);
    void setValueCommon(const WTF::String&);

    virtual bool supportsPlaceholder() const { return true; }
    virtual bool isEmptyValue() const { return value().isEmpty(); }
    virtual int cachedSelectionStart() const { return m_cachedSelectionStart; }
    virtual int cachedSelectionEnd() const { return m_cachedSelectionEnd; }

    virtual bool isOptionalFormControl() const { return !isRequiredFormControl(); }
    virtual bool isRequiredFormControl() const { return required(); }

    virtual void defaultEventHandler(Event*);

    virtual bool isEnumeratable() const { return true; }

    virtual const AtomicString& formControlType() const;

    virtual bool saveFormControlState(WTF::String& value) const;
    virtual void restoreFormControlState(const WTF::String&);

    virtual bool isTextFormControl() const { return true; }

    virtual void childrenChanged(bool changedByParser = false, Node* beforeChange = 0, Node* afterChange = 0, int childCountDelta = 0);
    virtual void parseMappedAttribute(Attribute*);
    virtual RenderObject* createRenderer(RenderArena*, RenderStyle*);
    virtual bool appendFormData(FormDataList&, bool);
    virtual void reset();
    virtual bool isMouseFocusable() const;
    virtual bool isKeyboardFocusable(KeyboardEvent*) const;
    virtual void updateFocusAppearance(bool restorePreviousSelection);

    virtual void accessKeyAction(bool sendToAnyElement);

    virtual bool shouldUseInputMethod() const;

    int m_rows;
    int m_cols;
    WrapMethod m_wrap;
    mutable WTF::String m_value;
    int m_cachedSelectionStart;
    int m_cachedSelectionEnd;
    mutable bool m_isDirty;
};

} //namespace

#endif