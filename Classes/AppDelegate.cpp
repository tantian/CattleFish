#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
//    director->setDepthTest(false);
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
//    director->setAnimationInterval(1.0 / 60);

    auto screenSize = glview->getFrameSize();
    auto resourceSize = Size(1280, 800);
    if (screenSize.width > screenSize.height) { // 横屏
    	std::vector<std::string> searchPaths;
    	searchPaths.push_back("landscape");
		log("screenSize width is %f, screenSize height is %f ", screenSize.width, screenSize.height);

//        if (screenSize.width > 1278 && screenSize.width < 1281) {
//        	searchPaths.push_back("landscape/1280_800");
//        } else
        if (screenSize.width < 1367 && screenSize.width > 1365) {
        	searchPaths.push_back("landscape/1366_768");
        } else if (screenSize.width > 1367) {
//        	searchPaths.push_back("landscape/1280_800");
        	float scale = MIN(resourceSize.height/screenSize.height, resourceSize.width/screenSize.width);
        	director->setContentScaleFactor(scale);
        }

    	FileUtils::getInstance()->setSearchPaths(searchPaths);
    } else {  // 竖屏
        resourceSize = Size(480, 800);
        if (screenSize.height > 800.1) {
        	float scale = MIN(resourceSize.height/screenSize.height, resourceSize.width/screenSize.width);
        	director->setContentScaleFactor(scale);
        }
    }
    log("screenSize height is %f, designSize height is %f ", screenSize.height, resourceSize.height);

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}


void AppDelegate::applicationScreenSizeChanged(int newWidth, int newHeight) {

    log(" applicationScreenSizeChanged newWidth is %d, newHeight is %d ", newWidth, newHeight);

}
