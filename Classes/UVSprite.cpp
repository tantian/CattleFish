/*
 * UVSprite.cpp
 *
 *  Created on: 2015-8-14
 *      Author: guh
 */

#include "UVSprite.h"

UVSprite::UVSprite() {
	// TODO Auto-generated constructor stub

}

UVSprite::~UVSprite() {
	// TODO Auto-generated destructor stub
}


UVSprite* UVSprite::createWithSpriteFrame(SpriteFrame *spriteFrame) {
    UVSprite *sprite = new (std::nothrow) UVSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

//从plist中的frame创建
UVSprite* UVSprite::createWithSpriteFrameName(const char *spriteFrameName) {
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);

#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName);
    CCASSERT(frame != nullptr, msg);
#endif

    return createWithSpriteFrame(frame);
}

//从贴图文件直接创建
UVSprite* UVSprite::create(const char *pszFileName) {
    UVSprite *sprite = new (std::nothrow) UVSprite();
    if (sprite && sprite->initWithFile(pszFileName))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}


void UVSprite::update(float dt) {
    Sprite::update(dt);

    //更新u
    if(_AutoScrollU)
    {
        _AutoScrollCountU += dt * _AutoScrollSpeedU;
//        log("UVSPRITE Update UU %f ", _AutoScrollCountU);
    }

    //更新v
    if (_AutoScrollV) {
        _AutoScrollCountV += dt * _AutoScrollSpeedV;
//        log("UVSPRITE Update VV %f ", _AutoScrollCountU);
    }

    //如果超出范围从0开始
    if (_AutoScrollCountU > 1 || _AutoScrollCountU < -1) {
        _AutoScrollCountU = 0;
    }

    if (_AutoScrollCountV > 1 || _AutoScrollCountV < -1) {
        _AutoScrollCountV = 0;
    }
}


void UVSprite::loadShaderVertex(const char *vert, const char *frag)
{
	auto shader = new GLProgram();
    shader->initWithFilenames(vert, frag);

    shader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    shader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    shader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);

    shader->link();

    shader->updateUniforms();
    auto glprogramState = GLProgramState::getOrCreateWithGLProgram(shader);
    auto kuang = Director::getInstance()->getTextureCache()->addImage("water05.png");
    glprogramState->setUniformTexture("u_Texture0", kuang);
    glprogramState->setUniformVec4("u_light", Vec4(0.0, 0.0, 0.5, 0.0));
    _uniformOffset = glGetUniformLocation(shader->getProgram(), "texOffset");
    _uniformURange = glGetUniformLocation(shader->getProgram(), "uRange");
    _uniformVRange = glGetUniformLocation(shader->getProgram(), "vRange");

//    this->setGLProgram(shader);
    glprogramState->setGLProgram(shader);
    this->setGLProgramState(glprogramState);
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//    glGenBuffers( 1, &vertexBuffer );
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer );
    _baseMapLoc = glGetUniformLocation(shader->getProgram(), "CC_Texture0");
//    _positionLoc = glGetAttribLocation(shader->getProgram(), "a_position");
//    _texCoordLoc = glGetAttribLocation(shader->getProgram(), "a_texCoord");
//    _colorLoc = glGetAttribLocation(shader->getProgram(), "a_color");
    _baseMapTexId = this->getTexture()->getName();
    ccGLBindTexture2DN(0, _baseMapTexId);
    glUniform1i(_baseMapLoc, 0);

//    shader->release();
}

//void UVSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
//{
////    Sprite::draw(renderer, transform, flags);
//    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
//    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
////    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//    Mat4 modelViewMatrix;
////    Mat4::createLookAt(Vec3(), Vec3(), V3c3(), modelViewMatrix);
//    Mat4::createLookAt(Vec3(0, 0, 500), Vec3(0, 0, 10), Vec3(0, -1, 0), &modelViewMatrix);
//    Mat4 projectionMatrix;
//    Mat4::createPerspective(170, 360/300, 100.0, 42, &projectionMatrix);
////    Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, projectionMatrix);
//    Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMatrix);
//
//#define TEX_COORD_MAX   1
//#define TEX_COORD_HARD 0.5
//    Vertex Vertices[] = {
//
//            {{-0.75, -0, 0}, {0, TEX_COORD_MAX}},
//            {{0.75, -0, 0}, {TEX_COORD_MAX, TEX_COORD_MAX}},
//            {{0.75, 0.75, 0}, {TEX_COORD_MAX, 0}},
//            {{-0.75, 0.75, 0}, {0, 0}},
//    };
//    auto p = this->getGLProgram();
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // 激活缓冲区对象
//    // 用数据分配和初始化缓冲区对象
//    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertices),Vertices, GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(_positionLoc);
//    glEnableVertexAttribArray(_texCoordLoc);
//    // 指向指定一个指向顶点坐标的数组
//    glVertexAttribPointer(_positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
//    //  指定一个指向纹理的数组
//    glVertexAttribPointer(_texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));
//
//    p->use();
//    p->setUniformsForBuiltins();
//
//    p->setUniformLocationWith2f(_uniformURange, _quad.bl.texCoords.u, _quad.br.texCoords.u);
//    p->setUniformLocationWith2f(_uniformVRange, _quad.bl.texCoords.v, _quad.tl.texCoords.v);
//    float offsetU = (_quad.br.texCoords.u - _quad.bl.texCoords.u) * _AutoScrollCountU;
//    float offsetV = (_quad.tl.texCoords.v - _quad.bl.texCoords.v) * _AutoScrollCountV;
//
//    p->setUniformLocationWith2f(_uniformOffset, offsetU, offsetV);
//
//
//    glEnable(GL_DEPTH_TEST);
//    glBindVertexArray(VAO);
//
//    ccGLBindTexture2DN(0, _baseMapTexId);
////    GL::bindTexture2DN(0, _baseMapTexId);
//    glUniform1i(_baseMapLoc, 0); // unnecc in practice
////    log("-------- UC Sprite ");
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
////    log("-------- UC Sprite 11 ");
//    //绑定纹理贴图
//    glBindVertexArray(0);
//
//    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,6);
//
////    CHECK_GL_ERROR_DEBUG();
//    glDisable(GL_DEPTH_TEST);
//
//    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
//    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//}


void UVSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
//	auto p = this->getGLProgram();
	auto pstate = this->getGLProgramState();
//	auto program = p->getProgram();
	pstate->setUniformVec2(_uniformURange, Vec2(_quad.bl.texCoords.u, _quad.br.texCoords.u));
	pstate->setUniformVec2(_uniformVRange, Vec2(_quad.bl.texCoords.v, _quad.tl.texCoords.v));
	float offsetU = (_quad.br.texCoords.u - _quad.bl.texCoords.u) * _AutoScrollCountU;
	float offsetV = (_quad.tl.texCoords.v - _quad.bl.texCoords.v) * _AutoScrollCountV;
	pstate->setUniformVec2(_uniformOffset, Vec2(offsetU, offsetV));
//	pstate->setUniformTexture(_baseMapLoc, _baseMapTexId);
	this->setGLProgramState(pstate);

	Sprite::draw(renderer, transform, flags);
}


//void UVSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
//{
//#if CC_USE_CULLING
//    // Don't do calculate the culling if the transform was not updated
//    _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
//
//    if(_insideBounds)
//#endif
//    {
//
//#define TEX_COORD_MAX   1
//#define TEX_COORD_HARD 0.5
//    	Vertex Vertices[] = {
//    			{{-1, -1, 0}, {0, TEX_COORD_MAX}},
//    	        {{1, -1, 0}, {TEX_COORD_MAX, TEX_COORD_MAX}},
//    	        {{1, 1, 0}, {TEX_COORD_MAX, 0}},
//    	        {{-1, 1, 0}, {0, 0}},
//    	};
//   	    auto p = this->getGLProgram();
//   	    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // 激活缓冲区对象
//   	    // 用数据分配和初始化缓冲区对象
//   	    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertices),Vertices, GL_STATIC_DRAW);
//   	    glEnableVertexAttribArray(_positionLoc);
//        glEnableVertexAttribArray(_texCoordLoc);
//   	    // 指向指定一个指向顶点坐标的数组
//   	    glVertexAttribPointer(_positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
////   	    //  指定一个指向纹理的数组
//   	    glVertexAttribPointer(_texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));
////      glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr);
////glprogramState->setVertexAttribPointer(const string &name, Glint size, Glenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer);
//   	    p->use();
//        p->setUniformsForBuiltins();
//
//   	    p->setUniformLocationWith2f(_uniformURange, _quad.bl.texCoords.u, _quad.br.texCoords.u);
//   	    p->setUniformLocationWith2f(_uniformVRange, _quad.bl.texCoords.v, _quad.tl.texCoords.v);
//   	    float offsetU = (_quad.br.texCoords.u - _quad.bl.texCoords.u) * _AutoScrollCountU;
//   	    float offsetV = (_quad.tl.texCoords.v - _quad.bl.texCoords.v) * _AutoScrollCountV;
//
//    	p->setUniformLocationWith2f(_uniformOffset, offsetU, offsetV);
//
////    	glprogramState->setUniformVec2(_uniformURange, Vec2(_quad.bl.texCoords.u, _quad.br.texCoords.u));
////    	glprogramState->setUniformVec2(_uniformVRange, Vec2(_quad.bl.texCoords.v, _quad.tl.texCoords.v));
////    	glprogramState->setUniformVec2(_uniformOffset, Vec2(offsetU, offsetV));
////    	glprogramState->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
////    	glprogramState->setVertexAttribPointer("a_texCoord", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));
////    	glprogramState->setUniformTexture("u_Texture0", _baseMapTexId);
//
//    	glEnable(GL_DEPTH_TEST);
//        glBindVertexArray(VAO);
//
//   	    ccGLBindTexture2DN(0, _baseMapTexId);
//    	//    GL::bindTexture2DN(0, _baseMapTexId);
//   	    glUniform1i(_baseMapLoc, 0); // unnecc in practice
//    	//    log("-------- UC Sprite ");
// 	    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    	//    log("-------- UC Sprite 11 ");
//    	    //绑定纹理贴图
//    	glBindVertexArray(0);
////
//    	auto glprogramState = getGLProgramState();
//    	glprogramState->setGLProgram(p);
//
//        _quadCommand.init(_globalZOrder, _texture->getName(), glprogramState, _blendFunc, &_quad, 1, transform, flags);
//        renderer->addCommand(&_quadCommand);
//
//#if CC_SPRITE_DEBUG_DRAW
//        _debugDrawNode->clear();
//        Vec2 vertices[4] = {
//            Vec2( _quad.bl.vertices.x, _quad.bl.vertices.y ),
//            Vec2( _quad.br.vertices.x, _quad.br.vertices.y ),
//            Vec2( _quad.tr.vertices.x, _quad.tr.vertices.y ),
//            Vec2( _quad.tl.vertices.x, _quad.tl.vertices.y ),
//        };
//        _debugDrawNode->drawPoly(vertices, 4, true, Color4F(1.0, 1.0, 1.0, 1.0));
//#endif //CC_SPRITE_DEBUG_DRAW
//    }
//}



