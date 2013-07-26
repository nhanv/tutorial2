/***************************************************************************
*
* CCParallaxNodeExtras.cpp
* SpaceGame
*
* Created by Nguyen Van Nha on 25/07/13.
*
* Copyright Vinova Pte. Ltd. All rights reserved.
***************************************************************************
*/

#include "CCParallaxNodeExtras.h"

using namespace cocos2d;

// Hack to access CCPointObject (which is not a public class)
//create private class CCPointObject extends class CCObject
class CCPointObject  : CCObject
{
	/*
	*That like
	*#define CC_SYNTHESIZE(varType, varName, funName)\
	*protected: varType varName;\
	*public: inline varType get##funName(void) const { return varName; }\
	*public: inline void set##funName(varType var){ varName = var; }
	*/
    CC_SYNTHESIZE(CCPoint, pRatio, Ratio)
    CC_SYNTHESIZE(CCPoint, pOffset, Offset)
    CC_SYNTHESIZE(CCNode*, pChild, Child)
};

/**
*constructor
*/
CCParallaxNodeExtras::CCParallaxNodeExtras()
{
	CCParallaxNode();
}

CCParallaxNodeExtras* CCParallaxNodeExtras::node()
{
	return new CCParallaxNodeExtras();
}

/**
*impelemt function incrementOffset
*m_pParallaxArray is ParallaxNode's value protected
*/
void CCParallaxNodeExtras::incrementOffset(CCPoint poffset,CCNode* pnode){
    for( unsigned int i = 0; i < m_pParallaxArray->num; i++) {
        CCPointObject* point = (CCPointObject* ) m_pParallaxArray->arr[i];
        CCNode* curNode = point->getChild();
        if( curNode->isEqual(pnode) ) {
            point->setOffset( ccpAdd(point->getOffset(), poffset) );
            break;
        }
    }
}