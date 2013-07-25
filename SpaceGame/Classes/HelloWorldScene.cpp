/***************************************************************************
*
* HelloWorldScene.cpp
* SpaceGame
*
* Created by Nguyen Van Nha on 25/07/13.
*
* Copyright Vinova Pte. Ltd. All rights reserved.
***************************************************************************
*/

#include "HelloWorldScene.h"
#include "CCParallaxNodeExtras.h"

USING_NS_CC;

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *mscene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *mlayer = HelloWorld::create();

    // add layer as a child to scene
    mscene->addChild(mlayer);

    // return the scene
    return mscene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

	//get winSize
	CCSize mwinSize = CCDirector::sharedDirector()->getWinSize();

	///////////////////////////////////////////////////////////////////////////////////////
	//3.add background: create object CCParallaxNode containt other element
	//CCParallaxNode nhu 1 background, cac phan tu gan tren no co the troi voi cac toc do
	//khac nhau

	mbackgroundNode = CCParallaxNodeExtras::node();
	this->addChild(mbackgroundNode);

	//create element space into mbackgroundNode
	mspacedust1 = CCSprite::create("bg_front_spacedust.png");
	mspacedust2 = CCSprite::create("bg_front_spacedust.png");
	mplanetsunrise = CCSprite::create("bg_planetsunrise.png");
	mgalaxy = CCSprite::create("bg_galaxy.png");
	mspacialanomaly = CCSprite::create("bg_spacialanomaly.png");
	mspacialanomaly2 = CCSprite::create("bg_spacialanomaly2.png");

	//element's speed move
	CCPoint mdustSpeed = ccp(0.1, 0.1);
	//parent's speed move
	CCPoint mbgSpeed = ccp(0.05, 0.05);

	//add into mbackground
	//ex: add mspacedust1 voi tag = 0 (tag lon hon ve sau, nho hon ve truoc)
	//co toc do di chuyen so voi CCDirctor = mduslSpeed, tai vi tri ccp(x, y)
	mbackgroundNode->addChild(mspacedust1, 0, mdustSpeed, ccp(mspacedust1->getContentSize().width/2, mwinSize.height/2));
	mbackgroundNode->addChild(mspacedust2, 0, mdustSpeed, ccp((mspacedust1->getContentSize().width/2)*3, mwinSize.height/2));
	mbackgroundNode->addChild(mplanetsunrise, -1, mbgSpeed, ccp(400, mwinSize.height*0));
	mbackgroundNode->addChild(mgalaxy, -1, mbgSpeed, ccp(30, mwinSize.height*0.7));
	mbackgroundNode->addChild(mspacialanomaly, -1, mbgSpeed, ccp(220, mwinSize.height*0.5));
	mbackgroundNode->addChild(mspacialanomaly2, -1, mbgSpeed, ccp(400, mwinSize.height*0.9));

    ///////////////////////////////////////////////////////////////////////////////////////
	//add the ship into game
	//create object mbatchNode containt list picture and add this into CCDirector
	mbatchNode = CCSpriteBatchNode::create("Sprites.pvr.ccz");
    // add the sprite as a child to this layer
	this->addChild(mbatchNode);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");

	//take picture element from batchNode
	mship = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
	mship->setPosition(ccp(mwinSize.width*0.1, mwinSize.height/2));
	mbatchNode->addChild(mship);

	////////////////////////////////////////////////////////////////////////////////////////
	//add starts into game
	HelloWorld::addChild(CCParticleSystemQuad::create("Stars1.plist"));
	HelloWorld::addChild(CCParticleSystemQuad::create("Stars2.plist"));
	HelloWorld::addChild(CCParticleSystemQuad::create("Stars3.plist"));

	/////////////////////////////////////////////////////////////////////////////////////////
	//call scheduled update
	this->scheduleUpdate();

	//move ship with Accelerometer
	//enable accelerometer for ship
	this->setAccelerometerEnabled(true);
    
    return true;
}

/**
*scheduled update
*/
void HelloWorld::update (float pdt)
{
	//di chuyen background theo huong x=500px/s, huong y=0px/s
	CCPoint mbackgroundScrollVert = ccp(-1000, 0);
	//toan tu vector ccpAdd: setPosition theo vector ccpAdd co toa do goc va dau tu
	//position cua mbackgroundNode toi ccpMult
	mbackgroundNode->setPosition(ccpAdd(mbackgroundNode->getPosition(), ccpMult(mbackgroundScrollVert, pdt)));

	CCArray* mspaceDusts = CCArray::createWithCapacity(2);
	mspaceDusts->addObject(mspacedust1);
	mspaceDusts->addObject(mspacedust2);

	for (int i = 0; i < (mspaceDusts->count()); ++i)
	{
		CCSprite* mspaceDust = (CCSprite*)(mspaceDusts->objectAtIndex(i));
		float mxPosition = mbackgroundNode->convertToNodeSpace(mspaceDust->getPosition()).x;
		float msize = mspaceDust->getContentSize().width;

		if( mxPosition < -msize/2)
		{
			mbackgroundNode->incrementOffset(ccp(mspaceDust->getContentSize().width*2, 0), mspaceDust);
		}
	}

	CCArray *mbackGrounds = CCArray::createWithCapacity(4);
	mbackGrounds->addObject(mgalaxy);
	mbackGrounds->addObject(mplanetsunrise);
	mbackGrounds->addObject(mspacialanomaly);
	mbackGrounds->addObject(mspacialanomaly2);

	for ( int i = 0; i < (mbackGrounds->count()); ++i ) {
		CCSprite* mbackground = (CCSprite*)(mbackGrounds->objectAtIndex(i));
		float mxPosition = mbackgroundNode->convertToWorldSpace(mbackground->getPosition()).x;
		float msize = mbackground->getContentSize().width;
	
		if ( mxPosition <= msize ) {
			mbackgroundNode->incrementOffset(ccp(480,0),mbackground); 
		}
	}
}

/**
*close game when click on close menu
*/
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
