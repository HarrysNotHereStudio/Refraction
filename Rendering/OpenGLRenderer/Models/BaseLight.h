#pragma once

#include "BaseModel.h"

class BaseLight
{
public:
	BaseModel* m_pLightModel;

	BaseLight() {
		m_pLightModel = new BaseModel("./Rendering/Resources/models/Primitives/primitiveSphere.obj");
		m_pLightModel->m_pTransform->Translate(glm::vec3(10.0f, 5.0f, 10.0f));
		m_pLightModel->m_pTransform->Scale(glm::vec3(0.02f));
	}

	glm::vec3 getLightColor() const { return m_lightColor * m_lightIntensity; };
	void setLightColor(glm::vec3 newColor) {
		m_lightColor = newColor;
		m_pLightModel->m_pShader->activate();
		m_pLightModel->m_pShader->setUniformVec3("lightColor", m_lightColor);
	};
private:
	float m_lightIntensity = 1.0f;
	glm::vec3 m_lightColor = glm::vec3(1.0f);
};

