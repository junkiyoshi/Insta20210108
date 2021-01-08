#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofNoFill();
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	this->near_line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
	this->line_list.clear();
	this->near_line.clear();

	for (int i = 0; i < 25; i++) {

		ofMesh line;
		line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		for (int k = 0; k < 50; k++) {

			auto vertex = glm::vec3(
				ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + k) * 0.005), 0, 1, -200, 200),
				ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + k) * 0.005), 0, 1, -200, 200),
				ofMap(ofNoise(noise_seed.z, (ofGetFrameNum() + k) * 0.005), 0, 1, -200, 200));
			line.addVertex(vertex);
			line.addColor(this->base_color_list[i % this->base_color_list.size()]);
			if (k > 0) {

				line.addIndex(line.getNumVertices() - 1);
				line.addIndex(line.getNumVertices() - 2);
			}
		}
		this->line_list.push_back(line);
	}


	for (int i = 0; i < this->line_list.size(); i++) {

		for (int k = i + 1; k < this->line_list.size(); k++) {

			for (int m = 0; m < this->line_list[i].getNumVertices(); m++) {

				bool m_flag = false;
				int m_index = m;
				for (int p = 0; p < this->line_list[k].getNumVertices(); p++) {

					if (glm::distance(this->line_list[i].getVertex(m), this->line_list[k].getVertex(p)) < 35) {

						if (m_flag == false) {

							m_index = this->near_line.getNumVertices();
							this->near_line.addVertex(this->line_list[i].getVertex(m));
							this->near_line.addColor(this->line_list[i].getColor(m));
							m_flag = true;
						}

						this->near_line.addVertex(this->line_list[k].getVertex(p));
						this->near_line.addColor(this->line_list[k].getColor(p));

						this->near_line.addIndex(m_index);
						this->near_line.addIndex(this->near_line.getNumVertices() - 1);
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * -0.25);

	for (auto& line : this->line_list) {

		line.draw();
	}

	this->near_line.draw();

	ofDrawBox(400);

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}