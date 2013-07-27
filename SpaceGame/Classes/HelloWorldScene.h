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

typedef enum {
  KENDREASONWIN,
  KENDREASONLOSE
} EndReason;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	//Override method update
	//scheduled update
	void update (float pdt);

	//add asteroid
	//random value
	float randomValueBetween(float plow, float phigh);
	
	void setInvisible(CCNode* pnode);
	
	float getTimeTick();
	//action when touch_move on screen
	virtual void ccTouchesBegan(cocos2d::CCSet* Touches, cocos2d::CCEvent* pEvent);

	//action when touch_move on screen
	virtual void ccTouchesEnded(cocos2d::CCSet* Touches, cocos2d::CCEvent* pEvent);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
private:
	//layout containt ship
	cocos2d::CCSpriteBatchNode* mBatchNode;
	//element ship
	cocos2d::CCSprite* mShip;
	//layout containt background
	CCParallaxNodeExtras* mBackgroundNode;
	//element into background layout
	cocos2d::CCSprite* mSpacedust1;
	cocos2d::CCSprite* mSpacedust2;
	cocos2d::CCSprite* mPlanetsunrise;
	cocos2d::CCSprite* mGalaxy;
	cocos2d::CCSprite* mSpacialanomaly;
	cocos2d::CCSprite* mSpacialanomaly2;
	float mShipPointPerSecY;

	//store asteroids
	CCArray* mAsteroids;
	int mNextAsteroid;
	float mNextAsteroidSpawn;

	//value store shoot
	CCArray* mShipLasers;
	int mNextShipLaser;

	//value store location touch
	CCPoint location;

	//collision detection
	int nLives;

	//over game
	double nGameOverTime;
	int nAsteroid;
	bool isGameOver;

	//over game
	void mEndScene(EndReason pEndReason);
	//restart game
	//fix: add parameters
	void restartTapped(CCObject* pSender);

};

#endif // __HELLOWORLD_SCENE_H__
