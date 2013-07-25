/***************************************************************************
*
* HelloWorldScene.h
* SpaceGame
*
* Created by Nguyen Van Nha on 25/07/13.
*
* Copyright Vinova Pte. Ltd. All rights reserved.
***************************************************************************
*/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtras.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	//scheduled update
	void update (float pdt);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
private:
	//layout containt ship
	cocos2d::CCSpriteBatchNode* mbatchNode;
	//element ship
	cocos2d::CCSprite* mship;
	//layout containt background
	CCParallaxNodeExtras* mbackgroundNode;
	//element into background layout
	cocos2d::CCSprite* mspacedust1;
	cocos2d::CCSprite* mspacedust2;
	cocos2d::CCSprite* mplanetsunrise;
	cocos2d::CCSprite* mgalaxy;
	cocos2d::CCSprite* mspacialanomaly;
	cocos2d::CCSprite* mspacialanomaly2;
};

#endif // __HELLOWORLD_SCENE_H__
