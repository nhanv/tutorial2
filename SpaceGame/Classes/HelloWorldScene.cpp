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
//quantity of asteroid
#define KNUMASTEROIDS 10
//quantity of laser
#define KNUMLASERS 5

#include "HelloWorldScene.h"
#include "CCParallaxNodeExtras.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    ////////////////////////////////////////////////////////////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    //////////////////////////////////////////////////////////////////////////////////////
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
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	//play sound
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("SpaceGame.wav",true);
	SimpleAudioEngine::sharedEngine()->preloadEffect("explosion_large.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("laser_ship.wav");

	///////////////////////////////////////////////////////////////////////////////////////
	//3.add background: create object CCParallaxNode containt other element
	//CCParallaxNode nhu 1 background, cac phan tu gan tren no co the troi voi cac toc do
	//khac nhau

	mBackgroundNode = CCParallaxNodeExtras::node();
	this->addChild(mBackgroundNode);

	//create element space into mbackgroundNode
	mSpacedust1 = CCSprite::create("bg_front_spacedust.png");
	mSpacedust2 = CCSprite::create("bg_front_spacedust.png");
	mPlanetsunrise = CCSprite::create("bg_planetsunrise.png");
	mGalaxy = CCSprite::create("bg_galaxy.png");
	mSpacialanomaly = CCSprite::create("bg_spacialanomaly.png");
	mSpacialanomaly2 = CCSprite::create("bg_spacialanomaly2.png");

	//element's speed move
	CCPoint dustSpeed = ccp(0.1, 0.1);
	//parent's speed move
	CCPoint bgSpeed = ccp(0.05, 0.05);

	//add into mbackground
	//ex: add mspacedust1 voi tag = 0 (tag lon hon ve sau, nho hon ve truoc)
	//co toc do di chuyen so voi CCDirctor = mduslSpeed, tai vi tri ccp(x, y)
	mBackgroundNode->addChild(mSpacedust1, 0, dustSpeed, ccp(mSpacedust1->getContentSize().width/2, winSize.height/2));
	mBackgroundNode->addChild(mSpacedust2, 0, dustSpeed, ccp((mSpacedust1->getContentSize().width/2)*3, winSize.height/2));
	mBackgroundNode->addChild(mPlanetsunrise, -1, bgSpeed, ccp(400, winSize.height*0));
	mBackgroundNode->addChild(mGalaxy, -1, bgSpeed, ccp(30, winSize.height*0.7));
	mBackgroundNode->addChild(mSpacialanomaly, -1, bgSpeed, ccp(220, winSize.height*0.5));
	mBackgroundNode->addChild(mSpacialanomaly2, -1, bgSpeed, ccp(400, winSize.height*0.9));

    ///////////////////////////////////////////////////////////////////////////////////////
	//add the ship into game
	//create object mbatchNode containt list picture and add this into CCDirector
	mBatchNode = CCSpriteBatchNode::create("Sprites.pvr.ccz");
    // add the sprite as a child to this layer
	this->addChild(mBatchNode);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");

	//take picture element from batchNode
	mShip = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
	mShip->setPosition(ccp(winSize.width*0.1, winSize.height/2));
	mBatchNode->addChild(mShip);

	////////////////////////////////////////////////////////////////////////////////////////
	//add starts into game
	HelloWorld::addChild(CCParticleSystemQuad::create("Stars1.plist"));
	HelloWorld::addChild(CCParticleSystemQuad::create("Stars2.plist"));
	HelloWorld::addChild(CCParticleSystemQuad::create("Stars3.plist"));

	/////////////////////////////////////////////////////////////////////////////////////////
	//call scheduled update
	this->scheduleUpdate();

	//move ship with Accelerometer
	//enable accelerometer for ship, allow use physic
    
	/////////////////////////////////////////////////////////////////////////////////////////
	//add asteroid
	
	mNextAsteroid = 0;
	mAsteroids = new CCArray();
	for(int i = 0; i < KNUMASTEROIDS; ++i)
	{
		CCSprite* asteroid = CCSprite::createWithSpriteFrameName("asteroid.png");
		asteroid->setVisible(false);
		mBatchNode->addChild(asteroid);
		mAsteroids->addObject(asteroid);
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//action when touch_move
	mNextShipLaser = 0;
	mShipLasers = new CCArray();
	for(int i = 0; i < KNUMLASERS; ++i) {
		CCSprite *shipLaser = CCSprite::createWithSpriteFrameName("laserbeam_blue.png");
		shipLaser->setVisible(false);
		mBatchNode->addChild(shipLaser);
		mShipLasers->addObject(shipLaser);
	}

	this->setTouchEnabled(true);
	/////////////////////////////////////////////////////////////////////////////////////////
	//over game
	//set default nLives and nGameOverTime
	nLives = 3;
	nAsteroid = 0;
	isGameOver = false;
	double curTime = getTimeTick();
	nGameOverTime = curTime + 30000;
	/////////////////////////////////////////////////////////////////////////////////////////
    return true;
}

/**
*scheduled update
*/
void HelloWorld::update (float pdt)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//di chuyen background theo chieu x=1000px/fr, huong y=0px/fr
	CCPoint backgroundScrollVert = ccp(-1000, 0);
	//toan tu vector ccpAdd: setPosition theo vector ccpAdd co toa do goc va dau tu
	//ccpAdd: tinh vector: giua position cua background> ccpMult tinh lai toc do cho background
	//nhan toc do mbackgroundScrollVert voi tg giua 2 frames (pdt) de tro choi nhat quan
	mBackgroundNode->setPosition(ccpAdd(mBackgroundNode->getPosition(), ccpMult(backgroundScrollVert, pdt)));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//repeat background
	CCArray* spaceDusts = CCArray::createWithCapacity(2);
	spaceDusts->addObject(mSpacedust1);
	spaceDusts->addObject(mSpacedust2);

	for (int i = 0; i < (spaceDusts->count()); ++i)
	{
		CCSprite* spaceDust = (CCSprite*)(spaceDusts->objectAtIndex(i));
		float xPosition = mBackgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
		float size = spaceDust->getContentSize().width;

		if( xPosition <= -size/2)
		{
			mBackgroundNode->incrementOffset(ccp(spaceDust->getContentSize().width*2,0),spaceDust);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//repeat element on background
	CCArray *backGrounds = CCArray::createWithCapacity(4);
	backGrounds->addObject(mGalaxy);
	backGrounds->addObject(mPlanetsunrise);
	backGrounds->addObject(mSpacialanomaly);
	backGrounds->addObject(mSpacialanomaly2);

	for ( int i = 0; i < (backGrounds->count()); ++i )
	{
		CCSprite* background = (CCSprite*)(backGrounds->objectAtIndex(i));
		float xPosition = mBackgroundNode->convertToWorldSpace(background->getPosition()).x;
		float size = background->getContentSize().width;
	
		if ( xPosition <= -size/2 )
		{
			mBackgroundNode->incrementOffset(ccp(800,0),background); 
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//repeat asteroids
	float curTimeMillis = getTimeTick();
	if (curTimeMillis > mNextAsteroidSpawn)
	{
 
		float randMillisecs = randomValueBetween(0.20f, 1.0f) * 1000;
		mNextAsteroidSpawn = randMillisecs + curTimeMillis;
 
		//random position asteroid
		float randY = randomValueBetween(0.0, winSize.height)*100000;
		float randDuration = randomValueBetween(2.0,10.0);
 
		CCSprite* asteroid = (CCSprite*) mAsteroids->objectAtIndex(mNextAsteroid);
		mNextAsteroid ++;
 
		if (mNextAsteroid >= (mAsteroids->count()))
			mNextAsteroid = 0;
 
		asteroid->stopAllActions();
		asteroid->setPosition( ccp(winSize.width+asteroid->getContentSize().width/2, randY));
		asteroid->setVisible(true);

		asteroid->runAction(CCSequence::create(CCMoveBy::create(
							randDuration, ccp(- winSize.width - asteroid->getContentSize().width, 0)),
								CCCallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)), NULL
		));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//collision detection
	CCObject* asteroid;
	CCObject* shipLaser;
	//duyet mang cach hanh tinh
	CCARRAY_FOREACH( mAsteroids, asteroid)
	{
		//neu chua duoc hien thi (visible) thi tiep tuc duyet phan tu khac
		if (!((CCSprite*) asteroid)->isVisible() )
			continue;
		//duyet mang chua laser
		CCARRAY_FOREACH( mShipLasers, shipLaser)
		{
			//neu laser chua dc hien thi (visible) thi tiep tuc duyet phan tu khac
			if (!((CCSprite *) shipLaser)->isVisible())
				continue;

			//neu xay ra va cham thi xoa ca laser va asteroid (setVisible = false)
			if ( ( (CCSprite* ) shipLaser)->boundingBox().intersectsRect( ( (CCSprite* ) asteroid)->boundingBox() ) )
			{
				//play sound
				SimpleAudioEngine::sharedEngine()->playEffect("explosion_large.wav");
				//remove shipLaser and asteroid
				((CCSprite* ) shipLaser)->setVisible(false);
				((CCSprite* ) asteroid)->setVisible(false);
				nAsteroid ++;
				continue;
			}
		}

		//truong hop xet va cham giua hanh tinh va tau
		//thi an tau di roi hien lai (hieu ung nhay nhay) va giam tg song cua tau di (lives--)
		//khi nao het mLives thi nguoi choi thua
		if (mShip->isVisible())
		{
			if (mShip->boundingBox().intersectsRect(((CCSprite* ) asteroid)->boundingBox()) )
			{
				((CCSprite *)asteroid)->setVisible(false);
				/*
				void Player::blink() {
				CCBlink blink = CCBlink::actionWithDuration(1.0, 9);
				runAction(blink);
				}*/
				mShip->runAction( CCBlink::create(1.0, 9));
				nLives --;
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//check over game
	if (nLives <= 0)
	{
		mShip->stopAllActions();
		mShip->setVisible(false);
		this->mEndScene(KENDREASONLOSE);

	}
	else if (curTimeMillis >= nGameOverTime)
	{
		this->mEndScene(KENDREASONWIN);
	}else if (nAsteroid >= 10)
	{
		this->mEndScene(KENDREASONWIN);
	}
}

/**
*action when touch on screen
*/
void HelloWorld::ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent)
{
	if(mShip->isVisible())
	{
		//play sound
		SimpleAudioEngine::sharedEngine()->playEffect("laser_ship.wav");
		//set location
		CCTouch* touch = (CCTouch*) (pTouches->anyObject());
		location = touch->getLocationInView();
		}
}

/**
*action when touch move on screen
*/
void HelloWorld::ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent)
{
	if(mShip->isVisible())
	{
		//get winSize
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		//get location touch on screen
		float touchY = location.y;

		CCPoint locationT = ccp(30.0, winSize.height - touchY);

		mShip->setPosition(locationT);

    
		CCSprite *shipLaser = (CCSprite* ) mShipLasers->objectAtIndex(mNextShipLaser++);
    
		if ( mNextShipLaser >= mShipLasers->count() )
			mNextShipLaser = 0;

		shipLaser->setPosition( ccpAdd( mShip->getPosition(), ccp(shipLaser->getContentSize().width/2, 0)));
		shipLaser->setVisible(true);
		shipLaser->stopAllActions();
		shipLaser->runAction(CCSequence::create(CCMoveBy::create(0.5,ccp(winSize.width, 0)),
																	CCCallFuncN::create(this,
																	callfuncN_selector(HelloWorld::setInvisible)), NULL
		));
	}
}


/**
*game over
*/
void HelloWorld::mEndScene( EndReason pEndReason )
{
	//check over game
    if (isGameOver)
		return;
	//if call to mEndScene then set isGameOver = true
    isGameOver = true;
 
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    char message[10] = "You Win";

    if ( pEndReason == KENDREASONLOSE)
        strcpy(message,"You Lose");

	//describe a label show mesage
    CCLabelBMFont* label ;
    label = CCLabelBMFont::create(message, "Arial.fnt");
    label->setScale(0.1f);
    label->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
    this->addChild(label);

	//menu
    CCLabelBMFont* restartLabel;
    strcpy(message,"Restart");
    restartLabel = CCLabelBMFont::create(message, "Arial.fnt");

	//describe menuItem
	CCMenuItemLabel* restartItem = CCMenuItemLabel::create(restartLabel, this, menu_selector(HelloWorld::restartTapped) );
    restartItem->setPosition( ccp(winSize.width/2, winSize.height*0.4));

	//describe menu
    CCMenu* menu = CCMenu::create (restartItem, NULL);
    menu->setPosition (CCPointZero);
    this->addChild(menu);
 
    // clear label and menu
    //restartItem->runAction (CCScaleTo::create(0.5, 1.0));
    label ->runAction (CCScaleTo::create(0.5, 1.0));
    // Terminate update callback
    this->unscheduleUpdate();
	
}

/**
*random value 
*/
float HelloWorld::randomValueBetween(float plow, float phigh)
{
	return ( ( (float) rand() / 0xFFFFFFFFu) * (phigh - plow) ) + plow;
}
 
/**
*get time
*/
float HelloWorld::getTimeTick()
{
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return (float) millisecs;
}

/**
*setInvisible
*/
void HelloWorld::setInvisible(CCNode* pnode)
{
    pnode->setVisible(false);
}

/**
*restartGame
*Khi mEndScene goi toi restartTapped se bi loi ep kieu sang ELS_Handler
*la do ham restartTapped ko tham so
*Cach fix: Them tham so CCObject* pSender vao
*/
void HelloWorld::restartTapped(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene (CCTransitionZoomFlipX::create(0.5, this->scene()));
    // reschedule: callback game;
    this->scheduleUpdate();
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
