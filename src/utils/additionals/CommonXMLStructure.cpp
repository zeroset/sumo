/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2021 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    AdditionalHandler.h
/// @author  Pablo Alvarez Lopez
/// @date    May 2021
///
// Structure for common XML Parsing
/****************************************************************************/
#include <config.h>

#include <utils/xml/SUMOSAXHandler.h>

#include "CommonXMLStructure.h"


// ===========================================================================
// method definitions
// ===========================================================================

// ---------------------------------------------------------------------------
// CommonXMLStructure::SumoBaseObject - methods
// ---------------------------------------------------------------------------

CommonXMLStructure::SumoBaseObject::SumoBaseObject(SumoBaseObject* parent, const SumoXMLTag tag) :
    mySumoBaseObjectParent(parent),
    myTag(tag) {
}


CommonXMLStructure::SumoBaseObject::~SumoBaseObject() {
    // delete all SumoBaseObjectChildrens
    for (const auto &sumoBaseObject : mySumoBaseObjectChildren) {
        delete sumoBaseObject;
    }
}


const SumoXMLTag 
CommonXMLStructure::SumoBaseObject::getTag() const {
    return myTag;
}


CommonXMLStructure::SumoBaseObject* 
CommonXMLStructure::SumoBaseObject::getParentSumoBaseObject() const {
    return mySumoBaseObjectParent;
}



const std::string&
CommonXMLStructure::SumoBaseObject::getStringAttribute(const SumoXMLAttr attr) const {
    if (myStringAttributes.count(attr) > 0) {
        return myStringAttributes.at(attr);
    } else {
        throw ProcessError("Attr doesn't exist");
    }
}


int
CommonXMLStructure::SumoBaseObject::getIntAttribute(const SumoXMLAttr attr) const {
    if (myIntAttributes.count(attr) > 0) {
        return myIntAttributes.at(attr);
    } else {
        throw ProcessError("Attr doesn't exist");
    }
}


double
CommonXMLStructure::SumoBaseObject::getDoubleAttribute(const SumoXMLAttr attr) const {
    if (myDoubleAttributes.count(attr) > 0) {
        return myDoubleAttributes.at(attr);
    } else {
        throw ProcessError("Attr doesn't exist");
    }
}


bool
CommonXMLStructure::SumoBaseObject::getBoolAttribute(const SumoXMLAttr attr) const {
    if (myBoolAttributes.count(attr) > 0) {
        return myBoolAttributes.at(attr);
    } else {
        throw ProcessError("Attr doesn't exist");
    }
}


SUMOTime
CommonXMLStructure::SumoBaseObject::getSUMOTimeAttribute(const SumoXMLAttr attr) const {
    if (mySUMOTimeAttributes.count(attr) > 0) {
        return mySUMOTimeAttributes.at(attr);
    } else {
        throw ProcessError("Attr doesn't exist");
    }
}


const std::map<std::string, std::string>&
CommonXMLStructure::SumoBaseObject::getParameters() const {
    return myParameters;
}


void 
CommonXMLStructure::SumoBaseObject::addStringAttribute(const SumoXMLAttr attr, const std::string &value) {
    // check if attribute was already inserted
    if (checkDuplicatedAttribute(attr)) {
        myStringAttributes[attr] = value;
    } else {
        throw InvalidArgument("duplicated attribute");
    }
}


void 
CommonXMLStructure::SumoBaseObject::addIntAttribute(const SumoXMLAttr attr, const int value) {
    // check if attribute was already inserted
    if (checkDuplicatedAttribute(attr)) {
        myIntAttributes[attr] = value;
    } else {
        throw InvalidArgument("duplicated attribute");
    }
}


void 
CommonXMLStructure::SumoBaseObject::addDoubleAttribute(const SumoXMLAttr attr, const double value) {
    // check if attribute was already inserted
    if (checkDuplicatedAttribute(attr)) {
        myDoubleAttributes[attr] = value;
    } else {
        throw InvalidArgument("duplicated attribute");
    }
}


void 
CommonXMLStructure::SumoBaseObject::addBoolAttribute(const SumoXMLAttr attr, const bool value) {
    // check if attribute was already inserted
    if (checkDuplicatedAttribute(attr)) {
        myBoolAttributes[attr] = value;
    } else {
        throw InvalidArgument("duplicated attribute");
    }
}


void 
CommonXMLStructure::SumoBaseObject::addSUMOTimeAttribute(const SumoXMLAttr attr, const SUMOTime value) {
    // check if attribute was already inserted
    if (checkDuplicatedAttribute(attr)) {
        mySUMOTimeAttributes[attr] = value;
    } else {
        throw InvalidArgument("duplicated attribute");
    }
}


void 
CommonXMLStructure::SumoBaseObject::addParameter(const std::string &attr, const std::string &value) {
    myParameters[attr] = value;
}


bool 
CommonXMLStructure::SumoBaseObject::checkDuplicatedAttribute(const SumoXMLAttr attr) const {
    return ((myStringAttributes.count(attr) + myIntAttributes.count(attr) + 
             myDoubleAttributes.count(attr) + mySUMOTimeAttributes.count(attr) + 
             myBoolAttributes.count(attr)) == 0);
}

// ---------------------------------------------------------------------------
// CommonXMLStructure - methods
// ---------------------------------------------------------------------------

CommonXMLStructure::CommonXMLStructure() :
    mySumoBaseObjectRoot(nullptr),
    myLastInsertedSumoBaseObject(nullptr) {

}


CommonXMLStructure::~CommonXMLStructure() {
    // delete mySumoBaseObjectRoot (this will also delete all SumoBaseObjectChildrens)
    delete mySumoBaseObjectRoot;
}


void
CommonXMLStructure::openTag(const SumoXMLTag myTag) {
    // first check if root is empty
    if (mySumoBaseObjectRoot == nullptr) {
        // create root
        mySumoBaseObjectRoot = new SumoBaseObject(nullptr, myTag);
        // update last inserted Root
        myLastInsertedSumoBaseObject = mySumoBaseObjectRoot;
    } else {
        // create new node
        SumoBaseObject* newSumoBaseObject = new SumoBaseObject(myLastInsertedSumoBaseObject, myTag);
        // update last inserted node
        myLastInsertedSumoBaseObject = newSumoBaseObject; 
    }
}


void 
CommonXMLStructure::closeTag() {
    // check that myLastInsertedSumoBaseObject is valid
    if (myLastInsertedSumoBaseObject) {
        // just update last inserted SumoBaseObject
        myLastInsertedSumoBaseObject = myLastInsertedSumoBaseObject->getParentSumoBaseObject();
    }
}


CommonXMLStructure::SumoBaseObject* 
CommonXMLStructure::getLastInsertedSumoBaseObject() const {
    return myLastInsertedSumoBaseObject;
}

/****************************************************************************/