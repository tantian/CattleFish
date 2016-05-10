/*
 * UVSprite.h
 *
 *  Created on: 2015-8-14
 *      Author: guh
 */

#ifndef UVSPRITE_H_
#define UVSPRITE_H_

#include "cocos2d.h"
USING_NS_CC;

/**
@class UVSprite
@brief Base class for cocos2d::Sprite.
@details The water ripple
*/
class UVSprite: public cocos2d::Sprite {
public:
	UVSprite();
	virtual ~UVSprite();

	//从plist中的frame创建
	static UVSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);

	//从贴图文件直接创建
	static UVSprite* create(const char *pszFileName);

	static UVSprite* createWithSpriteFrame(SpriteFrame *spriteFrame);

	void update(float dt);

	void loadShaderVertex(const char *vert, const char *frag);

	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);


private:
	 //U方向是否需要动画
	bool _AutoScrollU = true;

	//U方面动画速度（0~~1）
	float _AutoScrollSpeedU =0.1;

	//V方向是否需要动画
	bool _AutoScrollV = true;

	//V方向的动画速度（0~~1）
	float _AutoScrollSpeedV=0.1;

	//保存当前已经移动的uv值
	float _AutoScrollCountU=0;
	float _AutoScrollCountV=0;

	GLint _uniformOffset;
	GLint _uniformURange;
	GLint _uniformVRange;

	GLuint _baseMapLoc;
    GLuint _baseMapTexId;
    GLint _positionLoc;
    GLint _colorLoc;
    GLint _texCoordLoc;
    GLuint VAO;
    GLuint vertexBuffer;

    typedef struct {
        float Position[3];
        float TexCoord[2];
    } Vertex;

};

#endif /* UVSPRITE_H_ */
