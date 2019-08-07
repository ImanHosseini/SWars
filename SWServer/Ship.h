#pragma once
#include "glm.hpp"




#define PI 3.14159265358979f
const float DEFAULT_THROT_L = 1.2f;
const float DEFAULT_THROT_R = 0.03f;
const float DEFAULT_B_L = 1.0f;
const float DEFAULT_B_O = 1.0f;
const float DEFAULT_MASS = 1.0f;
const float DEFAULT_INERTIA = 1.0f;
const float EPS_V_L = 0.000001f;
const float EPS_V_R = 0.000001f;

typedef struct ShipPos {
	glm::vec2 pos;
	float theta;
	ShipPos(glm::vec2 pos) : pos(pos), theta(0.0f) {};
} ShipPos;

class Ship {
public:
	Ship(glm::vec2 pos) : pos(ShipPos{ pos }), vel(glm::vec2{ 0.0,0.0 }), acc(glm::vec2{ 0.0,0.0 }), w(0.0f), wdot(0.0f),
		mass(DEFAULT_MASS), b_l(DEFAULT_B_L), b_o(DEFAULT_B_O), throt_l(DEFAULT_THROT_L), throt_r(DEFAULT_THROT_R), inertia(DEFAULT_INERTIA) {};
	ShipPos pos;
	glm::vec2 vel;
	glm::vec2 acc;
	float w;
	float wdot;
	float mass, b_l, b_o, throt_l, throt_r, inertia;
	int keystate = 0;
	void update_ks(int ks) {
		keystate = ks;
	}
	void updatePos(float dt) {
		pos.pos = pos.pos + vel * dt;
		pos.pos.x = fmodf(pos.pos.x+1184.0f, 1184.0f);
		pos.pos.y = fmodf(pos.pos.y+761.0f, 761.0f);
		printf("t:%f-w:%f-wdot:%f\n",pos.theta,w,wdot);
		if(isnan(pos.theta)) pos.theta = 0.0f;
		if (isnan(w)) w = 0.0f;
		if (isnan(wdot)) wdot = 0.0f;
		pos.theta = fmodf(2.0f*PI+pos.theta + w * dt,2.0f*PI);
	}
	void updateVel(float dt) {
		// printf("%f - %f\n", vel.x, vel.y);
		vel = vel + acc * dt;
		w = w + wdot * dt;
	}
	void updateAcc(float dt) {
		float tr = 0.0f;
		bool tl = false;
		if (keystate % 2 == 1) tl = true;
		if ((keystate / 2) % 2 == 1 && (keystate/4)%2 ==0) tr = -1.0f;
		else if ((keystate / 4) % 2 == 1 && (keystate/2)%2==0) tr = 1.0f;
		glm::vec2 f_ext(0.0f, 0.0f);
		if (!(glm::length(vel) < EPS_V_L)) {
			f_ext -= (vel * b_l);
		}
		
		if (tl) {
			f_ext += glm::vec2(-sin(pos.theta), -cos(pos.theta)) * throt_l;
		}
		
		acc = f_ext * (1.0f / mass);
		float t_ext = 0.0f;
		if (!(abs(w) < EPS_V_R)) {
			t_ext += -b_o * w;
		}
		t_ext += throt_r * tr;
		wdot = t_ext / inertia;
	}
};
