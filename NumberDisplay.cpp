#include "NumberDisplay.h"

CNumberDisplay::CNumberDisplay(bool align)
    : color{1.f, 1.f, 1.f}, position{0.f, 0.f, 0.f},
      rotation{M_PI / 2.f, 0.f,0.f}, scale{1.f},
      alignFromLeft{align}, text{"0"}, letterWidth {0.65f} {
    for (int i = 0; i < 10; ++i) {
        models[i] = g3d::resourceManager.get("num_"+std::to_string(i)+"_model");
    }
}


CNumberDisplay::~CNumberDisplay(){ }


int CNumberDisplay::characterToNumber(char c){
	auto ret = (int)c-(int)'0';
	return ret;
}

void CNumberDisplay::setText(const std::string &str){
	text = str;
}

void CNumberDisplay::setText(const int number){
	text = std::to_string(number);
}

std::string CNumberDisplay::getText() { return text; }

void CNumberDisplay::setAlign(bool align) { alignFromLeft = align; }

void CNumberDisplay::setColor(const glm::vec3 &color) { this->color = color; }

void CNumberDisplay::setPosition(const glm::vec3 &position) {
    this->position = position;
}

void CNumberDisplay::setRotation(const glm::vec3 &rotation) {
    this->rotation = rotation;
}

void CNumberDisplay::setScale(const float scale) { this->scale = scale; }

void CNumberDisplay::drawNumber(const glm::vec3 &pos, int ID) {
    models[ID]->setRotation(rotation);
    models[ID]->setPosition(pos);
    models[ID]->setScale({scale, scale, scale});
    models[ID]->draw();
}

void CNumberDisplay::draw(){
	g3d::engine->getDefaultShader()->setUniform("colorModulation", color);

	for (std::size_t i = 0; i < text.size(); i++) {
		int numberID=characterToNumber(text[alignFromLeft?(i):(text.size()-i-1)]);
		drawNumber(position+glm::vec3{alignFromLeft?(letterWidth*i*scale):(-letterWidth*scale-letterWidth*scale*i),0.f,0.f}, numberID);
	}
	g3d::engine->getDefaultShader()->setUniform("colorModulation", glm::vec3(1.f, 1.f, 1.f));
}
